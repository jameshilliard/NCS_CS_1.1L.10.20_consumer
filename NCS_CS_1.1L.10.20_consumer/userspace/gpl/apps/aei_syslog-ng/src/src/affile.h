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
 * $Id: affile.h,v 1.19.4.1 2004/05/06 08:38:27 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __AFFILE_H
#define __AFFILE_H

#include "syslog-ng.h"

#include "log.h"
#include "io.h"
#include "sources.h"
#include "destinations.h"

#define AFFILE_COMPRESS       0x0001
#define AFFILE_COMPRESS_SHIFT 0

#define AFFILE_ENCRYPT        0x0002
#define AFFILE_ENCRYPT_SHIFT  1

#define AFFILE_PIPE           0x0004
#define AFFILE_NO_EXPAND      0x0008

#define AFFILE_CREATE_DIRS    0x0010
#define AFFILE_CREATE_DIRS_SHIFT 4

#define AFFILE_FSYNC          0x0020
#define AFFILE_FSYNC_SHIFT    5
#define AFFILE_SANITIZE_FILENAMES 0x0040

#define AFFILE_STOP_ON_FULL          0x0080
#define AFFILE_STOP_ON_FULL_SHIFT    7


void affile_set_syncfreq(struct log_dest_driver *affile, int syncfreq);
void affile_set_fsync(struct log_dest_driver *c, int enable);
void affile_set_stop_on_full(struct log_dest_driver *c, int enable);
void affile_dd_set_file_size_limit(struct log_dest_driver *c, int file_size_limit);
void affile_set_compress(struct log_dest_driver *affile, int enable);
void affile_set_encrypt(struct log_dest_driver *affile, int enable);
void affile_set_owner(struct log_dest_driver *c, char *owner);
void affile_set_group(struct log_dest_driver *c, char *group);
void affile_set_perm(struct log_dest_driver *c, int perm);
void affile_set_dir_owner(struct log_dest_driver *c, char *owner);
void affile_set_dir_group(struct log_dest_driver *c, char *group);
void affile_set_dir_perm(struct log_dest_driver *c, int perm);
void affile_set_create_dirs(struct log_dest_driver *c, int enable);
void affile_set_file_template(struct log_dest_driver *c, char *template);
void affile_set_template_escape(struct log_dest_driver *c, int enable);
void affile_set_remove_if_older(struct log_dest_driver *c, int interval);

struct log_source_driver *make_affile_source(const char *name, int flags, UINT8 *prefix, UINT32 pad_size);
struct log_dest_driver *make_affile_dest(const char *name, int flags);

#endif
