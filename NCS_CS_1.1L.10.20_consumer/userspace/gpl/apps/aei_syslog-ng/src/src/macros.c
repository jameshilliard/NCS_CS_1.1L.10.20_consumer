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
 * $Id: macros.c,v 1.4.4.9 2006/05/26 19:21:13 bazsi Exp $
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

/*
 * The patch to add the "template" and "template_escape" options to all
 * destination drivers was contributed by Achim Gsell.
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "format.h"
#include "cfgfile.h"
#include "macros.h"
#include "syslog-names.h"


#define MAX_MACRO_ARGS        32
#define MAX_EXPANDED_MACRO    2048


static int
append_string(char **dest, unsigned int *left, char *str, int length, int escape)
{
	int l;
  
	if (!escape) {
		l = LIBOL_MIN(length, *left - 1);
		strncpy(*dest, str, l);
	}
	else {
		char *p;
      
		l = 0;
		for (p = str; length && ((*left - l) > 1); p++, length--, l++) {
			if (*p == '\'' || *p == '\"' || *p == '\\') {
				if ((*left - l) < 3)
					break;
				*(*dest + l) = '\\';
				*(*dest + l + 1) = *p;
				l++;
			}
			else {
				*(*dest + l) = *p;
			}
		}
	}
	return l;
}

static size_t
format_tzofs(char *dest, size_t left, time_t unixtime, struct tm *tm)
{
	size_t length;
	
#if HAVE_STRFTIME_PERCENT_Z
	length = strftime(dest, left -1, "%z", tm);
#else
        struct tm ltm, *gtm;
	long tzoff;

	/* NOTE: we need to copy the results away, gmtime might use the same
	 * buffer as localtime */
	ltm = *localtime(&unixtime);
	gtm = gmtime(&unixtime);

	tzoff = (ltm.tm_hour - gtm->tm_hour) * 3600;
	tzoff += (ltm.tm_min - gtm->tm_min) * 60;
	tzoff += ltm.tm_sec - gtm->tm_sec;

       /*
        * NOTE! We are not allowed to change or normalize or otherwise modify
        * the timezone offset. It is what it is, and there are places in the
        * world where the offset exceeds 12 hours (e.g. New Zealand DST).
        *
        * The code below, simply accounts for the fact that a mere subtraction
        * of hours may be off by 24 hours when either of the two times is on
        * a different calendar day than the other. We are assuming that the
        * GMT offset is never >= +2400 or <= -2400. This is safe.
        */

       if (tzoff > 0
           && (ltm.tm_year < gtm->tm_year || ltm.tm_mon < gtm->tm_mon || ltm.tm_mday < gtm->tm_mday))
               tzoff -= 86400;
       else if (tzoff < 0
                && (ltm.tm_year > gtm->tm_year || ltm.tm_mon > gtm->tm_mon || ltm.tm_mday > gtm->tm_mday))
               tzoff += 86400;

	length = snprintf(dest, left - 1, "%c%02ld%02ld", 
			  tzoff < 0 ? '-' : '+', 
			  (tzoff < 0 ? -tzoff : tzoff) / 3600, 
			  (tzoff % 3600) / 60);
#endif
	return length;
}

