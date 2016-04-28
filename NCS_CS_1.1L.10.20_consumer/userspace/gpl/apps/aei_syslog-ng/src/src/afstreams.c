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
 * $Id: afstreams.c,v 1.16.4.2 2004/05/13 11:33:36 bazsi Exp $
 *
 ***************************************************************************/

#include "afstreams.h"
#include "sources.h"
#include "format.h"
#include "cfgfile.h"

#include "afstreams.c.x"

#if HAVE_SUN_STREAMS

#include <sys/types.h>
#include <sys/stat.h>
#include <stropts.h>
#include <sys/strlog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>

#if HAVE_SUN_DOOR && HAVE_DOOR_H
#include <door.h>
#endif

/* CLASS:
   (class
     (name door_fd)
     (super resource)
     (vars
       (fd simple int)))
*/

#if HAVE_SUN_DOOR
static void do_kill_door(struct resource *r)
{
	CAST(door_fd, self, r);

	door_revoke(self->fd);
	close(self->fd);
}

static struct door_fd *make_door_fd(int fd)
{
	NEW(door_fd, f);

	f->super.kill = do_kill_door;
	f->fd = fd;
	return f;
}
#endif

/* CLASS:
   (class
     (name stream_fd)
     (super nonblocking_fd)
     (vars
       (bad_hostname special-struct regex_t #f regfree)
       (max_log_line . UINT32)
       (pipe object log_handler)))
*/

static void prepare_stream_fd(struct nonblocking_fd *fd)
{
	if (fd->to_be_closed)
		kill_fd(fd);
}

static int do_stream_read(int fd, UINT32 *pri, UINT32 length, UINT8 *buf)
{
	struct strbuf ctl, data;
	struct log_ctl lc;
	int flags;
	int res;

	ctl.maxlen = ctl.len = sizeof(lc);
	ctl.buf = (char *) &lc;
	data.maxlen = length;
	data.len = 0;
	data.buf = buf;
	flags = 0;

	res = getmsg(fd, &ctl, &data, &flags);
	if ((res & MORECTL) == 0) {
		if (res & MOREDATA) {
			werror("do_stream_read: STREAMS device gave too long line\n");
		}
		*pri = lc.pri;
	}
	else {
		werror("do_stream_read: getmsg() trying to return too much ctl data, res=%i %z\n", res, strerror(errno));
		return A_FAIL;
	}
	return data.len;
}

static void stream_read_callback(struct nonblocking_fd *fd)
{
	CAST(stream_fd, self, fd);
	char *line = alloca(self->max_log_line + 1);
	UINT32 pri;
	int n;
	
	n = do_stream_read(self->super.fd, &pri, self->max_log_line, line);
	if (n < 0) {
		close_fd(fd, CLOSE_EOF);
		return;
	}

	{
		UINT8 *eol, *bol;
		UINT32 length;
		struct log_info *li;

		bol = line;
		eol = memchr(bol, '\n', n);
		if (!eol) eol = memchr(bol, '\0', n);
		
		if (!eol) {
		
			/* FIXME: there must be space for this '\0' as we
			 * specified sizeof(buf) - 1 when reading the message
			 */
			
			eol = &line[n];
			line[n] = '\0';
			n++;
		}

		while (eol) {

			if (*bol == '\r') {
				bol++;
				n--;
			}
			
			length = eol - bol;
			if (length) {
				li = make_log_info(length, bol, NULL, 0, &self->bad_hostname);
				li->pri = pri;
				HANDLE_LOG(self->pipe, li);
			}

			n -= length + 1;
			bol = eol + 1;
			eol = memchr(bol, '\n', n);
			if (!eol) eol = memchr(bol, '\0', n);
		}
	}
}

struct nonblocking_fd *io_stream_get(struct io_backend *backend,
				     int fd, 
				     UINT8 *hostname_re,
				     UINT32 max_log_line,
				     struct log_handler *pipe)
{
	NEW(stream_fd, f);

	init_file(backend, &f->super, fd, NULL);
	f->super.prepare = prepare_stream_fd;
	f->super.read = stream_read_callback;
	f->super.want_read = 1;
	f->pipe = pipe;
	f->max_log_line = max_log_line;
	if (hostname_re == NULL)
		regcomp(&f->bad_hostname, "^$", REG_NOSUB | REG_EXTENDED);
	else
		regcomp(&f->bad_hostname, hostname_re, REG_NOSUB | REG_EXTENDED);

