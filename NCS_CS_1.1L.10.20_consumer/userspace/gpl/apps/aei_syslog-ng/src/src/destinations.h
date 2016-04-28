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
 * $Id: destinations.h,v 1.13 2002/10/18 12:31:08 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __DESTINATIONS_H
#define __DESTINATIONS_H

#include "syslog-ng.h"

#include "log.h"

#define CLASS_DECLARE
#include "destinations.h.x"
#undef CLASS_DECLARE

/* CLASS:
     (class
       (name log_dest_driver)
       (super log_handler)
       (vars
         (log_fifo_size simple int)
         (next_driver object log_dest_driver)))
*/


#if 0
void init_dest_driver(struct log_dest_driver *drv);
#endif

#define append_dest_driver(item, drv) ((struct log_dest_driver *) item)->next_driver = (drv)

/* CLASS:
     (class
       (name log_dest_group)
       (super log_handler)
       (vars
         (next_dest_group object log_dest_group) ; for linking in syslog_config
         (used simple int)                   ; true when referenced
         (name string)
         (drivers object log_dest_driver)))
*/

/* #define set_dest_drivers(grp, drvs) (grp)->drivers = (drvs) */

#define append_dest_group(item, grp) ((struct log_dest_group *) item)->next_dest_group = (grp)

struct log_dest_group *make_dest_group(const char *name, struct log_dest_driver *drvs);

#endif