static void
expand_macro(struct syslog_config *cfg, int id, int escape, char **dest, unsigned int *left, struct log_info *msg)
{
	int length = 0;
	
	switch (id) {
	case M_FACILITY: {
		/* facility */
		char *n = syslog_lookup_value(msg->pri & LOG_FACMASK, sl_facilities);
		if (n) {
			length = append_string(dest, left, n, strlen(n), 0);
		}
		else {
			length = snprintf(*dest, *left, "%x", (msg->pri & LOG_FACMASK) >> 3);
		}
		break;
	}
        case M_FACILITY_NUM: {
                length = snprintf(*dest, *left, "%d", (msg->pri & LOG_FACMASK) >> 3);
                break;
        }
	case M_LEVEL: {
		/* level */
		char *n = syslog_lookup_value(msg->pri & LOG_PRIMASK, sl_levels);
		if (n) {
			length = append_string(dest, left, n, strlen(n), 0);
		}
		else {
			/* should never happen */
			length = snprintf(*dest, *left, "%d", msg->pri & LOG_PRIMASK);
		}

		break;
	}
        case M_LEVEL_NUM: {
                length = snprintf(*dest, *left, "%d", (msg->pri & LOG_PRIMASK));
                break;
        }
	case M_TAG: {
		length = snprintf(*dest, *left, "%02x", msg->pri);
		break;
	}
	case M_PRI: {
		length = snprintf(*dest, *left, "%d", msg->pri);
		break;
	}
	case M_SOURCE_IP: {
 		char *ip;
 		
		if (msg->saddr) {
	 		CAST(inet_address_info, addr, msg->saddr);
 		
	 		ip = inet_ntoa(addr->sa.sin_addr);
		}
		else {
			ip = "127.0.0.1";
		}
		length = append_string(dest, left, ip, strlen(ip), escape);
		break;
	}
	case M_FULLHOST_FROM:
	case M_FULLHOST: {
		struct ol_string *host = (id == M_FULLHOST ? msg->host : msg->host_from);
		/* full hostname */
		length = append_string(dest, left, (char *) host->data, host->length, escape);
		break;
	}
	case M_HOST_FROM:
	case M_HOST: {
		/* host */
		struct ol_string *host = (id == M_HOST ? msg->host : msg->host_from);
		UINT8 *p1;
		UINT8 *p2;
		int remaining;
		
		p1 = memchr(host->data, '@', host->length);
		if (p1) 
			p1++; 
		else 
			p1 = host->data;
                remaining = host->length - (p1 - host->data);
		p2 = memchr(p1, '/', remaining);
		if (p2) {
			length = LIBOL_MIN((unsigned int) (p2 - p1), *left);
		}
		else {
			length = LIBOL_MIN(*left, (unsigned int) (host->length - (p1 - host->data)));
		}
		length = append_string(dest, left, (char *) p1, length, escape);
		break;
	}
	case M_PROGRAM: {
		/* program */
		if (msg->program) {
			length = append_string(dest, left, (char *) msg->program->data, msg->program->length, escape);
		}
		break;
	}
	case M_FULLDATE_RECVD:
	case M_ISODATE_RECVD:
	case M_WEEKDAY_RECVD:
	case M_DATE_RECVD:
	case M_YEAR_RECVD:
	case M_MONTH_RECVD:
	case M_DAY_RECVD:
	case M_HOUR_RECVD:
	case M_MIN_RECVD:
	case M_SEC_RECVD:
	case M_TZOFFSET_RECVD:
	case M_TZ_RECVD:
	case M_UNIXTIME_RECVD:

	case M_FULLDATE_STAMP:
	case M_ISODATE_STAMP:
	case M_WEEKDAY_STAMP:
	case M_DATE_STAMP:
	case M_YEAR_STAMP:
	case M_MONTH_STAMP:
	case M_DAY_STAMP:
	case M_HOUR_STAMP:
	case M_MIN_STAMP:
	case M_SEC_STAMP:
	case M_TZOFFSET_STAMP:
	case M_TZ_STAMP:
	case M_UNIXTIME_STAMP:

	case M_FULLDATE:
	case M_ISODATE:
	case M_WEEKDAY:
	case M_DATE:
	case M_YEAR:
	case M_MONTH:
	case M_DAY: 
	case M_HOUR:
	case M_MIN:
	case M_SEC:
	case M_TZOFFSET:
	case M_TZ:
	case M_UNIXTIME: {
		/* year, month, day */
		struct tm *tm;
		time_t unixtime;

	       	switch(id) {
	       	case M_FULLDATE_RECVD:
	       	case M_ISODATE_RECVD: 
	       	case M_WEEKDAY_RECVD: 
	       	case M_DATE_RECVD:    
	       	case M_YEAR_RECVD:    
	       	case M_MONTH_RECVD:   
	       	case M_DAY_RECVD:     
	       	case M_HOUR_RECVD:    
	       	case M_MIN_RECVD:     
	       	case M_SEC_RECVD:     
		case M_TZOFFSET_RECVD:
		case M_TZ_RECVD:
	       	case M_UNIXTIME_RECVD:     
			unixtime = msg->recvd;
			break;
		case M_FULLDATE_STAMP:
		case M_ISODATE_STAMP: 
		case M_WEEKDAY_STAMP: 
		case M_DATE_STAMP:    
		case M_YEAR_STAMP:    
		case M_MONTH_STAMP:   
		case M_DAY_STAMP:     
		case M_HOUR_STAMP:    
		case M_MIN_STAMP:     
		case M_SEC_STAMP:     
		case M_TZOFFSET_STAMP:
		case M_TZ_STAMP:
		case M_UNIXTIME_STAMP:     
			unixtime = msg->stamp;
	                break;
		default:
 		        if (cfg->use_time_recvd)
				unixtime = msg->recvd;
 		        else
				unixtime = msg->stamp;
 		        break;
		}
		
		tm = localtime(&unixtime);

		switch (id) {
		case M_WEEKDAY:
 	        case M_WEEKDAY_RECVD:
 	        case M_WEEKDAY_STAMP:
	                length = strftime(*dest, *left - 1, "%a", tm);			
			break;
		case M_YEAR:
		case M_YEAR_RECVD:
 	        case M_YEAR_STAMP:
			length = snprintf(*dest, *left, "%04d", tm->tm_year + 1900);
			break;
		case M_MONTH:
 	        case M_MONTH_RECVD:
 	        case M_MONTH_STAMP:
			length = snprintf(*dest, *left, "%02d", tm->tm_mon + 1);
			break;
		case M_DAY:
 		case M_DAY_RECVD:
 	        case M_DAY_STAMP:  
			length = snprintf(*dest, *left, "%02d", tm->tm_mday);
			break;
		case M_HOUR:
		case M_HOUR_RECVD:
 	        case M_HOUR_STAMP:
			length = snprintf(*dest, *left, "%02d", tm->tm_hour);
			break;
		case M_MIN:
 		case M_MIN_RECVD:
		case M_MIN_STAMP:
			length = snprintf(*dest, *left, "%02d", tm->tm_min);
			break;
		case M_SEC:
		case M_SEC_RECVD:
		case M_SEC_STAMP:
			length = snprintf(*dest, *left, "%02d", tm->tm_sec);
			break;
		case M_ISODATE:
 		case M_ISODATE_RECVD:
		case M_ISODATE_STAMP:
	                length = strftime(*dest, *left - 1, "%Y-%m-%dT%H:%M:%S", tm);
	                length = length + format_tzofs((*dest) + length, *left - length - 1, unixtime, tm);
	                break;
	        case M_FULLDATE:
 	        case M_FULLDATE_RECVD:
 	        case M_FULLDATE_STAMP:
	                length = strftime(*dest, *left - 1, "%Y %h %e %H:%M:%S", tm);
	        	break;
	        case M_DATE:
 	        case M_DATE_RECVD:
 	        case M_DATE_STAMP:
	                length = strftime(*dest, *left - 1, "%h %e %H:%M:%S", tm);
	                break;
	        case M_TZOFFSET:
 	        case M_TZOFFSET_RECVD:
 	        case M_TZOFFSET_STAMP:
 	        	length = format_tzofs(*dest, *left - 1, unixtime, tm);
			break;
	        case M_TZ:
 	        case M_TZ_RECVD:
 	        case M_TZ_STAMP:
			length = strftime(*dest, *left -1, "%Z", tm);
			break;
	        case M_UNIXTIME:
 	        case M_UNIXTIME_RECVD:
 	        case M_UNIXTIME_STAMP:
			length = snprintf(*dest, *left, "%ld", (long) unixtime);
	                break;
		}
		break;
	}
	case M_MESSAGE: {
		/* message */
		length = append_string(dest, left, (char *) msg->msg->data, msg->msg->length, escape);
		break;
	}
	case M_MSGONLY: {
		char *colon;
		int ofs;
		
		colon = memchr(msg->msg->data, ':', msg->msg->length);
		
		if (!colon) {
			ofs = 0;
		}
		else {
			ofs = (colon - (char *) msg->msg->data) + 2;
			if (ofs > msg->msg->length)
				ofs = msg->msg->length;
		}
		length = append_string(dest, left, (char *) msg->msg->data + ofs, msg->msg->length - ofs, escape);
		break;
	}
	default:
		break;
	}
	if (length < 0 || (unsigned int) length > *left) 
		length = *left;

	*left -= length;
	*dest += length;
}

