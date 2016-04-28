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
 * $Id: affile.c,v 1.61.4.7 2007/02/02 08:58:30 bazsi Exp $
 *
 ***************************************************************************/
 
/*
 * Several patches to the macro expansion function were contributed
 * by Gert Menke.
 *
 * Portions Copyright (c) 2002 by Gert Menke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "affile.h"
#include "format.h"
#include "pkt_buffer.h"
#include "cfgfile.h"
#include "utils.h"
#include "center.h"
#include "resource.h"
#include "cfgfile.h"
#include "macros.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include "syslog-names.h"

#include "affile.c.x"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SYSLOG_BACKUP_FILE   "/tmp/log/last_syslog.txt"

static int do_open_file(struct ol_string *name, int flags, 
                        int uid, int gid, int mode, 
                        int dir_uid, int dir_gid, int dir_mode,
                        int create_dirs, int *fd)
{
	if (strstr((char *) name->data, "../") ||
	    strstr((char *) name->data, "/..")) {
		werror("Spurious path %S, logfile not created\n", name);
		return 0;
	}
	if (strncmp((char *) name->data, "/dev", 4) == 0 && (uid != -1 || gid != -1 || mode != -1)) {
		werror("Changing permissions on special file %S\n", name);
	}
	werror("do_open_file: %S\n",name);
	fprintf(stderr,"do_open_file: mode =%x\n",mode);
	*fd = open((char *) name->data, flags, mode != -1 ? mode : 0600);
	if (create_dirs && *fd == -1 && errno == ENOENT) {
		/* directory does not exist */
		char *p = (char *) name->data + 1;

		p = strchr(p, '/');
		while (p) {
			struct stat st;
			*p = 0;
			if (stat((char *) name->data, &st) == 0) {
				if (!S_ISDIR(st.st_mode))
					return 0;
			}
			else if (errno == ENOENT) {
				if (mkdir((char *) name->data, dir_mode != -1 ? dir_mode : 0700) == -1)
					return 0;
				if (dir_uid != -1 || dir_gid != -1)
					chown((char *) name->data, dir_uid, dir_gid);
				if (dir_mode != -1)
					chmod((char *) name->data, dir_mode);
			}
			*p = '/';
			p = strchr(p + 1, '/');
		}
		*fd = open((char *) name->data, flags, mode);
	}
	if (uid != -1)
		chown((char *) name->data, uid, -1);
	if (gid != -1)
		chown((char *) name->data, -1, gid);
	if (mode != -1)
		chmod((char *) name->data, mode);
	fprintf(stderr,"do_open_file: *fd =%d\n",*fd);
	return *fd != -1;
}

/* CLASS:
   (class 
     (name affile_source)
     (super log_source_driver)
     (vars
       (flags . UINT32)
       (src object io_fd)
       (prefix pointer UINT8)
       (pad_size . UINT32)
       (name string)))
*/


static int do_init_affile_source(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(affile_source, self, c);
	int fd, flags;
  
  	if (self->flags & AFFILE_PIPE)
  		flags = O_RDWR | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;
  	else
	  	flags = O_RDONLY | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;
	if (do_open_file(self->name, flags, -1, -1, -1, -1, -1, -1, 0, &fd)) {
		io_set_nonblocking(fd);
		lseek(fd, 0, SEEK_END);
		self->src = io_read(make_io_fd(cfg->backend, fd, ol_string_use(self->name)), 
			make_log_reader(0, self->prefix, cfg->log_msg_size, self->pad_size, LF_NO_MULTI_READ | (cfg->check_hostname ? LF_CHECK_HOSTNAME : 0), cfg->bad_hostname, c), 
			NULL);
		REMEMBER_RESOURCE(cfg->resources, &self->src->super.super);
		return ST_OK | ST_GOON;
	}
	else {
		werror("Error opening file %S for reading (%z)\n", self->name, strerror(errno));
	}
	return ST_FAIL | ST_QUIT;
}

static void do_affile_handle_line(struct log_handler *c, struct log_info *log)
{
	CAST(affile_source, self, c);
	log->flags |= LF_LOCAL;
	HANDLE_LOG(self->super.super.next, log);
}

