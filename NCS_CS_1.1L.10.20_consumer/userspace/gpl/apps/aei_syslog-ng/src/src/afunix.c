/***************************************************************************
 *
 * Copyright (c) 1999 Balázs Scheidler
 * Copyright (c) 1999-2001 BalaBit IT Ltd
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
 * $Id: afunix.c,v 1.27.4.3 2007/02/02 08:58:30 bazsi Exp $
 *
 ***************************************************************************/

#include "afsocket.h"
#include "cfgfile.h"
#include "format.h"
#include "pkt_buffer.h"
#include "queue.h"
#include "center.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "afunix.c.x"
#include "macros.h"


/* af_unix source connection */

static int
do_init_afunix_connection(struct log_handler *c, 
			  struct syslog_config *cfg,
			  struct persistent_config *persistent)
{
	CAST(afsocket_source_connection, self, c);

	io_read(self->client, 
		make_log_reader(!!(self->owner->flags & AFSOCKET_DGRAM), NULL, cfg->log_msg_size, 0, cfg->check_hostname ? LF_CHECK_HOSTNAME : 0, cfg->bad_hostname, c), 
		make_afsocket_source_close_callback(self));
	
	return ST_OK | ST_GOON;
}

static void do_handle_afunix_line(struct log_handler *c, struct log_info *msg)
{
	CAST(afsocket_source_connection, self, c);

	if (msg->host) {
		ol_string_free(msg->host);
		msg->host = NULL;
	}
	msg->flags |= LF_LOCAL;

	HANDLE_LOG(self->super.next, msg);
}

static int 
do_close_afunix_connection(struct afsocket_source_connection *c,
			   int close_reason)
{
	assert(c->owner);
	c->owner->num_connections--;
	object_queue_remove(c->queue_node);
	return ST_OK | ST_GOON;
}

static void
do_destroy_afunix_connection(struct log_handler *c,
			     struct syslog_config *cfg,
			     struct persistent_config *persistent)
{
	CAST(afsocket_source_connection, self, c);

	closekill_fd(&self->client->super, 0);
}

static struct afsocket_source_connection *
make_afunix_source_connection(struct io_fd *client, 
			      struct address_info *client_addr,
			      struct afsocket_source *owner,
			      struct log_handler *next)
{
	NEW(afsocket_source_connection, self);

	self->super.super.init = do_init_afunix_connection;
	self->super.super.handler = do_handle_afunix_line;
	self->super.super.destroy = do_destroy_afunix_connection;
	self->super.next = next;
	self->owner = owner;
	self->close = do_close_afunix_connection;
	self->client = client;
	self->client_addr = client_addr;
	return self;
}

/* CLASS:
     (class
       (name afunix_source)
       (super afsocket_source)
       (vars
         (uid . int)
	 (gid . int)
	 (perm simple int)
	 (listen object listen_fd)
	 (cfg object syslog_config)))
*/

void afunix_set_owner(struct log_source_driver *c, const char *owner)
{
	CAST(afunix_source, self, c);
	struct passwd *pwd;

	pwd = getpwnam(owner);
	if (pwd)
		self->uid = pwd->pw_uid;
	else
		self->uid = atoi(owner);

}

void afunix_set_group(struct log_source_driver *c, const char *group)
{
	CAST(afunix_source, self, c);
	struct group *grp;

	grp = getgrnam(group);
	if (grp)
		self->gid = grp->gr_gid;
	else
		self->gid = atoi(group);
}

void afunix_set_perm(struct log_source_driver *c, int perm)
{
	CAST(afunix_source, self, c);

	self->perm = perm;
}

static int
do_open_unix_connection(struct afsocket_source *c,
			struct io_fd *client,
			struct address_info *client_addr)
{
	CAST(afunix_source, self, c);
	struct afsocket_source_connection *conn;
	UINT32 res;

	if (c->num_connections >= c->max_connections) {
		notice("Error accepting AF_UNIX connection, opened connections: %i, max: %i\n", c->num_connections, c->max_connections);
		close_fd(&client->super, 0);
		return ST_OK | ST_GOON;
	}
	else {
		conn = make_afunix_source_connection(client, client_addr, c, self->super.super.super.next);
		res = LOG_HANDLER_INIT(conn, self->cfg, NULL);
		if ((res & ST_FAIL) == 0) {
			conn->queue_node = 
				object_queue_add_tail(self->super.connections, &conn->super.super.super);
			c->num_connections++;
		}
		return res;
	}
}

