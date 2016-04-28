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
 * $Id: afinet.c,v 1.25.4.10 2007/02/02 08:58:30 bazsi Exp $
 *
 ***************************************************************************/

#include "afsocket.h"
#include "format.h"
#include "cfgfile.h"
#include "pkt_buffer.h"

#include <errno.h>
#include <string.h>
#include <assert.h>

#if ENABLE_SPOOF_SOURCE
#include <libnet.h>
#else
typedef void libnet_t;
#endif

#if ENABLE_TCP_WRAPPER
#include <tcpd.h>
int allow_severity = 0;
int deny_severity = 0;
#endif

#include "afinet.c.x"
#include "macros.h"

void inet_address_setip(struct address_info **a, const char *ip)
{
	CAST(inet_address_info, addr, *a);
	if (!addr) {
		*a = &make_inet_address_c(ip, NULL)->super;
	}
	else {
		ol_string_free(addr->ip);
		addr->ip = c_format("%z", ip);
	}
}

void inet_address_setport(struct address_info **a, UINT32 port, 
			  const char *service, const char *proto)
{
	CAST(inet_address_info, addr, *a);
	struct servent *srv;

	if (!port && service && proto) {
		if ((srv = getservbyname(service, proto))) {
			port=srv->s_port;
		} 
		else {
			werror("Error finding port for service %z.  Falling back to default\n", service);
			return;
		}
	} 
	if (!addr) {
		*a = &make_inet_address(NULL, port)->super;
	}
	else {
		addr->port = port;
	}
}

/* af_inet source connection */
static int 
do_init_afinet_source_connection(struct log_handler *c, 
				 struct syslog_config *cfg,
				 struct persistent_config *persistent)
{
	CAST(afsocket_source_connection, self, c);
	if (self->client_addr) {
		CAST(inet_address_info, inet, self->client_addr);

		/* SOCK_STREAM */
		notice("AF_INET client connected from %S, port %i\n", 
		       inet->ip, inet->port);
		io_read(self->client, 
			make_log_reader(0, NULL, cfg->log_msg_size, 0, cfg->check_hostname ? LF_CHECK_HOSTNAME : 0, cfg->bad_hostname, c), 
			make_afsocket_source_close_callback(self));
	}
	else {
		/* SOCK_DGRAM */
		io_read(self->client, 
			make_log_reader(1, NULL, cfg->log_msg_size, 0, cfg->check_hostname ? LF_CHECK_HOSTNAME : 0, cfg->bad_hostname, c), 
			make_afsocket_source_close_callback(self));
	}
	
	return ST_OK | ST_GOON;
}

static void 
do_handle_afinet_line(struct log_handler *c, struct log_info *msg)
{
	CAST(afsocket_source_connection, self, c);

	if (!msg->saddr) {
		msg->saddr = self->client_addr;
	}
	HANDLE_LOG(self->super.next, msg);
}

static int
do_close_afinet_connection(struct afsocket_source_connection *c,
                           int close_reason)
{
	assert(c->owner);
	if (c->client_addr) {
		CAST(inet_address_info, inet, c->client_addr);

		notice("AF_INET client dropped connection from %S, port %i\n", 
		       inet->ip, inet->port);
	}
	object_queue_remove(c->queue_node);
	c->owner->num_connections--;
	return ST_OK | ST_GOON;
}

static void
do_destroy_afinet_connection(struct log_handler *c,
			     struct syslog_config *cfg,
			     struct persistent_config *persistent)
{
	CAST(afsocket_source_connection, self, c);

	closekill_fd(&self->client->super, 0);
}

struct afsocket_source_connection *
make_afinet_source_connection(struct io_fd *client,
			      struct address_info *client_addr, 
			      struct afsocket_source *owner,
			      struct log_handler *next)
{
	NEW(afsocket_source_connection, self);

	self->super.super.init = do_init_afinet_source_connection;
	self->super.super.handler = do_handle_afinet_line;
	self->super.super.destroy = do_destroy_afinet_connection;
	self->super.next = next;
	self->owner = owner;
	self->client = client;
	self->client_addr = client_addr;
	self->close = do_close_afinet_connection;
	return self;
}

