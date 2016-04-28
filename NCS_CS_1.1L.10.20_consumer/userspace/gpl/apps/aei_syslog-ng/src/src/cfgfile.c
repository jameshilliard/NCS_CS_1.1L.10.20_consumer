/***************************************************************************
 *
 * Copyright (c) 1999 Balázs Scheidler
 * Copyright (c) 1999-2001 BalaBit IT Ltd.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Inspired by nsyslog, originally written by Darren Reed.
 *
 * $Id: cfgfile.c,v 1.40.4.4 2007/02/02 08:58:30 bazsi Exp $
 *
 ***************************************************************************/

#include "cfgfile.h"
#include "nscache.h"

#include <string.h>
#include <assert.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>

#define CLASS_DEFINE
#include "cfgfile.h.x"
#undef CLASS_DEFINE

struct syslog_config *configuration;

void add_dest_group(struct log_dest_group *grp)
{
	grp->next_dest_group = configuration->destinations;
	configuration->destinations = grp;
}

void add_source_group(struct log_source_group *grp)
{
	grp->next_source_group = configuration->sources;
	configuration->sources = grp;
}

void add_filter_rule(struct log_filter *filter)
{
	filter->next_filter = configuration->filters;
	if (filter->next_filter)
	  filter->next_filter->prev_filter = filter;
	filter->prev_filter = NULL;
	configuration->filters = filter;
}

void add_log_connection(struct log_connection *conn)
{
	conn->next = NULL;
	if (configuration->last_connection) {
		configuration->last_connection->next = conn;
	}
	else {
		configuration->connections = conn;
	}
	configuration->last_connection = conn;
}

void cfg_set_owner(char *uid)
{
	struct passwd *pwd;

	pwd = getpwnam(uid);
	if (pwd)
		configuration->uid = pwd->pw_uid;
	else
		configuration->uid = atoi(uid);
}

void cfg_set_group(char *gid)
{
	struct group *grp;

	grp = getgrnam(gid);
	if (grp)
		configuration->gid = grp->gr_gid;
	else
		configuration->gid = atoi(gid);
}

void cfg_set_perm(int perm)
{
	configuration->perm = perm;
}

void cfg_set_dir_owner(char *uid)
{
	struct passwd *pwd;

	pwd = getpwnam(uid);
	if (pwd)
		configuration->dir_uid = pwd->pw_uid;
	else
		configuration->dir_uid = atoi(uid);
}

void cfg_set_dir_group(char *gid)
{
	struct group *grp;

	grp = getgrnam(gid);
	if (grp)
		configuration->dir_gid = grp->gr_gid;
	else
		configuration->dir_gid = atoi(gid);
}

void cfg_set_dir_perm(int perm)
{
	configuration->dir_perm = perm;
}

void cfg_set_bad_hostname(char *bad_hostname)
{
	configuration->bad_hostname = (UINT8 *) bad_hostname;
}

struct persistent_info *
make_persistent_info(struct ol_string *name, 
		     struct ol_object *o, 
		     void (*kill)(struct ol_string *name, struct ol_object *obj))
{
	NEW(persistent_info, self);

	self->name = name;
	self->o = o;
	self->kill = kill;
	return self;
}

struct persistent_info *
find_persistent_info(struct persistent_config *c, UINT32 length, const char *name)
{
	struct persistent_info *p;

	for (p = c->nodes; p; p = p->next) {
		if (p->name->length == length &&
		    memcmp(p->name->data, name, length) == 0) {
			return p;
		}
	}
	return NULL;
}

void add_persistent_info(struct persistent_config *c, 
			 struct persistent_info *node)
{
	struct persistent_info *p;

	p = find_persistent_info(c, node->name->length, (char *) node->name->data);
	if (p) {
		werror("You have a fatal error in your configuration, conflicting persistent names for one or more sources/destinations, name='%S'\n", node->name);
	}
	else {
		node->next = c->nodes;
		c->nodes = node;
	}
}

void kill_persistent_config(struct persistent_config *persistent)
{
	struct persistent_info *p;
	
	for (p = persistent->nodes; p; p = p->next) {
		if (p->kill)
			(p->kill)(p->name, p->o);
	}
	persistent->nodes = NULL;
}

struct persistent_config *
make_persistent_config(void)
{
	NEW(persistent_config, persistent);
	return persistent;
}