int do_init_afunix_source(struct log_handler *c, 
			  struct syslog_config *cfg,
			  struct persistent_config *persistent)
{
	CAST(afunix_source, self, c);
	CAST(unix_address_info, unix_addr, self->super.bind_addr);
	int fd;

	if (persistent && (self->super.flags & AFSOCKET_KEEP_ALIVE) &&
	    (self->super.flags & AFSOCKET_STREAM)) {
		struct ol_string *persistent_name;
		struct persistent_info *p_info;

		persistent_name = c_format("afunix_stream_%S", unix_addr->path);
		p_info = find_persistent_info(persistent, persistent_name->length, (char *) persistent_name->data);
		ol_string_free(persistent_name);

		if (p_info) {
			CAST(object_queue, p, p_info->o);
			
			self->super.num_connections = 0;
			{
				FOR_OBJECT_QUEUE(p, n) {
					CAST_SUBTYPE(afsocket_source_connection, h, n);
					h->owner = &self->super;
					append_log_handler(&h->super, self->super.super.super.next);
					self->super.num_connections++;
				}
			}
			self->super.connections = p;
			p_info->kill = NULL;

		}
	}

	self->cfg = cfg;
	fd = io_open_socket(unix_addr->super.family, 
			    self->super.flags & AFSOCKET_DGRAM ? SOCK_DGRAM : SOCK_STREAM,
			    0, 
			    &unix_addr->super);
	if (fd < 0) {
		return ST_QUIT | ST_FAIL;
	}

	chown((char *) unix_addr->path->data, self->uid, self->gid);
	chmod((char *) unix_addr->path->data, self->perm);

	switch (self->super.flags & 0x0003) {
	case AFSOCKET_STREAM: {
		if (!(self->listen = 
		     io_listen(cfg->backend, fd,
			       make_afsocket_accept_callback
			       (cfg->backend, &self->super)))) {
			close(fd);
			return ST_FAIL | ST_QUIT;
		}
		break;
	}
			
	case AFSOCKET_DGRAM: {
		struct afsocket_source_connection *conn;
		struct io_fd *client = make_io_fd(cfg->backend, fd, NULL);
		int res;
				
		self->super.num_connections = 1;
		conn = make_afunix_source_connection(client, NULL, &self->super, self->super.super.super.next);
		res = LOG_HANDLER_INIT(conn, cfg, NULL);
		
		if ((res & ST_FAIL) == 0) {
			conn->queue_node = 
				object_queue_add_tail(self->super.connections, &conn->super.super.super);
		}
		break;
	}
	default:
		return ST_FAIL | ST_QUIT;
	}

	return ST_OK | ST_GOON;		
}

static void
do_destroy_afunix_source(struct log_handler *c,
			 struct syslog_config *cfg,
			 struct persistent_config *persistent)
{
	CAST(afunix_source, self, c);

	if ((self->super.flags & AFSOCKET_KEEP_ALIVE) && persistent) {
		CAST(unix_address_info, unix_addr, self->super.bind_addr);
		struct persistent_info *p;

		p = make_persistent_info
			(c_format("afunix_%z_%S", (self->super.flags & AFSOCKET_STREAM) ? "stream" : "dgram", unix_addr->path),
			 &self->super.connections->super, 
			 NULL);
		add_persistent_info(persistent, p);
	}
	else {
		FOR_OBJECT_QUEUE(self->super.connections, n) {
			CAST_SUBTYPE(log_handler, p, n);

			LOG_HANDLER_DESTROY(p, cfg, NULL);
		}
	}
	if (self->listen) {
		closekill_fd(&self->listen->super, 0);
		self->listen = NULL;
	}
}

struct log_source_driver *
make_afunix_source(struct address_info *bind_addr, UINT32 flags)
{
        NEW(afunix_source, self);

        self->super.super.super.super.init = do_init_afunix_source;
	self->super.super.super.super.destroy = do_destroy_afunix_source;
	self->super.bind_addr = bind_addr;
        self->super.flags = flags;
	self->super.open_connection = do_open_unix_connection;
	self->super.connections = make_object_queue();
	self->super.max_connections = 100;
	self->perm = 0666;

	
        return &self->super.super;
}


