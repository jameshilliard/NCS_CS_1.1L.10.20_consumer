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
 * $Id: center.c,v 1.20 2003/01/08 09:31:37 bazsi Exp $
 *
 ***************************************************************************/

#include "center.h"
#include "format.h"
#include "filters.h"
#include "resource.h"
#include "cfgfile.h"

#include <assert.h>

#define CLASS_DEFINE
#include "center.h.x"
#undef CLASS_DEFINE

#include "center.c.x"

/* log_endpoint_info */

struct log_endpoint_info *make_log_endpoint_info(int type, const char *name)
{
	NEW(log_endpoint_info, self);
	
	self->type = type;
	self->name = c_format("%z", name);
	return self;
}

/* log_connection */

struct log_connection *make_log_connection(struct log_endpoint_info *nodes,
					   UINT32 flags)
{
	NEW(log_connection, self);
	struct log_endpoint_info *last_src, *last_filter, *last_dest;
	
	self->flags = flags;
	
	/* Preseve the order of endpoints, it might be important */
	for (last_src = last_filter = last_dest = NULL; nodes; nodes = nodes->next) {
		switch (nodes->type)
		{
		case EP_SOURCE:
			if (last_src) {
				last_src->next = nodes;
				last_src = last_src->next;
			} else
				self->sources = last_src = nodes;
			break;
		case EP_FILTER:
			if (last_filter) {
				last_filter->next = nodes;
				last_filter = last_filter->next;
			} else
				self->filters = last_filter = nodes;
			break;
		case EP_DESTINATION:
			if (last_dest) {
				last_dest->next = nodes;
				last_dest = last_dest->next;
			} else
				self->destinations = last_dest = nodes;
			break;
		} /* switch in for */
	}

	if (last_src)
		last_src->next = NULL;
	if (last_filter)
		last_filter->next = NULL;
	if (last_dest)
		last_dest->next = NULL;

	return self;
}


/* CLASS:
     (class
       (name log_center)
       (super log_handler)
       (vars
         (connections object log_connection)))
*/

static void do_distribute_log(struct log_handler *c, struct log_info *msg)
{
	CAST(log_center, self, c);
	struct log_connection *p;
	int match, fallbacks, have_fallbacks = 1;

	for (match = 0, fallbacks = 0; !match && have_fallbacks && (fallbacks <= 1); fallbacks++) {
		have_fallbacks = 1;
		for (p = self->connections; p; p = p->next) {
			struct log_endpoint_info *s, *f, *d;
	
			if ((!fallbacks && (p->flags & LOG_CONN_FALLBACK))) {
				have_fallbacks = 1;
				continue;
			}
			else if (fallbacks && !(p->flags & LOG_CONN_FALLBACK))
				continue;

			/* Source checking (source is a kind of filter) */
			if (!(p->flags & LOG_CONN_CATCHALL)) {
				for (s = p->sources; s; s = s->next) {
					assert(s->ref);
					if ((struct ol_object *) msg->source == s->ref)
						break;
				}
				if (!s)
					goto next_connection;
			}
	
			/* Filter checking */
			for (f = p->filters; f; f = f->next) {
				assert(f->ref);
				if (!FILTER_RULE_EVAL(((struct log_filter *) f->ref)->root,
						(struct log_filter *) f->ref, msg))
					goto next_connection;
			}
	
			/* Matched, let's distribute it */
			match = 1;
			for (d = p->destinations; d; d = d->next) {
				assert(d->ref);
				HANDLE_LOG(d->ref, log_info_use(msg));
			}
	
			if (p->flags & LOG_CONN_FINAL) {
				break;
			}
			
		next_connection:
			;
		}
	}

	log_info_free(msg);
}

struct log_handler *make_log_center(struct log_connection *connections)
{
	NEW(log_center, self);
	
	self->super.handler = do_distribute_log;
	self->connections = connections;
	return &self->super;
}

/* CLASS:
     (class
       (name driver_reinit)
       (super callback)
       (vars
         (handler object log_handler)
	 (cfg object syslog_config)))
*/

static void do_reinit(struct callback *c)
{
	CAST(driver_reinit, self, c);

	LOG_HANDLER_INIT(self->handler, self->cfg, NULL);
}

struct callback *
make_driver_reinit(struct log_handler *handler, struct syslog_config *cfg)
{
	NEW(driver_reinit, self);
	self->super.f = do_reinit;
	self->handler = handler;
	self->cfg = cfg;
	return &self->super;
}

/* CLASS:
     (class
       (name driver_reinit_on_close)
       (super close_callback)
       (vars
         (handler object log_handler)
	 (res simple "struct resource_node *")
	 (cfg object syslog_config)
	 (desc string)
         (time_reopen simple int)))
*/

static int do_driver_on_close(struct close_callback *c, int reason)
{
	CAST(driver_reinit_on_close, self, c);

	/* schedule a reopen */
	if (self->time_reopen)
		werror("Connection broken to %S, reopening in %i seconds\n", self->desc, self->time_reopen);
	if (self->res)
		KILL_RESOURCE_NODE(self->cfg->resources, self->res);
	io_callout(self->cfg->backend, self->time_reopen, 
		   make_driver_reinit(self->handler, self->cfg));
	
	return ST_OK | ST_GOON;
}

struct close_callback *
make_driver_reinit_on_close(struct log_handler *handler, 
			    struct resource_node *res, 
			    int time_reopen, 
			    struct syslog_config *cfg,
			    struct ol_string *desc)
{
	NEW(driver_reinit_on_close, self);

	self->super.f = do_driver_on_close;
	self->handler = handler;
	self->res = res;
	self->time_reopen = time_reopen;
	self->cfg = cfg;
	self->desc = desc;
	return &self->super;
}

