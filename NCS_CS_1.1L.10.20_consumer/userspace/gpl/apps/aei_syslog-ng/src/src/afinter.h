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
 * $Id: afinter.h,v 1.4 2002/04/26 09:43:54 bazsi Exp $
 *
 * Log source for internal messages of syslog-ng
 *
 ***************************************************************************/

#ifndef __AFINTER_H_INCLUDED
#define __AFINTER_H_INCLUDED

#include "sources.h"

void set_error_internal(void);

struct log_source_driver *make_afinter_source(void);

void set_internal_cfg(struct syslog_config *cfg);

#endif