/* cfgfile */
#define RESOLVE_VAR(s, first, next, i) \
do \
{ \
        for (s = first; s; s = s->next) { \
	         if (i->name->length == s->name->length && \
		     strncmp((char *) i->name->data, (char *) s->name->data, i->name->length) == 0) { \
			 i->ref = (struct ol_object *) s; \
			 break; \
		 } \
        } \
	if (i->ref == NULL) { \
		 werror("unresolved reference: %S\n", i->name); \
		 return 0; \
	} \
} while (0)

static int do_init_config(struct syslog_config *self, struct persistent_config *p)
{
	struct log_source_group *s;
	struct log_filter *f;
	struct log_dest_group *d;
	struct log_connection *c;
	struct log_endpoint_info *i;
	struct log_handler *center;
	int res, n;

	gc_idle_threshold = self->gc_idle_threshold;
	gc_busy_threshold = self->gc_busy_threshold;
	center = make_log_center(self->connections);

	if (self->dns_cache)
		self->cache = nscache_new(self->dns_cache_size,
			                   self->dns_cache_expire,
			                   self->dns_cache_expire_failed);
	else
		self->cache = NULL;

	for (s = self->sources; s; s = s->next_source_group) {
		res = LOG_HANDLER_INIT(s, self, p);
		append_log_handler(s, center);
		if (res & ST_FAIL)
			return 0;
	}
	
	for (d = self->destinations; d; d = d->next_dest_group) {
		res = LOG_HANDLER_INIT(d, self, p);
		if (res & ST_FAIL)
			return 0;
	}

	if (!self->internal) {
		werror("Warning: No source refers to internal messages, they'll go to /dev/null\n");
	}
	
	for (c = self->connections, n = 0; c; c = c->next, n++) {
		if (!c->sources && !(c->flags & LOG_CONN_CATCHALL)) {
			werror("Warning: connection %i doesn't have any sources\n", n);
		}
		for (i = c->sources; i; i = i->next) {
			RESOLVE_VAR(s, self->sources, next_source_group, i);
		}
		for (i = c->filters; i; i = i->next) {
			RESOLVE_VAR(f, self->filters, next_filter, i);
		}
		for (i = c->destinations; i; i = i->next) {
			RESOLVE_VAR(d, self->destinations, next_dest_group, i);
		}
	}

	return 1;
}

static int do_destroy_config(struct syslog_config *self, struct persistent_config *persistent)
{
	struct log_dest_group *d;
	struct log_source_group *s;

	self->living = 1;
	for (s = self->sources; s; s = s->next_source_group) {
		if (s->super.super.destroy)
			LOG_HANDLER_DESTROY(s, self, persistent);
	}
	
	for (d = self->destinations; d; d = d->next_dest_group) {
		if (d->super.destroy)
			LOG_HANDLER_DESTROY(d, self, persistent);
	}
	
	KILL_RESOURCE_LIST(self->resources);
	
	if (self->dns_cache && self->cache) {
		nscache_free(self->cache);
		self->cache = NULL;
	}
	return 0;
}

extern FILE *yyin;
extern int yyparse();
extern void lex_init(FILE *);
extern int yydebug;
extern int linenum;

struct syslog_config *make_syslog_config(const char *name, struct io_backend *backend)
{
	FILE *cfg;
	int res;
	
	NEW(syslog_config, self);
	
	configuration = self;
	self->init = do_init_config;
	self->destroy = do_destroy_config;
	self->backend = backend;
	self->living = 1;
	self->sync_freq = 0;
	self->mark_freq = 1200; /* 20 minutes */
	self->stats_freq = 600;
	self->chain_hostnames = 1;
	self->use_fqdn = 0;
	self->use_dns = 1;
	self->time_reopen = 60;
	self->time_reap = 60;
	self->log_fifo_size = 1000;
	self->gc_idle_threshold = 100;
	self->gc_busy_threshold = 3000;
	self->uid = 0;
	self->gid = 0;
	self->perm = 0600;
	self->dir_uid = 0;
	self->dir_gid = 0;
	self->dir_perm = 0700;
	self->dns_cache = 1;
	self->dns_cache_size = 1007;
	self->dns_cache_expire = 3600;
	self->dns_cache_expire_failed = 60;
	self->log_msg_size = 2048;
	self->bad_hostname = NULL;
	self->sanitize_filenames = 1;
	if ((cfg = fopen(name, "r")) != NULL) {
		lex_init(cfg);
		res = yyparse();
		fclose(cfg);
		if (!res) {
			/* successfully parsed */
			self->resources = empty_resource_list();
			return self;
		}
	}
	else {
		werror("Cannot open configuration file %z for reading\n", name);
	}
	KILL(self);
	configuration = NULL;
	return NULL;
}

