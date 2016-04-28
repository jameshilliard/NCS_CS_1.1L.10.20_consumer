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
 * $Id: afsocket.h,v 1.16.4.3 2006/08/22 07:45:21 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __AFSOCKET_H_INCLUDED
#define __AFSOCKET_H_INCLUDED

#include "syslog-ng.h"
#include "io.h"
#include "sources.h"
#include "destinations.h"
#include "utils.h"
#include "log.h"

#define CLASS_DECLARE
#include "afsocket.h.x"
#undef CLASS_DECLARE

#define AFSOCKET_DGRAM       0x0001
#define AFSOCKET_STREAM      0x0002
#define AFSOCKET_KEEP_ALIVE  0x0100

#define AFINET_YES            3
#define AFINET_NO             0

#define AFINET_REQUIRED       2
#define AFINET_ALLOW          1
#define AFINET_DENY           0

#define AFINET_AUTH_MASK      0x000C
#define AFINET_AUTH_SHIFT     2

#define AFINET_MAC_MASK       0x0030
#define AFINET_MAC_SHIFT      4

#define AFINET_ENCRYPT_MASK   0x00C0
#define AFINET_ENCRYPT_SHIFT  6

#define AFINET_TCP_KEEPALIVE 0x0100
#define AFINET_TCP_KEEPALIVE_MASK 0x0100
#define AFINET_TCP_KEEPALIVE_SHIFT 8


/* CLASS:
     (class
       (name afsocket_source_connection)
       (super log_handler_pipe)
       (vars
         (owner object afsocket_source)
	 (queue_node simple "struct object_queue_node *")
         (client object io_fd)
         (client_addr object address_info)
	 (close method int "int close_reason")))
*/

#define CLOSE_CONNECTION(c, r) ((c)->close(c, r))

struct close_callback *
make_afsocket_source_close_callback(struct afsocket_source_connection *);

struct fd_listen_callback *
make_afsocket_accept_callback(struct io_backend *backend, 
			      struct afsocket_source *source);


/* CLASS:
     (class
       (name afsocket_source)
       (super log_source_driver)
       (vars
	 (flags simple UINT32)
	 (bind_addr object address_info)
	 (max_connections simple UINT32)
	 (num_connections simple UINT32)
	 (connections object object_queue)
	 (open_connection method int
	                         "struct io_fd *" 
				 "struct address_info *a")))
*/

#define SOURCE_OPEN_CONNECTION(c, f, a) ((c)->open_connection(c, f, a))


/* CLASS:
     (class
       (name afsocket_dest)
       (super log_dest_driver)
       (vars
         (flags simple UINT32)
         (bind_addr object address_info)
         (dest_addr object address_info)
	 (connected method int "struct io_fd *fd")))
*/

#define DESTINATION_CONNECTED(c, f) ((c)->connected(c, f))

void do_handle_afsocket_dest(struct log_handler *c, struct log_info *msg);

struct fd_callback *
make_afsocket_dest_connected(struct io_backend *backend, 
			     struct afsocket_dest *dest);


void afsocket_src_set_keep_alive(struct log_source_driver *c, int enable);
void afsocket_src_set_max_connections(struct log_source_driver *c, UINT32 max_connections);

void afunix_set_owner(struct log_source_driver *c, const char *owner);
void afunix_set_group(struct log_source_driver *c, const char *group);
void afunix_set_perm(struct log_source_driver *c, int perm);

void afinet_src_set_localport(struct log_source_driver *c, UINT32 port, const char *service,const char *proto);
void afinet_src_set_localip(struct log_source_driver *c, const char *ip);
void afinet_src_set_auth(struct log_source_driver *c, UINT32 v);
void afinet_src_set_mac(struct log_source_driver *c, UINT32 v);
void afinet_src_set_encrypt(struct log_source_driver *c, UINT32 v);
void afinet_src_set_tcp_keepalive(struct log_source_driver *c, UINT32 v);
void afinet_src_set_so_rcvbuf(struct log_source_driver *c, UINT32 v);
        
void afinet_dest_set_localip(struct log_dest_driver *c, const char *ip);
void afinet_dest_set_localport(struct log_dest_driver *c, UINT32 port, const char *service,const char *proto);
void afinet_dest_set_destport(struct log_dest_driver *c, UINT32 port, const char *service,const char *proto);
void afinet_dest_set_auth(struct log_dest_driver *c, UINT32 value);
void afinet_dest_set_mac(struct log_dest_driver *c, UINT32 value);
void afinet_dest_set_encrypt(struct log_dest_driver *c, UINT32 value);
void afinet_dest_set_tcp_keepalive(struct log_dest_driver *c, UINT32 v);
void afinet_dest_set_spoof_source(struct log_dest_driver *c, int enable);
void afinet_dest_set_syncfreq(struct log_dest_driver *c, UINT32 value);
void afinet_dest_set_template(struct log_dest_driver *c, char *t);
void afinet_dest_set_template_escape(struct log_dest_driver *c, int enable);

struct log_source_driver *
make_afunix_source(struct address_info *bind_addr, UINT32 flags);

struct log_dest_driver *
make_afunix_dest(struct address_info *dest_addr, UINT32 flags);
void afunix_dest_set_template(struct log_dest_driver *c, char *t);
void afunix_dest_set_template_escape(struct log_dest_driver *c, int enable);

struct log_source_driver *
make_afinet_source(struct address_info *bind_addr, UINT32 flags);

struct log_dest_driver *
make_afinet_dest(struct address_info *dest_addr, UINT32 flags);

#endif

