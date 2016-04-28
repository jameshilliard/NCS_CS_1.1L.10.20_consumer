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
 * $Id: sources.c,v 1.37.4.11 2006/06/12 07:50:47 bazsi Exp $
 *
 ***************************************************************************/

#include "sources.h"
#include "xalloc.h"
#include "format.h"
#include "utils.h"
#include "cfgfile.h"
#include "objtypes.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define  CLASS_DEFINE
#include "sources.h.x"
#undef CLASS_DEFINE

#include "sources.c.x"
#include "nscache.h"

/* CLASS:
     (class
       (name log_reader)
       (super read_handler)
       (vars
         (dgram simple UINT32)
         (prefix pointer UINT8)
	 (pos simple UINT32)
	 (buffer space UINT8)
	 (max_log_line simple UINT32)
	 (pad_size simple UINT32)
	 (msg_flags simple UINT32)
	 (bad_hostname special-struct regex_t #f regfree)
         (next object log_handler)))
*/

static int 
do_handle_line(struct log_reader *self, 
	       UINT32 length, UINT8 *data, 
	       abstract_addr *addr, 
	       size_t addrlen)
{
	struct log_info *logmsg;

	logmsg = make_log_info(length, data, self->prefix, self->msg_flags, &self->bad_hostname);
	if (addrlen) {
		logmsg->saddr = sockaddr2address_info(addrlen, addr);
	}
	HANDLE_LOG(self->next, logmsg);
	return ST_OK | ST_GOON;
}

static int 
do_read_line(struct read_handler **h, 
	     struct abstract_read *read)
{
	CAST(log_reader, closure, *h);
	UINT8 *eol, *start;
	UINT32 length;
	int n;
	char sabuf[256];
	socklen_t salen = sizeof(sabuf);
	int fetch_count = 0;
	int fetch_max = (closure->msg_flags & LF_NO_MULTI_READ) ? 1 : 30;
	
	while (fetch_count < fetch_max) {

		if (!closure->dgram) {
			if (closure->pad_size)
				n = A_READ(read, LIBOL_MIN(closure->max_log_line, closure->pad_size), closure->buffer + closure->pos);
			else
				n = A_READ(read, closure->max_log_line - closure->pos, closure->buffer + closure->pos);
			salen = 0;
		}
		else
			n = A_RECV(read, closure->max_log_line - closure->pos, closure->buffer + closure->pos, (abstract_addr *) &sabuf, &salen);

		switch(n) {
		case 0:
			return ST_OK | ST_GOON;
		case A_FAIL:
			/* Fall through */
			return ST_FAIL | ST_CLOSE;
		case A_EOF:
			return ST_OK | ST_CLOSE;
		}
		closure->pos += n;

		eol = memchr(closure->buffer, '\0', closure->pos);
		if (eol == NULL)
			eol = memchr(closure->buffer, '\n', closure->pos);
		if (closure->pad_size && eol) {
			do_handle_line(closure, eol - closure->buffer, closure->buffer, salen ? (abstract_addr *) &sabuf : NULL, salen);
			closure->pos = 0;
			goto next_fetch;
		}
		if (closure->dgram || (!eol && closure->pos == closure->max_log_line)) {
			/* we don't have a terminating nl nor \0, and our buffer is
			   full or we are a datagram receiver, when the message is in
			   its own packet.
			 */
			if (!eol && closure->pos == closure->max_log_line) {
				werror("Message length overflow, line is split, log_msg_size=%i\n", closure->max_log_line);
			}
			if (closure->dgram) {
				/* strip one trailing LF or NUL character */
				if (closure->pos > 0 && 
				    (closure->buffer[closure->pos - 1] == '\n' || 
				     closure->buffer[closure->pos - 1] == '\0'))
					closure->pos--;
			}
			do_handle_line(closure, closure->pos, closure->buffer, salen ? (abstract_addr *) &sabuf : NULL, salen);
			closure->pos = 0;
			goto next_fetch;
		}
		start = closure->buffer;
		while (eol) {
			/* eol points at the newline character. end points at the
			 * character terminating the line, which may be a carriage
			 * return preceeding the newline. */
			UINT8 *end = eol;

			while ((end > start) && (end[-1] == '\r' || end[-1] == '\n' || end[-1] == 0))
				end--;

			length = end - start;
			
			if (length)
				do_handle_line(closure, length, start, salen ? (abstract_addr *) &sabuf : NULL , salen);

			start = eol + 1;

			eol = memchr(start, '\0', &closure->buffer[closure->pos] - start);
			if (eol == NULL)
				eol = memchr(start, '\n', &closure->buffer[closure->pos] - start);

		}
		memmove(closure->buffer, start, &closure->buffer[closure->pos] - start);
		closure->pos = &closure->buffer[closure->pos] - start;
		
	next_fetch:
		fetch_count++;
	}
	return ST_OK | ST_GOON;
}

struct read_handler *
make_log_reader(UINT32 dgram, 
		UINT8 *prefix,
		UINT32 max_log_line,
		UINT32 pad_size,
		UINT32 msg_flags,
		UINT8 *hostname_re,
		struct log_handler *next)
{
	NEW(log_reader, self);

