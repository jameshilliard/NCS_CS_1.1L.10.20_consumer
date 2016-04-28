/***************************************************************************
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
 ***************************************************************************/

#include "afremctrl.h"
#include "destinations.h"
#include "cfgfile.h"
#include "format.h"
#include "resource.h"

#include <string.h>

#include "afremctrl.c.x"

/* CLASS:
   (class
     (name afremctrl_dest)
     (super log_dest_driver)
     (vars
       (cfg object syslog_config)))
*/

static int 
do_init_afremctrl_dest(struct log_handler *c, 
		       struct syslog_config *cfg,
		       struct persistent_config *persistent)
{
        CAST(afremctrl_dest, self, c);

	self->cfg = cfg;
	return ST_OK | ST_GOON;
}

static void do_handle_afremctrl_dest(struct log_handler *c, 
				     struct log_info *msg)
{
	int found;
	struct nonblocking_fd *fd;
        CAST(afremctrl_dest, self, c);

	for (found = 0, fd = self->cfg->backend->files; fd; fd = fd->next) {
		if (fd->fname && !strcmp(fd->fname->data, msg->msg->data)) {
			found = 1;
			if (reopen_fd(fd) == 0)
				notice("afremctr: reopened %S (%i)\n",
					msg->msg, fd->fd);
			break;
		}
	}

	if (!found) {
		werror("afremctrl: couldn't find `%S' for re-opening\n",
			msg->msg);
	}
        log_info_free(msg);
}

struct log_dest_driver *make_afremctrl_dest(void)
{
	NEW(afremctrl_dest, self);

	self->super.super.init = do_init_afremctrl_dest;
	self->super.super.handler = do_handle_afremctrl_dest;
	return &self->super;
}
