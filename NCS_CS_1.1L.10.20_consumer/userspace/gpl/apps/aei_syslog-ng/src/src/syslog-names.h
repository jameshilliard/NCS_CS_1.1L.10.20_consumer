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
 * $Id: syslog-names.h,v 1.7 2002/10/18 12:31:08 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __SYSLOG_NAMES_H_INCLUDED
#define __SYSLOG_NAMES_H_INCLUDED

#include "syslog-ng.h"
#include <syslog.h>

struct sl_name {
	char *name;
	unsigned int value;
};

extern struct sl_name sl_levels[];
extern struct sl_name sl_facilities[];

/* returns an index where this name is found */
unsigned int syslog_lookup_name(const char *name, struct sl_name names[]);
char *syslog_lookup_value(unsigned int value, struct sl_name names[]);
UINT32 syslog_make_range(UINT32 r1, UINT32 r2);

#define syslog_lookup_level(name) syslog_lookup_name(name, sl_levels)
#define syslog_lookup_facility(name) syslog_lookup_name(name, sl_facilities)

#endif