	return &f->super;
}

/* CLASS:
   (class
     (name afstreams_source)
     (super log_source_driver)
     (vars
       (name string)
       (door_name string)
       (stream_fd object nonblocking_fd)
       (door object door_fd)))
*/

#if HAVE_SUN_DOOR
void afstreams_door_server_proc(void *cookie, char *argp, size_t arg_size, door_desc_t *dp, size_t n_desc)
{
        door_return(NULL, 0, NULL, 0);
        return;
}
#endif

void afstreams_set_sundoor(struct log_source_driver *c, const char *door)
{
	CAST(afstreams_source, self, c);
	self->door_name = c_format_cstring("%z", door);
}

static int 
do_init_afstreams_source(struct log_handler *c, 
			 struct syslog_config *cfg, 
			 struct persistent_config *persistent)
{
	CAST(afstreams_source, self, c);
	int fd;

	fd = open(self->name->data, O_RDONLY | O_NOCTTY | O_NONBLOCK);
	if (fd != -1) {
		struct strioctl ioc;
#if HAVE_SUN_DOOR
		struct stat st;
#endif
		
		memset(&ioc, 0, sizeof(ioc));
		ioc.ic_cmd = I_CONSLOG;
		if (ioctl(fd, I_STR, &ioc) < 0) { 
			werror("do_init_afstreams_source: Cannot enable console logging on sun-stream %S (%z)\n", self->name, strerror(errno));
			close(fd);
			return ST_FAIL | ST_QUIT;
		}
		self->stream_fd = io_stream_get(cfg->backend, fd, cfg->bad_hostname, cfg->log_msg_size, c);

		REMEMBER_RESOURCE(cfg->resources, &self->stream_fd->super);

#if HAVE_SUN_DOOR
		if (self->door_name) {
			if (stat(self->door_name->data, &st) == -1) {
				fd = creat(self->door_name->data, 0666);
				if (fd == -1) {
					werror("do_init_afstreams_source: cannot create door file %S (%z)\n", self->door_name, strerror(errno));
					return ST_FAIL | ST_QUIT;
				}
				close(fd);
			}
			fdetach(self->door_name->data);
			fd = door_create(afstreams_door_server_proc, NULL, 0);
	                if (fd == -1) {
        	                werror("do_init_afstreams_source: Cannot initialize door server %S (%z)\n", self->name, strerror(errno));
	                        return ST_FAIL | ST_QUIT;
	                }
	                if (fattach(fd, self->door_name->data) == -1) {
	                        werror("do_init_afstreams_source: Cannot attach door to %S (%z)\n", self->name, strerror(errno));
				close(fd);
	                        return ST_FAIL | ST_QUIT;
			}
			self->door = make_door_fd(fd);
			REMEMBER_RESOURCE(cfg->resources, &self->door->super);
	
		}
#else
		if (self->door_name) {
			close_fd(self->stream_fd, 0);
			werror("sun-door support is not compiled in, door=%S\n", self->door_name);
			return ST_FAIL | ST_QUIT;
		}
#endif
	}
	else {
		werror("do_init_afstreams_source: Cannot open sun-stream %S (%z)\n", self->name, strerror(errno));
		return ST_FAIL | ST_QUIT;
	}
	return ST_OK | ST_GOON;
}

static void do_handle_afstreams_source(struct log_handler *c, struct log_info *log)
{
	CAST(afstreams_source, self, c);
	log->flags |= LF_LOCAL;
	HANDLE_LOG(self->super.super.next, log);
}

struct log_source_driver *make_afstreams_source(const char *name)
{
	NEW(afstreams_source, self);

	self->super.super.super.init = do_init_afstreams_source;
	self->super.super.super.handler = do_handle_afstreams_source;
	self->name = c_format_cstring("%z", name);
	return &self->super;
}

#else

void afstreams_set_sundoor(struct log_source_driver *c, const char *door_name)
{
}

static int 
do_init_afstreams_source(struct log_handler *c, 
			 struct syslog_config *cfg,
			 struct persistent_config *persistent)
{
	CAST(afstreams_source, self, c);
	werror("sun-streams support not compiled in (STREAM=%S)\n", self->name);
	return ST_FAIL | ST_QUIT;
}

struct log_source_driver *make_afstreams_source(const char *name)
{
	NEW(afstreams_source, self);

	self->super.super.super.init = do_init_afstreams_source;
	self->name = c_format_cstring("%z", name);
	return &self->super;
}


#endif
