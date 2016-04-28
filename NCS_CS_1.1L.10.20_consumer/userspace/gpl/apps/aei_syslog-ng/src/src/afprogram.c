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
 * $Id: afprogram.c,v 1.7.4.3 2004/11/18 11:44:12 bazsi Exp $
 *
 ***************************************************************************/

#include "afprogram.h"
#include "destinations.h"
#include "cfgfile.h"
#include "format.h"
#include "resource.h"
#include "pkt_buffer.h"

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "afprogram.c.x"
#include "macros.h"

/* CLASS:
   (class
     (name afprogram_dest)
     (super log_dest_driver)
     (vars
       (progname string)
       (dest object io_fd)
       (template_output string)
       (template_escape . int)
       (cfg object syslog_config)))
*/

/* CLASS:
   (class
     (name afprogram_child)
     (super resource)
     (vars
       (pid simple pid_t)
       (dest object io_fd)))
*/
void
afprogram_dest_set_template(struct log_dest_driver *c, char *t)
{
	CAST(afprogram_dest, self, c);
 
	self->template_output = c_format("%z", t);
}

void
afprogram_dest_set_template_escape(struct log_dest_driver *c, int enable)
{
	CAST(afprogram_dest, self, c);

	self->template_escape = enable;
}


static void do_kill_child(struct resource *c)
{
	CAST(afprogram_child, self, c);
	kill(self->pid, SIGTERM);
}

static int 
do_init_afprogram_dest(struct log_handler *c, 
		       struct syslog_config *cfg,
		       struct persistent_config *persistent)
{
	CAST(afprogram_dest, self, c);
	pid_t pid;
	int msgpipe[2];

	if (pipe(msgpipe) < 0) {
		werror("do_afprogram_init(): Error in pipe(): %z\n", strerror(errno));
		return ST_FAIL | ST_QUIT;
	}
	if ((pid = fork()) < 0) {
		close(msgpipe[0]);
		close(msgpipe[1]);
		werror("do_afprogram_init(): Error in fork(): %z\n", strerror(errno));
		return ST_FAIL | ST_QUIT;
	}
	if (pid == 0) {
		int fd = open("/dev/null", O_WRONLY);

		dup2(msgpipe[0], 0);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);
		close(msgpipe[1]);
		setsid();
		execl("/bin/sh", "/bin/sh", "-c", self->progname->data, (void *) NULL);
		_exit(127);
	}
	else {
		NEW(afprogram_child, prg);
		self->cfg = cfg;

		prg->super.kill = do_kill_child;
		prg->pid = pid;
		REMEMBER_RESOURCE(cfg->resources, &prg->super);

		close(msgpipe[0]);
		self->dest = io_write(make_io_fd(cfg->backend, msgpipe[1], NULL),
				      make_pkt_buffer_ext(0, cfg->log_fifo_size, 0),
				      NULL);
		REMEMBER_RESOURCE(cfg->resources, &self->dest->super.super);
	}

	return ST_OK | ST_GOON;
}



static void
do_handle_afprogram_dest(struct log_handler *c, 
				     struct log_info *msg)
{
        CAST(afprogram_dest, self, c);

        if (self->dest) {
		struct ol_string *msg_line;

		if (self->template_output) {
			msg_line = c_format("%fS",
					    expand_macros(
						    self->cfg,
						    self->template_output,
						    self->template_escape, msg));
		} else {
			msg_line = c_format("<%i>%S %S %S\n",
					    msg->pri,
					    msg->date, msg->host, msg->msg);
		}
                A_WRITE_STRING(&self->dest->buffer->super, msg_line);
	} 
        log_info_free(msg);
}

struct log_dest_driver *make_afprogram_dest(const char *cmd)
{
	NEW(afprogram_dest, self);

	self->super.super.init = do_init_afprogram_dest;
	self->super.super.handler = do_handle_afprogram_dest;
	self->progname = c_format_cstring("%z", cmd);
	return &self->super;
}