/* afinet_source */
/* CLASS:
   (class
     (name afinet_source)
     (super afsocket_source)
     (vars
       (listen object listen_fd)
       (so_rcvbuf . UINT32)
       (cfg object syslog_config)))
*/

void afinet_src_set_localip(struct log_source_driver *c, const char *ip)
{
	CAST_SUBTYPE(afsocket_source, source, c);

	inet_address_setip(&source->bind_addr, ip);
}

void afinet_src_set_localport(struct log_source_driver *c, UINT32 port, 
			      const char *service, const char *proto)
{
	CAST_SUBTYPE(afsocket_source, source, c);

	inet_address_setport(&source->bind_addr, port, service, proto);
}

void afinet_src_set_auth(struct log_source_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_source, l, c);

	l->flags = set_flags(l->flags, AFINET_AUTH_MASK, AFINET_AUTH_SHIFT, v);
	werror("Warning: network authentication not implemented yet.\n");
}

void afinet_src_set_mac(struct log_source_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_source, l, c);

	l->flags = set_flags(l->flags, AFINET_MAC_MASK, AFINET_MAC_SHIFT, v);
	werror("Warning: network mac protection not implemented yet.\n");
}

void afinet_src_set_encrypt(struct log_source_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_source, l, c);

	l->flags = set_flags(l->flags, AFINET_ENCRYPT_MASK, AFINET_ENCRYPT_SHIFT, v);
	werror("Warning: network encryption not implemented yet.\n");
}

void afinet_src_set_tcp_keepalive(struct log_source_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_source, l, c);

	l->flags = set_flags(l->flags, AFINET_TCP_KEEPALIVE_MASK, AFINET_TCP_KEEPALIVE_SHIFT, v);
}

void afinet_src_set_so_rcvbuf(struct log_source_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afinet_source, l, c);

	l->so_rcvbuf = v;
}

static int 
do_open_afinet_connection(struct afsocket_source *c,
			  struct io_fd *client,
			  struct address_info *client_addr)
{
	CAST(afinet_source, self, c);
	struct afsocket_source_connection *conn;
	UINT32 res;
#if ENABLE_TCP_WRAPPER
	{
		struct request_info req;

		request_init(&req, RQ_DAEMON, "syslog-ng", RQ_FILE, client->super.fd, 0);
		fromhost(&req);
		if (hosts_access(&req) == 0) {
			CAST(inet_address_info, inet_addr, client_addr);

			notice("connection from %S:%i refused by hosts_access()\n", inet_addr->ip, inet_addr->port);
			close_fd(&client->super, 0);
			return ST_OK | ST_GOON;
		}
	}
  
#endif
	if (c->num_connections >= c->max_connections) {
		CAST(inet_address_info, inet_addr, client_addr);

		notice("Error accepting AF_INET connection from: %S:%i, opened connections: %i, max: %i\n", inet_addr->ip, inet_addr->port, c->num_connections, c->max_connections);
		close_fd(&client->super, 0);
		return ST_OK | ST_GOON;
	}
	else {
		if (self->super.flags & AFINET_TCP_KEEPALIVE) {
			int on = 1;
			setsockopt(client->super.fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
		}
		conn = make_afinet_source_connection(client, client_addr, c, self->super.super.super.next);
		res = LOG_HANDLER_INIT(conn, self->cfg, NULL);
		if ((res & ST_FAIL) == 0) {
			conn->queue_node = 
				object_queue_add_tail(self->super.connections, &conn->super.super.super);
			c->num_connections++;
		}
		return res;
	}


}

static int 
do_init_afinet_source(struct log_handler *c, 
		      struct syslog_config *cfg,
		      struct persistent_config *persistent)
{
	CAST(afinet_source, self, c);
	int fd;

	self->cfg = cfg;

