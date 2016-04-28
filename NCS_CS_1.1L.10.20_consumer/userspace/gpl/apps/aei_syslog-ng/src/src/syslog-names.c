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
 * $Id: syslog-names.c,v 1.8.4.1 2004/03/10 18:43:28 bazsi Exp $
 *
 ***************************************************************************/

#include "syslog-names.h"
#include "syslog-ng.h"
#include <string.h>

struct sl_name sl_levels[] = {
	{ "alert", LOG_ALERT },
	{ "crit", LOG_CRIT },
	{ "debug", LOG_DEBUG },
	{ "emerg", LOG_EMERG },
	{ "err", LOG_ERR },
	{ "error", LOG_ERR },               /* DEPRECATED */
	{ "info", LOG_INFO },
	{ "notice", LOG_NOTICE },
	{ "panic", LOG_EMERG },             /* DEPRECATED */
	{ "warning", LOG_WARNING },
	{ "warn", LOG_WARNING },            /* DEPRECATED */
	{ NULL, -1}
}; 

struct sl_name sl_facilities[] = {
#ifdef LOG_AUDIT
        { "audit", LOG_AUDIT },
        { "sat", LOG_AUDIT },
#endif
	{ "auth", LOG_AUTH },
#ifdef LOG_AUTHPRIV
	{ "authpriv", LOG_AUTHPRIV },
#endif
	{ "cron", LOG_CRON },
	{ "daemon", LOG_DAEMON },
#ifdef LOG_FTP
	{ "ftp", LOG_FTP },
#endif
	{ "kern", LOG_KERN },
	{ "lpr", LOG_LPR },
	{ "mail", LOG_MAIL },
	{ "news", LOG_NEWS },
	{ "security", LOG_AUTH },           /* DEPRECATED */
	{ "syslog", LOG_SYSLOG },
	{ "user", LOG_USER },
	{ "uucp", LOG_UUCP },
	{ "local0", LOG_LOCAL0 },
	{ "local1", LOG_LOCAL1 },
	{ "local2", LOG_LOCAL2 },
	{ "local3", LOG_LOCAL3 },
	{ "local4", LOG_LOCAL4 },
	{ "local5", LOG_LOCAL5 },
	{ "local6", LOG_LOCAL6 },
	{ "local7", LOG_LOCAL7 },
	{ NULL, -1 }
};

unsigned int syslog_lookup_name(const char *name, struct sl_name names[])
{
	int i;

	for (i = 0; names[i].name; i++) {
		if (strcasecmp(name, names[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

char *syslog_lookup_value(unsigned int value, struct sl_name names[])
{
	int i;

	for (i = 0; names[i].name; i++) {
		if (names[i].value == value) {
			return names[i].name;
		}
	}
	return NULL;
}

UINT32 syslog_make_range(UINT32 r1, UINT32 r2)
{
	UINT32 x;

	if (r1 > r2) {
		x = r2;
		r2 = r1;
		r1 = x;
	}
	return ((1 << (r2 + 1)) - 1) & ~((1 << r1) - 1);
}