	self->super.handler = do_read_line;
	self->dgram = dgram;
	self->next = next;
	self->prefix = prefix;
	self->max_log_line = LIBOL_MAX(max_log_line, pad_size);
	self->pad_size = pad_size;
	self->msg_flags = msg_flags;
	self->buffer = ol_space_alloc(self->max_log_line);
	if (hostname_re == NULL)
		regcomp(&self->bad_hostname, "^$", REG_NOSUB | REG_EXTENDED);
	else
		regcomp(&self->bad_hostname, (char *) hostname_re, REG_NOSUB | REG_EXTENDED);
	return &self->super;
}

/* source_group */

static struct ol_string *get_source_hostname(struct address_info *a, int usedns, int usefqdn, struct nscache *cache)
{
	struct ol_string *name;
	static struct ol_string *hostname = NULL;
	
	if (a && a->super.isa == &inet_address_info_class) {
		CAST(inet_address_info, inet_addr, a);
		char *hname, *p;

		if (usedns && cache) {
			hname = nscache_lookup(cache, inet_addr->sa.sin_addr);
		}
		else if (usedns) {
			struct hostent *hp;

			hp = gethostbyaddr((char *) &(inet_addr->sa.sin_addr),
					   sizeof(struct in_addr), AF_INET);
			hname = (hp && hp->h_name) ? hp->h_name : NULL;
		} 
		else
			hname = NULL;

		if (!hname) {
			return ol_string_use(inet_addr->ip);
		}
		else {
			if (!usefqdn) {
				p = strchr(hname, '.');
				if (p) *p = 0;
			}
		}
		name = c_format_cstring("%z", hname);
	}
	else {
		if (!hostname) {
			char buf[256];
			if (usefqdn) {
				gethostname(buf, sizeof(buf) - 1);
				buf[sizeof(buf) - 1] = 0;
				// Check if hostname includes a . else do an fqdn lookup
				if (strchr(buf, '.') == NULL) {
					struct hostent *result = gethostbyname(buf);
					if (result) {
						strncpy(buf, result->h_name, sizeof(buf) - 1);
					}
				}
			}
			else {
				getshorthostname(buf, sizeof(buf));
			}
			hostname = c_format_cstring("%z", buf);
		}

		ol_string_use(hostname);
		name = hostname;
	}
	return name;
}

static void do_add_source_name(struct log_handler *c, struct log_info *logmsg)
{
	CAST(log_source_group, self, c);
	struct ol_string *name;


	if (!self->super.next) {
		log_info_free(logmsg);
		return;
	}
	logmsg->source = c;
	name = get_source_hostname(logmsg->saddr, 
				   self->use_dns, self->use_fqdn,
				   self->cache);
	logmsg->host_from = name;
	if (!self->keep_hostname || !logmsg->host) {
		ol_string_use(name);
		if (self->chain_hostnames) {
			if (logmsg->flags & LF_LOCAL) {
				/* local */
				ol_string_free(logmsg->host);
				logmsg->host = c_format("%S@%fS", 
							self->name, name);
			}
			else if (!logmsg->host) {
				/* remote && no hostname */
				logmsg->host = c_format("%S/%fS", name, name);
			} 
			else {
				/* everything else, append source hostname */
				if (logmsg->host)
					logmsg->host = 
						c_format("%fS/%fS", 
							 logmsg->host, name);
				else
					logmsg->host = 
						c_format("%fS", name);
			}
		}
		else {
			ol_string_free(logmsg->host);
			logmsg->host = c_format("%fS", name);
		}
	}
	HANDLE_LOG(self->super.next, logmsg);
}

static int do_init_group(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(log_source_group, self, c);
	int res;
	
	struct log_source_driver *drv;
	
	self->chain_hostnames = cfg->chain_hostnames;
	self->use_dns = cfg->use_dns;
	self->use_fqdn = cfg->use_fqdn;
	self->cache = cfg->cache;
	self->keep_hostname = cfg->keep_hostname;
	res = 0;
	for (drv = self->drivers; drv; drv = drv->next_driver) {
		res = LOG_HANDLER_INIT(drv, cfg, persistent);
		if (res & ST_QUIT) 
			break;
	}
	return res;
}

static void do_destroy_group(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(log_source_group, self, c);
	
	struct log_source_driver *drv;
	
	for (drv = self->drivers; drv; drv = drv->next_driver) {
		if (drv->super.super.destroy)
			LOG_HANDLER_DESTROY(drv, cfg, persistent);
	}
}

void set_source_drivers(struct log_source_group *src, struct log_source_driver *drvs)
{
	struct log_source_driver *drv;
	
	src->drivers = drvs;
	for (drv = src->drivers; drv; drv = drv->next_driver) {
		append_log_handler(drv, src);
	}
}

struct log_source_group *make_source_group(const char *name, struct log_source_driver *drvs)
{
	NEW(log_source_group, self);
  
	self->super.super.init = do_init_group;  
	self->super.super.destroy = do_destroy_group;
	self->super.super.handler = do_add_source_name;
	self->name = c_format("%z", name);
	set_source_drivers(self, drvs);
	
	return self;
}