	if (persistent && (self->super.flags & AFSOCKET_KEEP_ALIVE)) {
		CAST(inet_address_info, inet_addr, self->super.bind_addr);
		struct ol_string *persistent_name;
		struct persistent_info *p_info;

		persistent_name = c_format("afinet_%z_%S_%i", 
					   self->super.flags & AFSOCKET_DGRAM ? "dgram" : "stream",
					   inet_addr->ip, inet_addr->port);
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

	fd = io_open_socket(self->super.bind_addr->family, 
			    (self->super.flags & AFSOCKET_DGRAM)
			    ? SOCK_DGRAM : SOCK_STREAM,
			    (self->super.flags & AFSOCKET_DGRAM) 
			    ? IPPROTO_UDP : IPPROTO_TCP, 
			    self->super.bind_addr);
	if (fd < 0) {
		return ST_QUIT | ST_FAIL;
	}
	switch (self->super.flags & 0x0003) {
	case AFSOCKET_STREAM: {
		if (!(self->listen = 
		     io_listen(cfg->backend, fd,
			       make_afsocket_accept_callback(
				       cfg->backend, 
				       &self->super))))
			return ST_FAIL | ST_QUIT;
		break;
	}
			
	case AFSOCKET_DGRAM: {
		struct afsocket_source_connection *conn;
		struct io_fd *client = make_io_fd(cfg->backend, fd, NULL);
		int res;

		if (self->so_rcvbuf)
			setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &self->so_rcvbuf, sizeof(self->so_rcvbuf));
		self->super.num_connections = 1;
		conn = make_afinet_source_connection(client, NULL, &self->super, self->super.super.super.next);
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
do_destroy_afinet_source(struct log_handler *c,
			 struct syslog_config *cfg,
			 struct persistent_config *persistent)
{
	CAST(afinet_source, self, c);

	if ((self->super.flags & AFSOCKET_KEEP_ALIVE) && persistent) {
		CAST(inet_address_info, inet_addr, self->super.bind_addr);
		struct persistent_info *p;

		p = make_persistent_info
			(c_format("afinet_%z_%S_%i", 
				  self->super.flags & AFSOCKET_DGRAM ? "dgram" : "stream",
				  inet_addr->ip, inet_addr->port),
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

struct log_source_driver *make_afinet_source(struct address_info *bind_addr, UINT32 flags)
{
        NEW(afinet_source, self);

        self->super.super.super.super.init = do_init_afinet_source;        
        self->super.super.super.super.destroy = do_destroy_afinet_source;
	self->super.open_connection = do_open_afinet_connection;
	self->super.bind_addr = bind_addr;
        self->super.flags = flags;
	self->super.connections = make_object_queue();
	self->super.max_connections = 10;
        return &self->super.super;
}

/* CLASS:
   (class
     (name afinet_dest)
     (super afsocket_dest)
     (vars
       (dest object io_fd)
       (spoof_source simple int)
       (lnet_ctx simple "libnet_t *")
       (dest_buf object abstract_buffer)
       (sync_freq simple UINT32)
       (conn_fd object connect_fd)
       (template_output string)
       (template_escape . int)
       (cfg object syslog_config)))
*/
/* af_inet destination */
void afinet_dest_set_auth(struct log_dest_driver *c, UINT32 v)
{	
	CAST_SUBTYPE(afsocket_dest, l, c);
	l->flags = set_flags(l->flags, AFINET_AUTH_MASK, AFINET_AUTH_SHIFT, v);
}

void afinet_dest_set_mac(struct log_dest_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_dest, l, c);
	l->flags = set_flags(l->flags, AFINET_MAC_MASK, AFINET_MAC_SHIFT, v);
}

void afinet_dest_set_encrypt(struct log_dest_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_dest, l, c);
	l->flags = set_flags(l->flags, AFINET_ENCRYPT_MASK, AFINET_ENCRYPT_SHIFT, v);
}

void afinet_dest_set_tcp_keepalive(struct log_dest_driver *c, UINT32 v)
{
	CAST_SUBTYPE(afsocket_dest, l, c);

	l->flags = set_flags(l->flags, AFINET_TCP_KEEPALIVE_MASK, AFINET_TCP_KEEPALIVE_SHIFT, v);
}

void afinet_dest_set_localip(struct log_dest_driver *c, const char *ip)
{
	CAST_SUBTYPE(afsocket_dest, dest, c);

	inet_address_setip(&dest->bind_addr, ip);
}

void afinet_dest_set_localport(struct log_dest_driver *c, UINT32 port, 
			       const char *service, const char *proto)
{
	CAST_SUBTYPE(afsocket_dest, dest, c);

	inet_address_setport(&dest->bind_addr, port, service, proto);
}

void afinet_dest_set_template(struct log_dest_driver *c, char *t)
{
	CAST(afinet_dest, self, c);
 
	self->template_output = c_format("%z", t);
}

void afinet_dest_set_template_escape(struct log_dest_driver *c, int enable)
{
	CAST(afinet_dest, self, c);

	self->template_escape = enable;
}

void afinet_dest_set_destport(struct log_dest_driver *c, UINT32 port, 
			      const char *service, const char *proto)
{
	CAST_SUBTYPE(afsocket_dest, dest, c);

	inet_address_setport(&dest->dest_addr, port, service, proto);
}

void afinet_dest_set_syncfreq(struct log_dest_driver *c, UINT32 syncfreq)
{
 	CAST_SUBTYPE(afinet_dest, dest, c);
	dest->sync_freq = syncfreq;
}

#if ENABLE_SPOOF_SOURCE
void afinet_dest_set_spoof_source(struct log_dest_driver *c, int enable)
{
 	CAST_SUBTYPE(afinet_dest, dest, c);
	dest->spoof_source = enable;
}
#else
void afinet_dest_set_spoof_source(struct log_dest_driver *c, int enable)
{
	werror("spoof-source support not compiled in.");
}

#endif

static int do_init_afinet_dest(struct log_handler *c, 
			       struct syslog_config *cfg,
			       struct persistent_config *persistent)
{
	CAST(afinet_dest, self, c);
	int fd;

	self->cfg = cfg;
	if (self->super.super.log_fifo_size == -1)
		self->super.super.log_fifo_size = LIBOL_MIN(1000, cfg->log_fifo_size);
	
	fd = io_open_socket(self->super.dest_addr->family, 
			    (self->super.flags & AFSOCKET_DGRAM) 
			    ? SOCK_DGRAM : SOCK_STREAM, 
			    (self->super.flags & AFSOCKET_DGRAM)
			    ? IPPROTO_UDP : IPPROTO_TCP, 
			    self->super.bind_addr);
	
	if (fd < 0) {
		return ST_FAIL | ST_QUIT;
	}
	if (!self->dest_buf) {
		self->dest_buf = make_pkt_buffer_ext(self->sync_freq, 
                				     LIBOL_MAX(self->super.super.log_fifo_size + 1, 
                                                         self->sync_freq),
                                                     !!(self->super.flags & AFSOCKET_DGRAM));
        }
#if ENABLE_SPOOF_SOURCE
	if (self->spoof_source && !self->lnet_ctx) {
		UINT8 error[LIBNET_ERRBUF_SIZE];
		self->lnet_ctx = libnet_init(LIBNET_RAW4, NULL, error);
		if (!self->lnet_ctx) {
			werror("Error initializing raw socket, spoof-source support disabled. (%z)\n", error);
		}
	}
#endif
	self->conn_fd = io_connect(cfg->backend, 
		 	           fd, 
			           self->super.dest_addr, 
 		                   make_afsocket_dest_connected(cfg->backend, &self->super));

	if (self->conn_fd) {
		return ST_OK | ST_GOON;
	}
	else {
		io_callout(self->cfg->backend, 
			   self->cfg->time_reopen, 
			   make_driver_reinit(&self->super.super.super, self->cfg));
		return ST_OK | ST_GOON;
	}
	return ST_FAIL | ST_QUIT;
}

static int do_afinet_dest_connected(struct afsocket_dest *c,
	                            struct io_fd *server)
{
	CAST(afinet_dest, self, c);
	struct resource_node *n;
	CAST(inet_address_info, inet_addr, self->super.dest_addr);

	if (!self->cfg->living) {
		werror("Connection established for an old configuration, dropping...\n");
		return ST_CLOSE | ST_OK;
	}
	self->conn_fd = NULL;
	if (self->dest)
		self->dest->buffer = NULL;
	if (server) {
		if (self->super.flags & AFINET_TCP_KEEPALIVE) {
			int on = 1;
			setsockopt(server->super.fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
		}
		n = REMEMBER_RESOURCE(self->cfg->resources, 
				      &server->super.super);
		self->dest_buf->closed = 0;
		self->dest = io_write(server, 
				      self->dest_buf,
				      make_driver_reinit_on_close
				      (&self->super.super.super, n, 
				       self->cfg->time_reopen, 
				       self->cfg,
				       c_format("AF_INET(%S:%i)", inet_addr->ip, inet_addr->port)));
	}
	else {

		werror("Error connecting to remote host AF_INET(%S:%i), reattempting in %i seconds\n", inet_addr->ip, inet_addr->port, self->cfg->time_reopen);
		self->dest = NULL;
		io_callout(self->cfg->backend, 
			   self->cfg->time_reopen, 
			   make_driver_reinit(&self->super.super.super, self->cfg));
	}
	return ST_OK | ST_GOON;

}

void do_handle_afinet_dest(struct log_handler *c, struct log_info *msg)
{
	CAST(afinet_dest, self, c);

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
#if ENABLE_SPOOF_SOURCE
		if ((self->super.flags & AFSOCKET_DGRAM) &&
		    self->spoof_source && 
		    msg->saddr && msg->saddr->family == AF_INET && 
		    self->super.dest_addr->family == AF_INET && 
		    self->lnet_ctx) {
		        struct sockaddr_in src_addr, dst_addr;
		        libnet_ptag_t ip, udp;
			
			ADDRESS2SOCKADDR(msg->saddr, sizeof(src_addr), (struct sockaddr *) &src_addr);
			ADDRESS2SOCKADDR(self->super.dest_addr, sizeof(dst_addr), (struct sockaddr *) &dst_addr);
			
			libnet_clear_packet(self->lnet_ctx);
			
			udp = libnet_build_udp(ntohs(src_addr.sin_port), 
			                       ntohs(dst_addr.sin_port), 
			                       LIBNET_UDP_H + msg_line->length,
			                       0, 
			                       msg_line->data, 
			                       msg_line->length, 
			                       self->lnet_ctx,
			                       0);
			if (udp == -1) {
				goto fallback_socket;
				
			}
			
			ip = libnet_build_ipv4(LIBNET_IPV4_H + msg_line->length + LIBNET_UDP_H,
        	                               IPTOS_LOWDELAY,         /* IP tos */
                	                       0,                      /* IP ID */
                        	               0,                      /* frag stuff */
                                	       64,                     /* TTL */
                                               IPPROTO_UDP,            /* transport protocol */
					       0,
	                                       src_addr.sin_addr.s_addr,/* source IP */
        	                               dst_addr.sin_addr.s_addr,/* destination IP */
                	                       NULL,                   /* payload (none) */
                        	               0,                      /* payload length */
                                	       self->lnet_ctx, 
                                	       0);
			if (ip == -1) {
				goto fallback_socket;
			}
			
			if (libnet_write(self->lnet_ctx) < 0) {
				werror("Error sending raw frame, error: %z", libnet_geterror(self->lnet_ctx));
			}
			ol_string_free(msg_line);
		}
		else {
		fallback_socket:
			A_WRITE_STRING(&self->dest_buf->super, msg_line);
		}
#else
		A_WRITE_STRING(&self->dest_buf->super, msg_line);
#endif
	}
	log_info_free(msg);
}

void do_destroy_afinet_dest(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(afinet_dest, self, c);
	if (self->conn_fd) {
		/* KILL_RESOURCE(&self->conn_fd->super.super); */
		closekill_fd(&self->conn_fd->super, 0);
		self->conn_fd = NULL;
	}
#if ENABLE_SPOOF_SOURCE
	if (self->lnet_ctx) {
		libnet_destroy(self->lnet_ctx);
		self->lnet_ctx = NULL;
	}
#endif
}

struct log_dest_driver *make_afinet_dest(struct address_info *dest_addr, UINT32 flags)
{
	NEW(afinet_dest, self);
	
	self->super.super.super.init = do_init_afinet_dest;
	self->super.super.super.handler = do_handle_afinet_dest;
	self->super.super.super.destroy = do_destroy_afinet_dest;
	self->super.super.log_fifo_size = -1;
	self->super.dest_addr = dest_addr;
	self->super.flags = flags;
	self->super.connected = do_afinet_dest_connected;
	return &self->super.super;
}
