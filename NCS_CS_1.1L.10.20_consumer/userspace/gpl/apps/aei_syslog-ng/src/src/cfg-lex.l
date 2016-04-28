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
 * $Id: cfg-lex.l,v 1.27.4.9 2006/08/22 07:45:22 bazsi Exp $
 *
 ***************************************************************************/
%{

#include "syslog-ng.h"
#include "utils.h"

#include <string.h>
#include <strings.h>

#include "cfg-grammar.h"

struct keyword {
	char	*kw_name;
	int	kw_token;
};

static struct keyword keywords[] = {
	/* statements */
	{ "source", 		KW_SOURCE },
	{ "destination",	KW_DESTINATION },
	{ "filter",             KW_FILTER },
	{ "log",		KW_LOG },
	{ "options",		KW_OPTIONS },

	/* option items */
	{ "flags",		KW_FLAGS },
	{ "catchall",		KW_CATCHALL },
	{ "fallback",		KW_FALLBACK },
	{ "final",		KW_FINAL },
	{ "mark",		KW_MARK_FREQ },
	{ "sync", 		KW_SYNC_FREQ },
	{ "stats",		KW_STATS_FREQ },
	{ "fsync",		KW_FSYNC },
	{ "long_hostnames",	KW_CHAIN_HOSTNAMES },
        { "chain_hostnames",    KW_CHAIN_HOSTNAMES },
        { "keep_hostname",      KW_KEEP_HOSTNAME },
        { "use_time_recvd",	KW_USE_TIME_RECVD },
        { "use_fqdn",           KW_USE_FQDN },
	{ "use_dns",		KW_USE_DNS },
	{ "size_limit",        KW_SIZE_LIMIT },
	{ "check_hostname",	KW_CHECK_HOSTNAME },
	{ "bad_hostname",	KW_BAD_HOSTNAME },
	{ "sanitize_filenames",	KW_SANITIZE_FILENAMES },
	{ "sanitize-filenames",	KW_SANITIZE_FILENAMES },
  	{ "gc_threshold",	KW_GC_BUSY_THRESHOLD },
  	{ "gc_busy_threshold",	KW_GC_BUSY_THRESHOLD },
  	{ "gc_idle_threshold",	KW_GC_IDLE_THRESHOLD },
 	{ "time_reopen",	KW_TIME_REOPEN },
 	{ "time_reap",          KW_TIME_REAP },
	{ "time_sleep",         KW_TIME_SLEEP },
	{ "log_fifo_size",	KW_LOG_FIFO_SIZE },
        { "create_dirs",        KW_CREATE_DIRS },
 	{ "localip",		KW_LOCALIP },
	{ "ip",			KW_IP },
	{ "localport",		KW_LOCALPORT },
	{ "port",		KW_PORT },
	{ "destport",		KW_DESTPORT },
	{ "owner",		KW_OWNER },
	{ "group",		KW_GROUP },
	{ "perm",		KW_PERM },
	{ "dir_owner",		KW_DIR_OWNER },
	{ "dir_group",		KW_DIR_GROUP },
        { "dir_perm",           KW_DIR_PERM },
        { "template",           KW_TEMPLATE },
        { "template-escape",	KW_TEMPLATE_ESCAPE },
        { "template_escape",	KW_TEMPLATE_ESCAPE },
 	{ "keep-alive",         KW_KEEP_ALIVE },
 	{ "keep_alive",         KW_KEEP_ALIVE },
 	{ "tcp-keep-alive",     KW_TCP_KEEP_ALIVE },
 	{ "tcp_keep_alive",     KW_TCP_KEEP_ALIVE },
	{ "max-connections",	KW_MAX_CONNECTIONS },
	{ "max_connections",	KW_MAX_CONNECTIONS },
	{ "mac",		KW_MAC },
	{ "authentication",	KW_AUTH },
	{ "encrypt",		KW_ENCRYPT },
	{ "required",		KW_REQUIRED },
	{ "allow",		KW_ALLOW },
	{ "deny",		KW_DENY },
	{ "compress",		KW_COMPRESS },
	{ "remove_if_older",	KW_REMOVE_IF_OLDER },
	{ "dns_cache",		KW_DNS_CACHE },
	{ "dns_cache_size",	KW_DNS_CACHE_SIZE },
	{ "dns_cache_expire",	KW_DNS_CACHE_EXPIRE },
	{ "dns_cache_expire_failed", KW_DNS_CACHE_EXPIRE_FAILED },
	{ "log_prefix",		KW_LOG_PREFIX },
	{ "log_msg_size",	KW_LOG_MSG_SIZE },
	{ "pad_size",		KW_PAD_SIZE },
	{ "spoof_source",	KW_SPOOF_SOURCE },
	{ "spoof-source",	KW_SPOOF_SOURCE },
        { "so_rcvbuf",          KW_SO_RCVBUF },
        
	/* source or destination items */
	{ "file",		KW_FILE },
        { "fifo",               KW_PIPE },
	{ "pipe",		KW_PIPE },
        { "internal",           KW_INTERNAL },
	{ "unix-dgram",		KW_UNIX_DGRAM },
	{ "unix_dgram",		KW_UNIX_DGRAM },
	{ "unix-stream",	KW_UNIX_STREAM },
	{ "unix_stream",	KW_UNIX_STREAM },
        { "udp",                KW_UDP },
        { "tcp",                KW_TCP },
        { "usertty", 		KW_USER },
        { "door",               KW_DOOR },
        { "sun-stream",		KW_SUN_STREAMS },
        { "sun-streams",	KW_SUN_STREAMS },
        { "sun_stream",		KW_SUN_STREAMS },
        { "sun_streams",	KW_SUN_STREAMS },
        { "program",		KW_PROGRAM },
        { "remote_control",	KW_REMCTRL },

	/* filter items */
        { "or",                 KW_OR },
	{ "and",                KW_AND },
        { "not",                KW_NOT },
	{ "level",              KW_LEVEL },
	{ "priority",           KW_LEVEL },
	{ "facility",           KW_FACILITY },
	{ "program",		KW_PROGRAM },
	{ "netmask",            KW_NETMASK },
        { "host",               KW_HOST },
        { "match",		KW_MATCH },

	/* on/off switches */
	{ "yes",		KW_YES },
	{ "on",			KW_YES },
	{ "no",			KW_NO },
	{ "off", 		KW_NO }
};

#define YY_NO_UNPUT   1

#define MAX_REGEXP_LEN	1024

int linenum = 1;
int lex_filter_params = 0;
char buf[MAX_REGEXP_LEN];
char *str;

static int check_reserved_words(char *token);
static void append_string(int length, char *str);
static void append_char(char c);

%}