/* CLASS:
   (class
     (name afunix_dest)
     (super afsocket_dest)
     (vars
       (dest object io_fd)
       (dest_buf object abstract_buffer)
       (template_output string)
       (template_escape . int)
       (cfg object syslog_config)))
*/

void
afunix_dest_set_template(struct log_dest_driver *c, char *t)
{
	CAST(afunix_dest, self, c);
 
	self->template_output = c_format("%z", t);
}

void
afunix_dest_set_template_escape(struct log_dest_driver *c, int enable)
{
	CAST(afunix_dest, self, c);

	self->template_escape = enable;
}

static void
do_handle_afunix_dest(struct log_handler *c, struct log_info *msg)
{
	CAST(afunix_dest, self, c);

	if (self->dest_buf) {
		struct ol_string *msg_line;

		if (self->template_output) {
			msg_line = c_format("%fS",
					    expand_macros(
						    self->cfg,
						    self->template_output,
						    self->template_escape, msg));
		} else {
                	msg_line = c_format("<%i>%S %S %S\n",
					    msg->pri,
					    msg->date, msg->host, msg->msg);
		}                
		A_WRITE_STRING(&self->dest_buf->super, msg_line);
	}

	log_info_free(msg);
}

static int do_init_afunix_dest(struct log_handler *c, 
			       struct syslog_config *cfg,
			       struct persistent_config *persistent)
{
	CAST(afunix_dest, self, c);
	struct connect_fd *conn_fd;
	int fd;

	self->cfg = cfg;
	if (self->super.super.log_fifo_size == -1)
		self->super.super.log_fifo_size = LIBOL_MIN(cfg->log_fifo_size, 1000);
	
	fd = io_open_socket(self->super.dest_addr->family, 
			    (self->super.flags & AFSOCKET_DGRAM) 
			    ? SOCK_DGRAM : SOCK_STREAM, 
			    0,
			    self->super.bind_addr);
	
	if (fd < 0) {
		return ST_FAIL | ST_QUIT;
	}
	if (!self->dest_buf)
		self->dest_buf = make_pkt_buffer_ext(0, self->super.super.log_fifo_size, !!(self->super.flags & AFSOCKET_DGRAM));

	conn_fd = io_connect(cfg->backend, 
			     fd, 
			     self->super.dest_addr, 
			     make_afsocket_dest_connected(cfg->backend, &self->super));

	if (conn_fd) {
		REMEMBER_RESOURCE(cfg->resources, &conn_fd->super.super);

		return ST_OK | ST_GOON;
	}
	else {
		CAST(unix_address_info, unix_addr, self->super.dest_addr);

		werror("Error connecting to AF_UNIX(%S): %z\n", unix_addr->path, strerror(errno));
		close(fd);
	}
	return ST_FAIL | ST_QUIT;
}

static int
do_afunix_dest_connected(struct afsocket_dest *c,
			 struct io_fd *server)
{
	CAST(afunix_dest, self, c);
	struct resource_node *n;
	CAST(unix_address_info, unix_addr, self->super.dest_addr);

	if (!self->cfg->living) {
		werror("Connection established for an old configuration, dropping...\n");
		return ST_CLOSE | ST_OK;
	}
	if (self->dest)
		self->dest->buffer = NULL;
	if (server) {
		n = REMEMBER_RESOURCE(self->cfg->resources, 
				      &server->super.super);
		self->dest = io_write(server, 
				      self->dest_buf,
				      make_driver_reinit_on_close
				      (&self->super.super.super, n, 
				       self->cfg->time_reopen, self->cfg,
				       c_format("AF_UNIX(%S)", unix_addr->path)));
	}
	else {

		werror("Error connecting to remote AF_UNIX socket (%S), reattempting in %i seconds\n", unix_addr->path, self->cfg->time_reopen);
		self->dest = NULL;
		io_callout(self->cfg->backend, self->cfg->time_reopen, 
			   make_driver_reinit
			   (&self->super.super.super, self->cfg));
	}
	return ST_OK | ST_GOON;

}

struct log_dest_driver *make_afunix_dest(struct address_info *dest_addr, UINT32 flags)
{
	NEW(afunix_dest, self);
	
	self->super.super.super.init = do_init_afunix_dest;
	self->super.super.super.handler = do_handle_afunix_dest;
	self->super.super.log_fifo_size = -1;
	self->super.connected = do_afunix_dest_connected;
	self->super.flags = flags;
	self->super.dest_addr = dest_addr;
	return &self->super.super;
}

