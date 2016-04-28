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
 * $Id: afuser.h,v 1.4 2003/01/31 14:26:48 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __AFUSER_H_INCLUDED
#define __AFUSER_H_INCLUDED

#include "syslog-ng.h"

struct log_dest_driver *make_afuser_dest(const char *username);
void afuser_dest_set_template(struct log_dest_driver *c, char *t);
void afuser_dest_set_template_escape(struct log_dest_driver *c, int enable);

#endif