#define GPERF_MACROS

#ifndef GPERF_MACROS

struct macro_def {
	char *name;
	int id;
	int len;
};

static struct macro_def macros[] = {
	{ "FACILITY", M_FACILITY },
	{ "PRIORITY", M_LEVEL },
	{ "LEVEL", M_LEVEL },
	{ "TAG", M_TAG },
	{ "PRI", M_PRI },

	{ "DATE", M_DATE },
	{ "FULLDATE", M_FULLDATE },
	{ "ISODATE", M_ISODATE },
	{ "YEAR", M_YEAR },
	{ "MONTH", M_MONTH },
	{ "DAY", M_DAY },
	{ "HOUR", M_HOUR },
	{ "MIN", M_MIN },
	{ "SEC", M_SEC },
	{ "WEEKDAY", M_WEEKDAY },
	{ "UNIXTIME", M_UNIXTIME },
	{ "TZOFFSET", M_TZOFFSET },
	{ "TZ", M_TZ },

	{ "R_DATE", M_DATE_RECVD },
	{ "R_FULLDATE", M_FULLDATE_RECVD },
	{ "R_ISODATE", M_ISODATE_RECVD },
	{ "R_YEAR", M_YEAR_RECVD },
	{ "R_MONTH", M_MONTH_RECVD },
	{ "R_DAY", M_DAY_RECVD },
	{ "R_HOUR", M_HOUR_RECVD },
	{ "R_MIN", M_MIN_RECVD },
	{ "R_SEC", M_SEC_RECVD },
	{ "R_WEEKDAY", M_WEEKDAY_RECVD },
	{ "R_UNIXTIME", M_UNIXTIME_RECVD },
	{ "R_TZOFFSET", M_TZOFFSET_RECVD },
	{ "R_TZ", M_TZ_RECVD },

