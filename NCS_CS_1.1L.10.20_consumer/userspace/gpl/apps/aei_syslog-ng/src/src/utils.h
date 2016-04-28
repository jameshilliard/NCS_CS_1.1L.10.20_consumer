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
 * $Id: utils.h,v 1.9 2001/02/24 12:38:03 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __UTILS_H_INCLUDED
#define __UTILS_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utmp.h>

char *getshorthostname(char *buf, size_t bufsize);

#ifndef HAVE_INET_ATON
int inet_aton(const char *cp, struct in_addr *addr);
#endif

#ifndef HAVE_GETUTENT
struct utmp *getutent(void);
void endutent(void);
#endif

#define get_flags(flags, mask, shift) ((flags & mask) >> shift)
#define set_flags(flags, mask, shift, value) ((flags & ~mask) | value << shift)

#endif
