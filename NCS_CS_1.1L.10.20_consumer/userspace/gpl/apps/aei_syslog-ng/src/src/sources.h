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
 * $Id: sources.h,v 1.17 2003/01/08 09:31:37 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __SOURCES_H
#define __SOURCES_H

#include "syslog-ng.h"
#include "read_line.h"

#include "log.h"
#include "list.h"

#define CLASS_DECLARE
#include "sources.h.x"
#undef CLASS_DECLARE

/* CLASS:
     (class
       (name log_source_driver)
       (super log_handler_pipe)
       (vars
         (next_driver object log_source_driver)))
*/

/* CLASS:
     (class
       (name log_source_group)
       (super log_handler_pipe)
       (vars
         (next_source_group object log_source_group) ; for linking into syslog_config
         (used simple int)			 ; true, when referenced
	 (chain_hostnames simple int)
	 (use_fqdn simple int)
	 (use_dns simple int)
	 (cache simple "struct nscache *")
	 (keep_hostname simple int)
         (drivers object log_source_driver)
         (name string)))
*/

struct read_handler *
make_log_reader(UINT32 dgram, 
		UINT8 *prefix,
		UINT32 max_log_line,
		UINT32 pad_size,
		UINT32 msg_flags,
		UINT8 *bad_hostname,
		struct log_handler *next);

struct log_source_group *make_source_group(const char *name, struct log_source_driver *drvs);

/* void set_source_drivers(struct log_source_group *src, struct log_source_driver *drvs); */

#define append_source_driver(item, drv) ((struct log_source_driver *) item)->next_driver = (drv)
#define append_source_group(item, grp) ((struct log_source_group *) item)->next_source_group = (grp)

#endif