	{ "S_DATE", M_DATE_STAMP },
	{ "S_FULLDATE", M_FULLDATE_STAMP },
	{ "S_ISODATE", M_ISODATE_STAMP },
	{ "S_YEAR", M_YEAR_STAMP },
	{ "S_MONTH", M_MONTH_STAMP },
	{ "S_DAY", M_DAY_STAMP },
	{ "S_HOUR", M_HOUR_STAMP },
	{ "S_MIN", M_MIN_STAMP },
	{ "S_SEC", M_SEC_STAMP },
	{ "S_WEEKDAY", M_WEEKDAY_STAMP },
	{ "S_UNIXTIME", M_UNIXTIME_STAMP },
	{ "S_TZOFFSET", M_TZOFFSET_STAMP },
	{ "S_TZ", M_TZ_STAMP },
	
	{ "HOST_FROM", M_HOST_FROM },
	{ "FULLHOST_FROM", M_FULLHOST_FROM },
	{ "HOST", M_HOST },
	{ "FULLHOST", M_FULLHOST },

	{ "PROGRAM", M_PROGRAM },
	{ "MSG", M_MESSAGE },
	{ "MSGONLY", M_MSGONLY },
	{ "MESSAGE", M_MESSAGE },
	{ "SOURCEIP", M_SOURCE_IP }
};

static int macro_cache[sizeof(macros) / sizeof(struct macro_def)];
static int macro_cache_inited = 0;

int 
cmpmacro(const void *k1, const void *k2)
{
	return strcmp(macros[*((int *) k1)].name, macros[*((int *) k2)].name);
}

void
cache_macros(void)
{
	int i;
	int num;
	
	num = sizeof(macros) / sizeof(struct macro_def);	
	for (i = 0; i < num; i++) {
		macro_cache[i] = i;
		macros[i].len = strlen(macros[i].name);
	}
	qsort(macro_cache, num, sizeof(int), cmpmacro);
}

struct macro_def *
find_macro(char *template, int template_left)
{
	int l, h, m, len, cmp;
	
	if (!macro_cache_inited) {
		cache_macros();
		macro_cache_inited = 1;
	}
	
	l = 0; 
	h = sizeof(macros) / sizeof(struct macro_def);
	while (l <= h) {
		m = (l + h) >> 1;
		len = LIBOL_MIN(template_left, macros[macro_cache[m]].len);
		
		cmp = strncmp(template, macros[macro_cache[m]].name, len);
		if (cmp == 0) {
			return &macros[macro_cache[m]];
		}
		else if (cmp < 0) {
			h = m - 1;
		}
		else if (cmp > 0) {
			l = m + 1;
		}
	}
	return NULL;
}

#else

#include "macros-gperf.c"

#endif

struct ol_string *
expand_macros(struct syslog_config *cfg, struct ol_string *template, int template_escape, struct log_info *msg)
{
	char format[cfg->log_msg_size + 1], *format_ptr = format;
	unsigned int left = sizeof(format) - 1;
	unsigned int i;

	i = 0;
	while (left && (i < template->length)) {

		if (template->data[i] == '$') {
			/* beginning of a macro */
			struct macro_def *m;
			int start;
			
			start = ++i;
			while ((template->data[i] >= 'A' && 
				template->data[i] <= 'Z') ||
			       template->data[i] == '_') 
				i++;
			m = find_macro((char *) &template->data[start], i - start);
			if (m) {
				expand_macro(cfg, m->id, template_escape, &format_ptr, &left, msg);
			}
		}
		else {
			*format_ptr = template->data[i];
			format_ptr++;
			i++;
			left--;
		}
	}
	*format_ptr = 0;
	return c_format_cstring("%z", format);
}
