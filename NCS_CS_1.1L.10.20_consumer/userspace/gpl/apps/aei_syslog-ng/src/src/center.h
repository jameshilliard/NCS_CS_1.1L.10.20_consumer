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
 * $Id: center.h,v 1.10 2001/07/08 17:04:21 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __CENTER_H_INCLUDED
#define __CENTER_H_INCLUDED

#include "log.h"

#define CLASS_DECLARE
#include "center.h.x"
#undef CLASS_DECLARE

/* CLASS:
     (class
       (name log_endpoint_info)
       (vars
         (type simple int)
         (name string)
         (ref object ol_object)
         (next object log_endpoint_info)))
*/

#define EP_SOURCE	1
#define EP_DESTINATION	2
#define EP_FILTER	3

#define append_endpoint_info(ep, nxt) ((struct log_endpoint_info *) ep)->next = (nxt)

struct log_endpoint_info *make_log_endpoint_info(int type, const char *name);

#define LOG_CONN_CATCHALL  (1 << 0)
#define LOG_CONN_FALLBACK  (1 << 1)
#define LOG_CONN_FINAL     (1 << 2)

/* CLASS:
     (class
       (name log_connection)
       (vars
         (flags simple UINT32)
         (sources object log_endpoint_info)
         (filters object log_endpoint_info)
         (destinations object log_endpoint_info)
         (next object log_connection)))
*/

struct log_connection *make_log_connection(struct log_endpoint_info *nodes,
	UINT32 flags);

struct log_handler *make_log_center(struct log_connection *connections);

struct callback *
make_driver_reinit(struct log_handler *handler, struct syslog_config *cfg);

struct close_callback *
make_driver_reinit_on_close(struct log_handler *handler, 
			    struct resource_node *res, 
			    int time_reopen, 
			    struct syslog_config *cfg,
			    struct ol_string *desc);

#endif


