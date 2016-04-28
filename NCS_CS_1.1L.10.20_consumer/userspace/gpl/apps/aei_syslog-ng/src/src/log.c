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
 * $Id: log.c,v 1.32.4.1 2004/01/13 18:08:02 bazsi Exp $
 *
 ***************************************************************************/

#include "log.h"
#include "xalloc.h"
#include "format.h"
#include <stdio.h>
#include <sys/time.h>
#include <syslog.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define CLASS_DEFINE
#include "log.h.x"
#undef CLASS_DEFINE

static char aix_fwd_string[] = "Message forwarded from ";
static char repeat_msg_string[] = "last message repeated";

static void parse_log_msg(struct log_info *lm, UINT32 length, UINT8 *data, UINT8 *prefix, regex_t *hostname_re)
{
	unsigned char *src;
	unsigned int left, pri, oldleft;
	time_t now = time(NULL);
	unsigned char *oldsrc;
	char msg_string[512] = {0};
	static int repeat_cnt = 0;
	static char last_msg_string[512] = {0};

	src = data;
	left = length;

	if (left && src[0] == '<') {
		src++;
		left--;
		pri = 0;
		while (left && *src != '>') {
			if (isdigit(*src)) {
				pri = pri * 10 + ((*src) - '0');
			}
			else {
				lm->msg = c_format_cstring("unparseable log message: \"%s\"", length, data);
				lm->pri = LOG_SYSLOG | LOG_ERR;
				return;
			}
			src++;
			left--;
		}
		lm->pri = pri;
		if (left) {
			src++;
			left--;
		}
	}
	/* No priority info in the buffer? Just assign a default. */
	else {
	        lm->pri = LOG_USER | LOG_NOTICE;
	}

	while (left && *src == ' ') {  /* Move past whitespace */
		src++;
		left--;
	}

	/* If the next chars look like a date, then read them as a date. */
	if (left >= 15) {
                struct tm tm, *nowtm;

		if (left >= 21 &&
		    src[3] == ' ' && src[6] == ' ' && src[11] == ' ' &&
		    src[14] == ':' && src[17] == ':' && src[20] ==':') {
			/* PIX time stamp, format: MMM DD YYYY HH:MM:SS: */

			src[20] = 0;
			memset(&tm, 0, sizeof(tm));
			strptime((char *) src, "%b %e %Y %H:%M:%S", &tm);
			tm.tm_isdst = -1;
			lm->date = ol_string_alloc(20);

			strftime((char *) lm->date->data, 20, "%Y-%m-%d %H:%M:%S", &tm);
			lm->date->length--;

			src[20] = ':';
			src += 21;
			left -= 21;

			lm->stamp = mktime(&tm);
		}	 else if (src[3] == ' ' && src[6] == ' ' &&
		         src[9] == ':' && src[12] == ':') {
	  	        /* Expected buffer format: MMM DD HH:MM:SS ... */

			/* Just read the buffer data into a textual
			   datestamp. */
			lm->date = c_format_cstring("%s", 15, src);
			src += 15;
			left -= 15;

			/* And also make struct time timestamp for the msg */
			nowtm = localtime(&now);
			memset(&tm, 0, sizeof(tm));
			strptime((char *) lm->date->data, "%b %e %H:%M:%S", &tm);
			tm.tm_isdst = -1;
			tm.tm_year = nowtm->tm_year;

			if (tm.tm_mon > nowtm->tm_mon + 1)
				tm.tm_year--;

			ol_string_free(lm->date);
			lm->date = ol_string_alloc(20);
			strftime((char *) lm->date->data, 20, "%Y-%m-%d %H:%M:%S", &tm);

			lm->date->length--;
			lm->stamp = mktime(&tm);
		}
	}

	if (lm->date) {
	    /* Expected format: hostname program[pid]: */
	    /* Possibly: Message forwarded from hostname: ... */
	    unsigned char *hostname_start = NULL;
	    int hostname_len = 0;

	    while (left && *src == ' ') {
	        src++; /* skip whitespace */
	        left--;
	    }

		/* Detect funny AIX syslogd forwarded message. */
		if (left >= (sizeof(aix_fwd_string) - 1) &&
		    !memcmp(src, aix_fwd_string, sizeof(aix_fwd_string) - 1)) {

		        oldsrc = src;
			oldleft = left;
			src += sizeof(aix_fwd_string) - 1;
			left -= sizeof(aix_fwd_string) - 1;
			hostname_start = src;
			hostname_len   = 0;
			while (left && *src != ':') {
				src++;
				left--;
				hostname_len++;
			}
			while (left && (*src == ' ' || *src == ':')) {
				src++; /* skip whitespace */
				left--;
			}
		}

		/* Now, try to tell if it's a "last message repeated" line */
		if (left >= sizeof(repeat_msg_string) &&
		    !memcmp(src, repeat_msg_string,
			    sizeof(repeat_msg_string) - 1)) {
			; /* It is. Do nothing since there's no hostname or
			     program name coming. */
		}
		/* It's a regular ol' message. */
		else {
			/* If we haven't already found the original hostname,
			   look for it now. */
			char hostname_buf[256];
			int dst;

		        oldsrc = src;
			oldleft = left;

			dst = 0;
			while (left && *src != ' ' && *src != ':'
			       && *src != '[' && dst < sizeof(hostname_buf) - 1) {
				if (lm->flags & LF_CHECK_HOSTNAME &&
				    !((*src >= 'A' && *src <= 'Z') ||
				      (*src >= 'a' && *src <= 'z') ||
				      (*src >= '0' && *src <= '9') ||
				      *src == '-' || *src == '_' ||
				      *src == '.' || *src == ':' ||
				      *src == '@' || *src == '/')) {
					break;
				}
				hostname_buf[dst++] = *src;
		 		src++;
				left--;
			}
			hostname_buf[dst] = 0;
			if (left && *src == ' ' &&
			    (!hostname_re || regexec(hostname_re, hostname_buf, 0, NULL, 0))) {
		  		/* This was a hostname. It came from a
				   syslog-ng, since syslogd doesn't send
				   hostnames. It's even better then the one
				   we got from the AIX fwd message, if we
				   did. */
				hostname_start = oldsrc;
				hostname_len   = oldleft - left;
			} else {
		  		src = oldsrc;
		  		left = oldleft;
			}

			/* Skip whitespace. */
			while (left && *src == ' ') {
				src++;
				left--;
			}

			/* Try to extract a program name */
			oldsrc = src;
			oldleft = left;
			while (left && *src != ':' && *src != '[') {
				src++;
				left--;
			}
			if (left) {
				lm->program = c_format_cstring("%s",
						oldleft - left, oldsrc);
			}

			src = oldsrc;
			left = oldleft;
		}

		/* If we did manage to find a hostname, store it. */
		if (hostname_start)
			lm->host = c_format_cstring("%s", hostname_len,
						    hostname_start);
	}
	else {
	        /* Different format */

		oldsrc = src;
		oldleft = left;
		/* A kernel message? Use 'kernel' as the program name. */
		if ((lm->pri & LOG_FACMASK) == LOG_KERN) {
			lm->program = c_format_cstring("kernel");
		}
		/* No, not a kernel message. */
		else {
		        /* Capture the program name */
		        while (left && *src != ' ' && *src != '['
			       && *src != ':' && *src != '/' &&
			       *src != ',' && *src != '<') {
				src++;
				left--;
			}
			if (left) {
				lm->program = c_format_cstring("%s", oldleft - left, oldsrc);
			}
			left = oldleft;
			src = oldsrc;
		}
		lm->stamp = now;
	}

	for (oldsrc = src, oldleft = left; oldleft > 0; oldleft--, oldsrc++) {
		if (*oldsrc == '\n' || *oldsrc == '\r') *oldsrc = ' ';
	}

	/* check repeat message */
	if (strcmp(src, last_msg_string) == 0) {
	    repeat_cnt ++;
	    snprintf(msg_string, sizeof(msg_string),	 "%s %d times", repeat_msg_string,repeat_cnt);
	    src = msg_string;
	    left = strlen(msg_string);
	} else {
	    snprintf(last_msg_string, sizeof(last_msg_string),"%s", src);
	    repeat_cnt = 0;
	}

	lm->msg = c_format_cstring("%z%s", prefix ? prefix : (UINT8 *) "", left, src);
}

