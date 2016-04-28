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
 * $Id: syslog-ng.h,v 1.22.4.1 2006/06/12 07:50:47 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __SYSLOG_NG_H
#define __SYSLOG_NG_H

#include "config.h"

#include "objects.h"

#if WITH_DEBUG
#define PATH_SYSLOG_NG_CONF     "syslog-ng.conf"
#define PATH_PIDFILE            "syslog-ng.pid"
#else
#define PATH_SYSLOG_NG_CONF     PATH_SYSCONFDIR "/syslog-ng.conf"
#define PATH_PIDFILE            "/var/run/syslog-ng.pid"
#endif

#define ST_QUIT                 16

/* amount of time before reopening is attempted */

#ifndef HAVE_STRCASECMP
extern int strcasecmp(const char *s1, const char *s2);
#endif

#if !HAVE_O_LARGEFILE
#undef O_LARGEFILE
#define O_LARGEFILE 0
#endif

#endif

