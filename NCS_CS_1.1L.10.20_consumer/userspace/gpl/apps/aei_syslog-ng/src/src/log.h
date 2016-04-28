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
 * $Id: log.h,v 1.19.4.1 2006/04/30 20:10:24 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __LOG_H
#define __LOG_H

#include "syslog-ng.h"
#include "io.h"

#include <sys/time.h>
#include <regex.h>

struct syslog_config;
struct persistent_config;

struct log_info {
	UINT32 use_cnt;
	time_t recvd;
	time_t stamp;
	struct log_handler *source;
	UINT32 pri;
	UINT32 flags;
	struct address_info *saddr;
	struct ol_string *msg;
	struct ol_string *date;
	struct ol_string *program;
	struct ol_string *host;
	struct ol_string *host_from;
};

#define CLASS_DECLARE
#include "log.h.x"
#undef CLASS_DECLARE

#define LF_INTERNAL           0x0001
#define LF_MARK               0x0002
#define LF_LOCAL              0x0004
/* hack, this is piggybacked to msg_flags instead of using a separate argument of make_log_reader */
#define LF_NO_MULTI_READ      0x0008
#define LF_CHECK_HOSTNAME     0x0100

#define LF_USER_FLAGS 0xff00

/* CLASS:
     (class
       (name log_handler)
       (vars
         (params object log_handler_params)
         (handler method void "struct log_info *")
         (init method int "struct syslog_config *" 
	                  "struct persistent_config *")
	 (destroy method void "struct syslog_config *" 
	                      "struct persistent_config *")))
*/

#define HANDLE_LOG(l, i) (((struct log_handler *) l)->handler((struct log_handler *) (l), i))
#define LOG_HANDLER_INIT(l, c, p) ((struct log_handler *) l)->init((struct log_handler *) (l), c, p)
#define LOG_HANDLER_DESTROY(l, c, p) ((struct log_handler *) l)->destroy((struct log_handler *) (l), c, p)

/* CLASS:
     (class
       (name log_handler_pipe)
       (super log_handler)
       (vars
         (next object log_handler)))
*/

#define append_log_handler(item, append) ((struct log_handler_pipe *) (item))->next = ((struct log_handler *) append);

struct log_info *log_info_use(struct log_info *msg);
void log_info_free(struct log_info *msg);

struct log_info *make_log_info(UINT32 length, UINT8 *data, UINT8 *prefix, UINT32 flags, regex_t *badhostname_re);
struct log_info *make_internal_message(UINT32 pri, UINT32 length, UINT8 *data);
struct log_info *make_mark_message(void);

#endif

