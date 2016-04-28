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
 * $Id: afuser.c,v 1.9.4.1 2005/10/15 16:38:19 bazsi Exp $
 *
 ***************************************************************************/

#include "afuser.h"
#include "destinations.h"
#include "format.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <utmp.h>

#include "afuser.c.x"
#include "macros.h"

/* CLASS:
   (class
     (name afuser_dest)
     (super log_dest_driver)
     (vars
       (username string)
       (template_output string)
       (template_escape . int)
       (cfg object syslog_config)))
*/

void
afuser_dest_set_template(struct log_dest_driver *c, char *t)
{
	CAST(afuser_dest, self, c);
 
	self->template_output = c_format("%z", t);
}

void
afuser_dest_set_template_escape(struct log_dest_driver *c, int enable)
{
	CAST(afuser_dest, self, c);

	self->template_escape = enable;
}

static int 
do_init_afuser_dest(struct log_handler *c, 
		    struct syslog_config *cfg, 
		    struct persistent_config *persistent)
{
	CAST(afuser_dest, self, c);

	self->cfg = cfg;

	return ST_OK | ST_GOON;
}

static void
do_handle_afuser_log(struct log_handler *c, struct log_info *msg)
{
	CAST(afuser_dest, self, c);
	struct utmp *ut;
	struct ol_string *msg_line;

	if (self->template_output) {
		msg_line = c_format("%fS",
				    expand_macros(
					    self->cfg,
					    self->template_output,
					    self->template_escape, msg));
	} else {
		msg_line = c_format("%S %S %S\n",
				    msg->date, msg->host, msg->msg);
	}

	while ((ut = getutent())) {
#if HAVE_MODERN_UTMP
		if (ut->ut_type == USER_PROCESS &&
		    ((self->username->length == 1 && 
		      self->username->data[0] == '*') ||
		     (self->username->length <= sizeof(ut->ut_user) &&
		      memcmp(self->username->data, ut->ut_user, self->username->length) == 0))) {
#else
		if ((self->username->length == 1 && 
		     self->username->data[0] == '*') ||
		    (self->username->length <= sizeof(ut->ut_name) &&
		     memcmp(self->username->data, ut->ut_name, self->username->length) == 0)) {
#endif
			char line[128];
			int fd;

			strcpy(line, "/dev/");
			strcat(line, ut->ut_line);
			fd = open(line, O_NOCTTY | O_APPEND | O_WRONLY | O_NONBLOCK);
			if (fd != -1) {
				write_raw_with_poll(fd, msg_line->length, msg_line->data);
				close(fd);
			}

		}
	}
	endutent();
	ol_string_free(msg_line);
	log_info_free(msg);
}

struct log_dest_driver *make_afuser_dest(const char *username)
{
	NEW(afuser_dest, self);
	
	self->super.super.init = do_init_afuser_dest;
	self->super.super.handler = do_handle_afuser_log;
	self->username = c_format("%z", username);
	return &self->super;
}
