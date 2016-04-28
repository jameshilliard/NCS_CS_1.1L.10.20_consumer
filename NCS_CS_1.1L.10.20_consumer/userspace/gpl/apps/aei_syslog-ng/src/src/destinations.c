/***************************************************************************
 *
 * Copyright (c) 1999 Bal�zs Scheidler
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
 * $Id: destinations.c,v 1.18.4.1 2006/06/12 07:50:47 bazsi Exp $
 *
 ***************************************************************************/

#include "destinations.h"
#include "format.h"
#include "utils.h"
#include "cfgfile.h"
#include "resource.h"

#define CLASS_DEFINE
#include "destinations.h.x"
#undef CLASS_DEFINE

static void add_date(struct log_info *msg)
{
    struct tm *tm;
    char ts[24]={0};

    if (!msg->date) {
        if (!(tm = localtime(&msg->recvd))) {
            msg->date = c_format("%i", msg->recvd);
        } else {
            strftime(ts, sizeof(ts) - 1, "%Y-%m-%d %H:%M:%S", tm);
            msg->date = c_format("%z", ts);
        }
    }
}

static void add_host(struct log_info *msg)
{
	if (!msg->host) {
		char buf[256];
		
		msg->host = c_format_cstring("%z", getshorthostname(buf, sizeof(buf)));
	}
}

/* log_dest_group */

static int do_init_group(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(log_dest_group, self, c);
	int res;
	
	struct log_dest_driver *drv;
	
	res = 0;
	for (drv = self->drivers; drv; drv = drv->next_driver) {
		res |= LOG_HANDLER_INIT(drv, cfg, persistent);
		if (res & ST_QUIT) 
			break;
	}
	return res;
}

static void 
do_destroy_group(struct log_handler *c, 
		 struct syslog_config *cfg, 
		 struct persistent_config *persistent)
{
	CAST(log_dest_group, self, c);
	
	struct log_dest_driver *drv;
	
	for (drv = self->drivers; drv; drv = drv->next_driver) {
		if (drv->super.destroy)
			LOG_HANDLER_DESTROY(drv, cfg, persistent);
	}
}

static void do_handle_log(struct log_handler *c, struct log_info *msg)
{
	CAST(log_dest_group, self, c);
	struct log_dest_driver *drv;

	add_date(msg);
	add_host(msg);	

	for (drv = self->drivers; drv; drv = drv->next_driver) {
		HANDLE_LOG(drv, log_info_use(msg));
	}
	log_info_free(msg);
}

struct log_dest_group *make_dest_group(const char *name, struct log_dest_driver *drvs)
{
	NEW(log_dest_group, self);

	self->super.handler = do_handle_log;
	self->super.destroy = do_destroy_group;
	self->super.init = do_init_group;
	self->name = c_format("%z", name);
	self->drivers = drvs;
	
	return self;
}
