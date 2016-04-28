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
 * $Id: afstreams.h,v 1.4 2001/02/24 12:38:02 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __AFSTREAMS_H_INCLUDED
#define __AFSTREAMS_H_INCLUDED

#include "syslog-ng.h"
#include "log.h"
#include "sources.h"

void afstreams_set_sundoor(struct log_source_driver *c, const char *door);
struct log_source_driver *make_afstreams_source(const char *name);

#endif
