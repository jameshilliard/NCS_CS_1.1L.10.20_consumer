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
 * $Id: afinter.c,v 1.14 2003/01/31 14:26:48 bazsi Exp $
 *
 * Log source for internal messages of syslog-ng
 *
 ***************************************************************************/

#include "afinter.h"
#include "werror.h"
#include "syslog-names.h"
#include "cfgfile.h"
#include "log.h"

#include <assert.h>

struct syslog_config *assigned_configuration = NULL;

static int write_internal_message(int level, UINT32 length, UINT8 *data)
{
	struct log_info *msg;
	int pri;

	if (assigned_configuration && assigned_configuration->internal) {
		switch (level) {
		case MSG_ERROR:
			pri = LOG_SYSLOG | LOG_ERR;
			break;
		case MSG_FATAL:
			pri = LOG_SYSLOG | LOG_CRIT;
			break;
		case MSG_DEBUG:
			pri = LOG_SYSLOG | LOG_DEBUG;
			break;
		case MSG_VERBOSE:
		case MSG_NOTICE:
		default:
			pri = LOG_SYSLOG | LOG_NOTICE;
		}
		while (data[length - 1] == '\n') 
			length--;
		msg = make_internal_message(pri, length, data);
		msg->flags |= LF_LOCAL;
		HANDLE_LOG(assigned_configuration->internal, msg);
		return 0;
	}
	return 0;
}

void set_error_internal(void)
{
	error_write = write_internal_message;
}

void set_internal_cfg(struct syslog_config *cfg)
{
	assigned_configuration = cfg;
}

static int do_init_afinter(struct log_handler *c, 
			   struct syslog_config *cfg,
			   struct persistent_config *persistent)
{
	CAST(log_source_driver, self, c);
	cfg->internal = self->super.next;
	if (assigned_configuration == NULL) {
		assigned_configuration = cfg;
	}
	return ST_OK | ST_GOON;
}

static void 
do_destroy_afinter(struct log_handler *c, 
		   struct syslog_config *cfg, 
		   struct persistent_config *persistent)
{
	cfg->internal = NULL;
}

struct log_source_driver *make_afinter_source(void)
{
	NEW(log_source_driver, self);

	self->super.super.init = do_init_afinter;
	self->super.super.destroy = do_destroy_afinter;
	return self;
}