static void do_destroy_affile_source(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persist)
{
	CAST(affile_source, self, c);
	if (self->src) {
  		/* KILL_RESOURCE_NODE(cfg->resources, self->res);*/ 
  		closekill_fd(&self->src->super, 0);
  	}
}

struct log_source_driver *make_affile_source(const char *name, int flags, UINT8 *prefix, UINT32 pad_size)
{
	NEW(affile_source, self);
  
	self->super.super.super.init = do_init_affile_source;
	self->super.super.super.handler = do_affile_handle_line;
	self->super.super.super.destroy = do_destroy_affile_source;
	self->name = c_format_cstring("%z", name);
	self->flags = flags;
	self->prefix = prefix;
	self->pad_size = pad_size;
	return &self->super;
}

/* CLASS:
     (class
       (name affile_dest_reaper)
       (super callback)
       (vars
         (cfg object syslog_config)
         (affile_dest object affile_dest_writer)))
*/

static void do_reap_affile_dest(struct callback *c)
{
	CAST(affile_dest_reaper, self, c);

	if (self->affile_dest->prev) {
		self->affile_dest->prev->next = self->affile_dest->next;
	}
	else {
		self->affile_dest->owner->writers = self->affile_dest->next;
	}
	if (self->affile_dest->next) {
		self->affile_dest->next->prev = self->affile_dest->prev;
	}
	if (self->affile_dest && self->affile_dest->dest) {
		KILL_RESOURCE_NODE(self->cfg->resources, self->affile_dest->res);
	}
}

static struct callback *
make_affile_dest_reaper(struct syslog_config *cfg,
                        struct affile_dest_writer *affile_dest)
{
	NEW(affile_dest_reaper, self);
	
	self->super.f = do_reap_affile_dest;
	self->cfg = cfg;
	self->affile_dest = affile_dest;
	return &self->super;
}

/* CLASS:
     (class
       (name affile_dest_writer)
       (super log_handler)
       (vars
         (res pointer "struct resource_node")
         (time_reap simple UINT32)
         (owner object affile_dest)
         (dest object io_fd)
	 (reap simple "struct callout *")
         (expanded_fname string)
	 (prev object affile_dest_writer)
	 (next object affile_dest_writer)))
*/

static int
do_init_dest_writer(struct log_handler *c, struct syslog_config *cfg, struct persistent_config *persistent)
{
	CAST(affile_dest_writer, self, c);
	int fd, flags;
	
  	if (self->owner->flags & AFFILE_PIPE)
  		flags = O_RDWR | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;
  	else
	  	flags = O_WRONLY | O_CREAT | O_APPEND | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;

        if (self->owner->remove_if_older > 0) {
                /* remove (=unlink) if file is more than XXX days old */
                struct stat st;
                time_t now = time(NULL);

                if (stat((char *) self->expanded_fname->data, &st) == 0) {
                        if ((now-st.st_mtime) >= ((self->owner->remove_if_older))) {
                                unlink((char *) self->expanded_fname->data);
                        }
                }  
        }


	if (do_open_file(self->expanded_fname, flags, 
	                 self->owner->uid, self->owner->gid, self->owner->perm, 
	                 self->owner->dir_uid, self->owner->dir_gid, self->owner->dir_perm,
	                 self->owner->flags & AFFILE_CREATE_DIRS, &fd)) {

		self->dest = 
			io_write(make_io_fd(cfg->backend, fd, ol_string_use(self->expanded_fname)), 
				 make_pkt_buffer_ext(self->owner->sync_freq, 
				                     LIBOL_MAX(self->owner->super.log_fifo_size + 1, 
					                 self->owner->sync_freq),
					             !!(self->owner->flags & AFFILE_PIPE)),
				 NULL);
		if (self->owner->flags & AFFILE_FSYNC)
			self->dest->fsync = 1;
		self->res = REMEMBER_RESOURCE(cfg->resources, &self->dest->super.super);
		self->time_reap = cfg->time_reap;
		if ((self->owner->flags & AFFILE_NO_EXPAND) == 0) {
			self->reap = io_callout(cfg->backend, cfg->time_reap, make_affile_dest_reaper(cfg, self));
			/* we want to be called on reinitialization */
			io_callout_set_drop(self->reap, 0);
		}
		/* self->attempts = 0; */
	}
	else {
		werror("Cannot open file %S for writing (%z)\n", self->expanded_fname, strerror(errno));
		return ST_FAIL | ST_QUIT;
	}
	return ST_OK | ST_GOON;

}

