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
 * $Id: afsocket.c,v 1.22 2001/07/08 17:04:21 bazsi Exp $
 *
 ***************************************************************************/

#include "afsocket.h"
#include "format.h"
#include "cfgfile.h"
#include "pkt_buffer.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define CLASS_DEFINE
#include "afsocket.h.x"
#undef CLASS_DEFINE

#include "afsocket.c.x"

/* CLASS:
     (class
       (name afsocket_source_close_callback)
       (super close_callback)
       (vars
         (connection object afsocket_source_connection)))
*/

static int do_afsocket_src_close(struct close_callback *c, int close_reason)
{
	CAST(afsocket_source_close_callback, self, c);

	return CLOSE_CONNECTION(self->connection, close_reason);
}

struct close_callback *
make_afsocket_source_close_callback(struct afsocket_source_connection *connection)
{
	NEW(afsocket_source_close_callback, self);
	self->super.f = do_afsocket_src_close;
	self->connection = connection;
	return &self->super;
}

/* accept connection callback */
/* CLASS:
     (class
       (name afsocket_accept_callback)
       (super fd_listen_callback)
       (vars
         (backend object io_backend)
         (source object afsocket_source)))
*/

static int 
do_accept_socket_connection(struct fd_listen_callback *c, 
			    int fd, 
			    struct address_info *a)
{
	CAST(afsocket_accept_callback, self, c);
	return SOURCE_OPEN_CONNECTION(self->source, 
				      make_io_fd(self->backend, fd, NULL),
				      a);
}

struct fd_listen_callback *
make_afsocket_accept_callback(struct io_backend *backend, 
			      struct afsocket_source *source)
{
	NEW(afsocket_accept_callback, self);
	
	self->super.f = do_accept_socket_connection;
	self->backend = backend;
	self->source = source;
	return &self->super;
}

/* afsocket_source */
void afsocket_src_set_keep_alive(struct log_source_driver *c, int enable)
{
	CAST_SUBTYPE(afsocket_source, self, c);

	if (enable)
		self->flags |= AFSOCKET_KEEP_ALIVE;
	else
		self->flags &= ~AFSOCKET_KEEP_ALIVE;
}

void afsocket_src_set_max_connections(struct log_source_driver *c, UINT32 max_connections)
{
	CAST_SUBTYPE(afsocket_source, self, c);

	self->max_connections = max_connections;
}

/* CLASS:
     (class
       (name afsocket_dest_connected)
       (super fd_callback)
       (vars
         (backend object io_backend)
         (dest object afsocket_dest)))

*/

static int do_afsocket_dest_connected(struct fd_callback **r, int fd)
{
	CAST(afsocket_dest_connected, self, *r);
	
	return DESTINATION_CONNECTED(self->dest, fd != -1
		? make_io_fd(self->backend, fd, NULL) : NULL);

}

struct fd_callback *
make_afsocket_dest_connected(struct io_backend *backend, 
			     struct afsocket_dest *dest)
{
	NEW(afsocket_dest_connected, self);
	self->super.f = do_afsocket_dest_connected;
	self->dest = dest;
	self->backend = backend;
	return &self->super;
}