struct log_info *log_info_use(struct log_info *msg)
{
	msg->use_cnt++;
	return msg;
}

void log_info_free(struct log_info *msg)
{
	if (--msg->use_cnt == 0) {
		ol_string_free(msg->host);
		ol_string_free(msg->program);
		ol_string_free(msg->date);
		ol_string_free(msg->msg);
		ol_string_free(msg->host_from);
		ol_space_free(msg);
	}
}

struct log_info *make_log_info(UINT32 length, UINT8 *msg, UINT8 *prefix, UINT32 flags, regex_t *hostname_re)
{
	struct log_info *self;

	NEW_SPACE(self);
	self->flags = flags & LF_USER_FLAGS;
  	parse_log_msg(self, length, msg, prefix, hostname_re);
	self->use_cnt = 1;
  	self->recvd = time(NULL);
	return self;
}

struct log_info *make_internal_message(UINT32 pri, UINT32 length, UINT8 *data)
{
	struct log_info *self;

	NEW_SPACE(self);
	self->msg = c_format_cstring("syslog-ng[%i]: %s", getpid(), length, data);
	self->program = c_format_cstring("syslog-ng");
	self->stamp = self->recvd = time(NULL);
	self->pri = pri;
	self->flags = LF_INTERNAL;
	self->use_cnt = 1;
	self->host_from = NULL;
	return self;
}

struct log_info *make_mark_message(void)
{
	struct log_info *self = make_internal_message(LOG_SYSLOG | LOG_NOTICE, 12, (UINT8 *) "--- MARK ---");
	self->flags |= LF_MARK;
	return self;
}