static void do_handle_dest_writer(struct log_handler *c, struct log_info *msg)
{
	CAST(affile_dest_writer, self, c);

	if (self->dest) {
		struct ol_string *msg_line;

		if (self->owner->template_output) {
			msg_line = expand_macros(
					   self->owner->cfg,
					   self->owner->template_output,
					   self->owner->template_escape, msg);
		} else {
			msg_line = c_format("%S %S %S\n",
					    msg->date,
					    msg->host, msg->msg);
		}
		A_WRITE_STRING(&self->dest->buffer->super, msg_line);
		if (self->reap)
			io_callout_set_timeout(self->reap, self->time_reap);
	}
	log_info_free(msg);
}

struct affile_dest_writer *make_affile_dest_writer(struct affile_dest *owner, struct ol_string *expanded_fname)
{
	NEW(affile_dest_writer, self);

	self->super.init = do_init_dest_writer;
	self->super.handler = do_handle_dest_writer;
	self->owner = owner;
	self->expanded_fname = expanded_fname;
	return self;
}

/* CLASS:
     (class
       (name affile_dest)
       (super log_dest_driver)
       (vars
         (uid . int)
	 (gid . int)
	 (perm . int)
	 (dir_uid . int)
	 (dir_gid . int)
	 (dir_perm . int)
         (flags simple int)
	 (remove_if_older . int)
	 (sync_freq . int)
	 (template_fname string)
	 (template_output string)
	 (template_escape . int)
	 (writers object affile_dest_writer)
	 (cfg object syslog_config)))
*/

void affile_set_syncfreq(struct log_dest_driver *c, int syncfreq)
{
	CAST(affile_dest, self, c);

	self->sync_freq = syncfreq;
}

void affile_set_fsync(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);

	self->flags = set_flags(self->flags, AFFILE_FSYNC, AFFILE_FSYNC_SHIFT, enable);
}
void affile_set_stop_on_full(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);
	self->flags = set_flags(self->flags, AFFILE_STOP_ON_FULL, AFFILE_STOP_ON_FULL_SHIFT, enable);

}



void affile_dd_set_file_size_limit(struct log_dest_driver *c, int file_size_limit)
{
	CAST(affile_dest, self, c);
	fprintf(stderr, "%s:%s:%d ==before self->size_limit=%d==\n"
		,__FUNCTION__,__FILE__,__LINE__,self->size_limit);

	self->size_limit = file_size_limit;
	fprintf(stderr, "%s:%s:%d ==after self->size_limit=%d==\n"
		,__FUNCTION__,__FILE__,__LINE__,self->size_limit);

	return;
}


void affile_set_compress(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);

	self->flags = set_flags(self->flags, AFFILE_COMPRESS, AFFILE_COMPRESS_SHIFT, enable);
	werror("Warning: File compression not implemented yet.\n");
}

void affile_set_encrypt(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);

	self->flags = set_flags(self->flags, AFFILE_ENCRYPT, AFFILE_ENCRYPT_SHIFT, enable);
	werror("Warning: File encryption not implemented yet.\n");
}

void affile_set_owner(struct log_dest_driver *c, char *owner)
{
	CAST(affile_dest, self, c);
	struct passwd *pwd;

	pwd = getpwnam(owner);
	if (pwd)
		self->uid = pwd->pw_uid;
	else
		self->uid = atoi(owner);
}

void affile_set_group(struct log_dest_driver *c, char *group)
{
	CAST(affile_dest, self, c);
	struct group *grp;

	grp = getgrnam(group);
	if (grp)
		self->gid = grp->gr_gid;
	else
		self->gid = atoi(group);
}

void affile_set_perm(struct log_dest_driver *c, int perm)
{
	CAST(affile_dest, self, c);

	self->perm = perm;
}

void affile_set_dir_owner(struct log_dest_driver *c, char *owner)
{
	CAST(affile_dest, self, c);
	struct passwd *pwd;

	pwd = getpwnam(owner);
	if (pwd)
		self->dir_uid = pwd->pw_uid;
	else
		self->dir_uid = atoi(owner);
}