white	[ \t]
digit	[0-9]
alpha		[a-zA-Z]
alphanum	[a-zA-Z0-9]
word	[^ \#'"\(\)\{\}\\;\n\t,|\.]

%x string
%x qstring
%%

\#.*$                      ;
\n			   { linenum++; }
{white}+		   ;
\.\.                       { return DOTDOT; }
0x{digit}+ 		   { yylval.num = strtol(yytext, NULL, 16); return NUMBER; }
0{digit}+		   { yylval.num = strtol(yytext, NULL, 8); return NUMBER; }
(-|\+)?{digit}+		   { yylval.num = strtol(yytext, NULL, 10); return NUMBER; }
({word}+(\.)?)*{word}+ 	   { return check_reserved_words(yytext); }
\(	      		   { return '('; }
\)			   { return ')'; }
\;			   { return ';'; }
\{			   { return '{'; }
\}			   { return '}'; }
\|			   { return '|'; }
\,			   ;

\"                         {
				str = buf;
				/* yy_push_state(string);*/
				BEGIN(string);
			   }
\'			   {
				str = buf;
				BEGIN(qstring);
			   }
<string>\\a		   { append_char(7); }
<string>\\n	   	   { append_char(10); }
<string>\\r		   { append_char(13); }
<string>\\t		   { append_char(9); }
<string>\\v		   { append_char(11); }
<string>\\[^anrtv]	   { append_string(1, yytext + 1); }
<string>\"		   { 
				BEGIN(INITIAL);
				/* yy_pop_state();*/
				yylval.cptr = strdup(buf);
				return STRING; 
		           }
<string>[^"\\]+		   { append_string(strlen(yytext), yytext); }
<qstring>[^']+		   { append_string(strlen(yytext), yytext); }
<qstring>\'		   { 
				BEGIN(INITIAL);
				yylval.cptr = strdup(buf);
				return STRING;
			   }

%%

int lex_init(FILE *file)
{
	yyrestart(file);
	linenum = 1;
	return 0;
}

static int check_reserved_words(char *token)
{
	int i;
	
	for (i = 0; i < (sizeof(keywords) / sizeof(struct keyword)); i++) {
		if (strcmp(keywords[i].kw_name, token) == 0) {
			return keywords[i].kw_token;
		}
	}
	yylval.cptr = strdup(token);
	return IDENTIFIER;
}

static void append_string(int length, char *s)
{
	int to_copy = LIBOL_MIN(MAX_REGEXP_LEN - (str - buf) - 1, length);

	memcpy(str, s, to_copy);
	str += to_copy;
	*str = 0;
}

static void append_char(char c)
{
      	*str = c;
	str++;
	*str = 0;
}