void affile_set_dir_group(struct log_dest_driver *c, char *group)
{
	CAST(affile_dest, self, c);
	struct group *grp;

	grp = getgrnam(group);
	if (grp)
		self->dir_gid = grp->gr_gid;
	else
		self->dir_gid = atoi(group);
}


void affile_set_dir_perm(struct log_dest_driver *c, int perm)
{
	CAST(affile_dest, self, c);

	self->dir_perm = perm;
}

void affile_set_create_dirs(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);

	self->flags = set_flags(self->flags, AFFILE_CREATE_DIRS, AFFILE_CREATE_DIRS_SHIFT, enable);
}

void affile_set_file_template(struct log_dest_driver *c, char *t)
{
	CAST(affile_dest, self, c);

	self->template_output = c_format("%z", t);
}

void affile_set_template_escape(struct log_dest_driver *c, int enable)
{
	CAST(affile_dest, self, c);

	self->template_escape = enable;
}

void affile_set_remove_if_older(struct log_dest_driver *c, int interval)
{
	CAST(affile_dest, self, c);

	if (interval < 0) {
		werror("Warning: the time before removal parameter is negative, disables option\n");
		interval = -1;
	}
	self->remove_if_older = interval;
}
static struct affile_dest_writer *
check_and_replace_dest_writer(struct affile_dest_writer * writer,struct log_info *msg)
{
	int fd, flags;
	off_t size = 0;
	struct stat statf;
	int lastMsgSize=0;
	char cmdstr[256]={0};

	if (NULL == writer->dest || NULL == writer->owner)
	{
		return writer;
	}
	if (writer->dest->super.fd >0)
	{
		if (fstat(writer->dest->super.fd, &statf) == 0 && S_ISREG(statf.st_mode))
			size = statf.st_size;
	}

	//fprintf(stderr,"===now size=%d,size_limit=%d,fd=%d\n"
	//	,size,writer->owner->size_limit,writer->dest->super.fd);
	struct ol_string *msg_line;
	msg_line = c_format("%S %S %S\n",
			msg->date,
			msg->host, msg->msg);
	//printf("msg length=(%d)\n",msg_line->length);
	lastMsgSize=msg_line->length;
	ol_string_free(msg_line);

	if (writer->owner && (size + lastMsgSize >= writer->owner->size_limit) )
	{
		fprintf(stderr,"===now size + lastMsgSize =%d,size_limit=%d,lastMsgSize=%d,file size=%d\n"
		,size + lastMsgSize,writer->owner->size_limit,lastMsgSize,size);

		if (!(writer->owner->flags & AFFILE_STOP_ON_FULL))
		{
			fprintf(stderr,"###### syslog file content is full to backup!\n");
			snprintf(cmdstr, sizeof(cmdstr), "cp %s %s", (char *)writer->expanded_fname->data, SYSLOG_BACKUP_FILE);
			system(cmdstr);

			close(writer->dest->super.fd);

			if (writer->owner->flags & AFFILE_PIPE)
				flags = O_RDWR | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;
			else
				flags = O_WRONLY | O_CREAT | O_APPEND | O_NOCTTY | O_NONBLOCK | O_LARGEFILE;

			flags |= O_TRUNC;
			if (do_open_file(writer->expanded_fname, flags, 
							 writer->owner->uid, writer->owner->gid, writer->owner->perm, 
							 writer->owner->dir_uid, writer->owner->dir_gid, writer->owner->dir_perm,
							 writer->owner->flags & AFFILE_CREATE_DIRS, &fd))
			{
				writer->dest->super.fd =fd;
				lseek(writer->dest->super.fd, 0, SEEK_CUR);
				fprintf(stderr,"new fd=%d\n\n",writer->dest->super.fd); 
			}
		}
		else
		{
			close(writer->dest->super.fd);
			printf("####log file is full, stop===\n");
		}
		
	}
	return writer;
}

/* FIXME: a binary search would be nicer */
static struct affile_dest_writer *
affile_find_writer(struct affile_dest *self, 
		   struct log_info *msg)
{
	struct ol_string *expanded_fname = NULL;
	struct affile_dest_writer *p;
	UINT32 res;	

	if (self->flags & AFFILE_NO_EXPAND) {
		if (!self->writers) {
			//werror("###open file %S=========\n",self->template_fname);
			self->writers = make_affile_dest_writer(self, ol_string_use(self->template_fname));
			res = LOG_HANDLER_INIT(self->writers, self->cfg, NULL);
			if (res & ST_QUIT)
				return NULL;
		}
		check_and_replace_dest_writer(self->writers,msg);
		return self->writers;
	}
	//werror("======4444=========\n");

	expanded_fname = expand_macros(self->cfg, self->template_fname, 0, msg);
	if (self->flags & AFFILE_SANITIZE_FILENAMES) {
		int i;
		
		for (i = 0; i < expanded_fname->length; i++) {
			if (expanded_fname->data[i] < 32) {
				expanded_fname->data[i] = '.';
			}
		}
	}
	p = self->writers;
	while (p && 
	       ((p->expanded_fname->length != expanded_fname->length) || 
		(memcmp(p->expanded_fname->data, expanded_fname->data, expanded_fname->length) != 0))) {
		p = p->next;
	}
	if (p) {
		ol_string_free(expanded_fname);
		return p;
	}
	else {
	
		/* allocate new writer */
		p = make_affile_dest_writer(self, expanded_fname);
		res = LOG_HANDLER_INIT(p, self->cfg, NULL);
		if (res & ST_QUIT)
			return NULL;
		p->next = self->writers;
		if (p->next) p->next->prev = p;
		self->writers = p;
		return p;
	}
}

static int 
do_init_affile_dest(struct log_handler *c, 
		    struct syslog_config *cfg, 
		    struct persistent_config *persistent)
{
	CAST(affile_dest, self, c);
	
	if (self->sync_freq == -1)
		self->sync_freq = cfg->sync_freq;
	if (self->size_limit == -1)
	{
		fprintf(stderr, "%s:%s:%d ==self->size_limit=%d,cfg->file_size_limit=%d==\n"
			,__FUNCTION__,__FILE__,__LINE__,self->size_limit,cfg->file_size_limit);

		self->size_limit = cfg->file_size_limit;
	}
	
	if (self->super.log_fifo_size == -1)
		self->super.log_fifo_size = LIBOL_MIN(1000, cfg->log_fifo_size);
	if (cfg->create_dirs)
		self->flags |= AFFILE_CREATE_DIRS;
	if (self->uid == -1)
		self->uid = cfg->uid;
	if (self->gid == -1)
		self->gid = cfg->gid;
	if (self->perm == -1)
		self->perm = cfg->perm;
	if (self->dir_uid == -1)
		self->dir_uid = cfg->dir_uid;
	if (self->dir_gid == -1)
		self->dir_gid = cfg->dir_gid;
	if (self->dir_perm == -1)
		self->dir_perm = cfg->dir_perm;
	if (cfg->sanitize_filenames)
		self->flags |= AFFILE_SANITIZE_FILENAMES;
	
	self->cfg = cfg;
	return ST_OK | ST_GOON;
}

static void do_handle_affile_log(struct log_handler *c, struct log_info *msg)
{
	CAST(affile_dest, self, c);
	struct affile_dest_writer *w;
	//werror("==%S==\n",msg->msg);

	w = affile_find_writer(self, msg);
	if (w) {
		HANDLE_LOG(&w->super, msg);
	}
	else {
		log_info_free(msg);
	}
}

struct log_dest_driver *make_affile_dest(const char *name, int flags)
{
	NEW(affile_dest, self);
	
	self->super.super.init = do_init_affile_dest;
	self->super.super.handler = do_handle_affile_log;
	self->super.log_fifo_size = -1;
	self->uid = self->gid = -1;
	self->perm = -1;
	self->dir_uid = self->dir_gid = -1;
	self->dir_perm = -1;
	self->remove_if_older = -1;
	self->template_fname = c_format_cstring("%z", name);
	self->template_escape = 1;
	if (strchr(name, '$') == NULL)
		flags |= AFFILE_NO_EXPAND;
	self->flags = flags;
	self->sync_freq = -1; /* use global setting */
	self->size_limit=-1;
	return &self->super;
}
