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
 * $Id: cfg-grammar.y,v 1.58.4.9 2006/08/22 07:45:21 bazsi Exp $
 *
 ***************************************************************************/

%{

#include "cfgfile.h"
#include "filters.h"
#include "syslog-names.h"
#include "format.h"
#include "afinter.h"
#include "affile.h"
#include "afsocket.h"
#include "afuser.h"
#include "afstreams.h"
#include "afprogram.h"
#include "afremctrl.h"

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

void yyerror(char *msg);
int yylex();

static struct log_source_driver *last_source_driver;
static struct log_dest_driver *last_dest_driver;
UINT8 *last_prefix = NULL;
UINT32 last_pad_size;

UINT8 *
get_last_log_prefix(void)
{
	UINT8 *res = last_prefix;
	last_prefix = NULL;
	return res;
}

%}

%union {
	UINT32 num;
	char *cptr;
	void *ptr;
	struct filter_expr_node *node;
}

/* statements */
%token	KW_SOURCE KW_DESTINATION KW_LOG KW_OPTIONS KW_FILTER

/* source & destination items */
%token	KW_INTERNAL KW_FILE KW_PIPE KW_UNIX_STREAM KW_UNIX_DGRAM KW_UDP 
%token  KW_TCP KW_USER KW_SPOOF_SOURCE KW_SO_RCVBUF
%token  KW_DOOR KW_SUN_STREAMS KW_PROGRAM KW_REMCTRL

/* option items */
%token KW_FLAGS KW_CATCHALL KW_FALLBACK KW_FINAL
%token KW_FSYNC KW_MARK_FREQ KW_SYNC_FREQ KW_STATS_FREQ KW_CHAIN_HOSTNAMES KW_KEEP_HOSTNAME KW_CHECK_HOSTNAME KW_BAD_HOSTNAME
%token KW_LOG_FIFO_SIZE KW_LOG_MSG_SIZE
%token KW_TIME_REOPEN KW_TIME_REAP KW_TIME_SLEEP KW_USE_TIME_RECVD
%token KW_USE_DNS KW_USE_FQDN KW_GC_BUSY_THRESHOLD 
%token KW_GC_IDLE_THRESHOLD KW_CREATE_DIRS KW_SANITIZE_FILENAMES
%token KW_DIR_OWNER KW_DIR_GROUP KW_DIR_PERM KW_TEMPLATE KW_TEMPLATE_ESCAPE
%token KW_OWNER KW_GROUP KW_PERM KW_KEEP_ALIVE KW_TCP_KEEP_ALIVE KW_MAX_CONNECTIONS
%token KW_LOCALIP KW_IP KW_LOCALPORT KW_PORT KW_DESTPORT 
%token KW_COMPRESS KW_MAC KW_AUTH KW_ENCRYPT
%token KW_DNS_CACHE KW_DNS_CACHE_SIZE
%token KW_DNS_CACHE_EXPIRE KW_DNS_CACHE_EXPIRE_FAILED
%token KW_REMOVE_IF_OLDER KW_LOG_PREFIX KW_PAD_SIZE
%token KW_FILE_SIZE_LIMIT KW_SIZE_LIMIT
%token KW_STOP_ON_FULL


/* filter items*/
%token KW_FACILITY KW_LEVEL KW_NETMASK KW_HOST KW_MATCH

/* yes/no switches */
%token KW_YES KW_NO

/* tripleoption */
%token KW_REQUIRED KW_ALLOW KW_DENY

%token  DOTDOT
%token	<cptr> IDENTIFIER
%token	<num>  NUMBER
%token	<cptr> STRING

%left	KW_OR
%left	KW_AND
%left   KW_NOT

%type	<ptr> source_stmt
%type	<ptr> dest_stmt
%type	<ptr> log_stmt
%type	<ptr> options_stmt

%type	<ptr> source_items
%type	<ptr> source_item
%type   <ptr> source_afinter
%type	<ptr> source_affile
%type	<ptr> source_afsocket
%type	<ptr> source_afunix_dgram_params
%type	<ptr> source_afunix_stream_params
%type	<ptr> source_afinet_udp_params
%type	<ptr> source_afinet_tcp_params
%type   <ptr> source_afsocket_stream_params
%type	<ptr> source_afstreams
%type	<ptr> source_afstreams_params

%type	<ptr> dest_items
%type	<ptr> dest_item
%type   <ptr> dest_affile
%type	<ptr> dest_affile_params
%type   <ptr> dest_afpipe
%type   <ptr> dest_afpipe_params
%type	<ptr> dest_afsocket
%type	<ptr> dest_afunix_dgram_params
%type	<ptr> dest_afunix_stream_params
%type	<ptr> dest_afinet_udp_params
%type	<ptr> dest_afinet_tcp_params
%type   <ptr> dest_afuser
%type   <ptr> dest_afuser_params
%type   <ptr> dest_afprogram
%type   <ptr> dest_afprogram_params
%type   <ptr> dest_afremctrl

%type	<ptr> log_items
%type	<ptr> log_item
%type	<num> log_flags
%type   <num> log_flags_items
%type	<num> log_flags_item

%type	<ptr> options_items
%type	<ptr> options_item

%type	<ptr> filter_stmt
%type	<node> filter_expr
%type	<node> filter_simple_expr

%type   <num> filter_fac_list
%type   <num> filter_fac
%type	<num> filter_level_list
%type   <num> filter_level

%type	<num> yesno
%type	<num> tripleoption

%type	<cptr> string

%%

start   
        : stmts
	;

stmts   
        : stmt ';' stmts
	|	
	;

stmt    
        : KW_SOURCE source_stmt			{ add_source_group($2); }
	| KW_DESTINATION dest_stmt		{ add_dest_group($2); }
	| KW_LOG log_stmt			{ add_log_connection($2); }
	| KW_FILTER filter_stmt			{ add_filter_rule($2); }
	| KW_OPTIONS options_stmt		{  }
	;

source_stmt
	: string '{' source_items '}'		{ $$ = make_source_group($1, $3); free($1); }        
	;

dest_stmt
        : string '{' dest_items '}'		{ $$ = make_dest_group($1, $3); free($1); }
	;

log_stmt
        : '{' log_items log_flags '}'		{ $$ = make_log_connection($2, $3); }
	;

options_stmt
        : '{' options_items '}'			{ $$ = NULL; }
	;

source_items
        : source_item ';' source_items		{ append_source_driver($1, $3); $$ = $1; }
	|					{ $$ = NULL; }
	;

source_item
  	: source_afinter			{ $$ = $1; }
	| source_affile				{ $$ = $1; }
	| source_afsocket			{ $$ = $1; }
	| source_afstreams			{ $$ = $1; }
	;

source_afinter
	: KW_INTERNAL '(' ')'			{ $$ = make_afinter_source(); }
	;

source_affile
	: KW_FILE '(' string source_affile_options ')'		
          { 
        	$$ = make_affile_source($3, 0, get_last_log_prefix(), last_pad_size); 
		free($3); 
	  }
	| KW_PIPE '(' string source_affile_options ')'
	  { 
		$$ = make_affile_source($3, AFFILE_PIPE, get_last_log_prefix(), last_pad_size); 
		free($3); 
	  }
	;

source_affile_options
	: source_affile_option source_affile_options
	|
	;

source_affile_option
	: KW_LOG_PREFIX '(' string ')'				{ last_prefix = (UINT8 *) $3; }
	| KW_PAD_SIZE '(' NUMBER ')'				{ last_pad_size = $3; }
	;

source_afsocket
	: KW_UNIX_DGRAM '(' source_afunix_dgram_params ')'	{ $$ = $3; }
	| KW_UNIX_STREAM '(' source_afunix_stream_params ')' 	{ $$ = $3; }
	| KW_UDP '(' source_afinet_udp_params ')'		{ $$ = $3; } 
	| KW_TCP '(' source_afinet_tcp_params ')'		{ $$ = $3; } 
	;
 	
source_afunix_dgram_params
	: string 
	  { 
	    last_source_driver = make_afunix_source(
		&make_unix_address_c($1)->super, 
		AFSOCKET_DGRAM); 
	    free($1); 
	  }
	  source_afunix_options			{ $$ = last_source_driver; }
	;
	
source_afunix_stream_params
	: string 
	  { 
	    last_source_driver = make_afunix_source(
		&make_unix_address_c($1)->super,
		AFSOCKET_STREAM | AFSOCKET_KEEP_ALIVE);
	    free($1);
	  }
	  source_afunix_options			{ $$ = last_source_driver; }
	;

/* options are common between dgram & stream */
source_afunix_options
	: source_afunix_option source_afunix_options
	|
	;

source_afunix_option
	: KW_OWNER '(' string ')'		{ afunix_set_owner(last_source_driver, $3); free($3); }
	| KW_GROUP '(' string ')'		{ afunix_set_group(last_source_driver, $3); free($3); }
	| KW_PERM '(' NUMBER ')'		{ afunix_set_perm(last_source_driver, $3); }
	| source_afsocket_stream_params		{}
	; 

source_afinet_udp_params
        : 
          { 
	    last_source_driver = make_afinet_source(
			&make_inet_address_c("0.0.0.0", "514")->super, 
			AFSOCKET_DGRAM);
	  }
	  source_afinet_udp_options		{ $$ = last_source_driver; }
	;

source_afinet_udp_options
	: source_afinet_udp_option source_afinet_udp_options
	|
	;

source_afinet_udp_option
	: source_afinet_option
	| KW_LOCALPORT '(' string ')'		{ afinet_src_set_localport(last_source_driver, 0, $3 ,"udp"); free($3); }
	| KW_PORT '(' string ')'		{ afinet_src_set_localport(last_source_driver, 0, $3, "udp"); free($3); }
	| KW_SO_RCVBUF '(' NUMBER ')'		{ afinet_src_set_so_rcvbuf(last_source_driver, $3); }
	;

source_afinet_option
	: KW_LOCALIP '(' string ')'		{ afinet_src_set_localip(last_source_driver, $3); free($3); }
	| KW_LOCALPORT '(' NUMBER ')'		{ afinet_src_set_localport(last_source_driver, $3, NULL, NULL); }
	| KW_PORT '(' NUMBER ')'		{ afinet_src_set_localport(last_source_driver, $3, NULL, NULL); }
	| KW_IP '(' string ')'			{ afinet_src_set_localip(last_source_driver, $3); free($3); }
	;

source_afinet_tcp_params
	: 
	  { 
	    last_source_driver = make_afinet_source(
			&make_inet_address_c("0.0.0.0", "514")->super,
			AFSOCKET_STREAM);
	  }
	  source_afinet_tcp_options	{ $$ = last_source_driver; }
	;

source_afinet_tcp_options
	: source_afinet_tcp_option source_afinet_tcp_options
	|
	;

source_afinet_tcp_option
        : source_afinet_option
        | KW_TCP_KEEP_ALIVE '(' yesno ')'	{ afinet_src_set_tcp_keepalive(last_source_driver, $3); }
	| KW_LOCALPORT '(' string ')'		{ afinet_src_set_localport(last_source_driver, 0, $3, "tcp"); free($3); }
	| KW_PORT '(' string ')'		{ afinet_src_set_localport(last_source_driver, 0, $3, "tcp"); free($3); }
	| KW_AUTH '(' tripleoption ')'		{ afinet_src_set_auth(last_source_driver, $3); }
	| KW_MAC '(' tripleoption ')'		{ afinet_src_set_mac(last_source_driver, $3); }
	| KW_ENCRYPT '(' tripleoption ')'	{ afinet_src_set_encrypt(last_source_driver, $3); }
	| source_afsocket_stream_params		{}
	;

source_afsocket_stream_params
	: KW_KEEP_ALIVE '(' yesno ')'		{ afsocket_src_set_keep_alive(last_source_driver, $3); }
	| KW_MAX_CONNECTIONS '(' NUMBER ')'	{ afsocket_src_set_max_connections(last_source_driver, $3); }
	;
	
source_afstreams
	: KW_SUN_STREAMS '(' source_afstreams_params ')'	{ $$ = $3; }
	;
	
source_afstreams_params
	: string
	  { 
	    last_source_driver = make_afstreams_source($1); 
	    free($1); 
	  }
	  source_afstreams_options		{ $$ = last_source_driver; }
	;
	
source_afstreams_options
	: source_afstreams_option source_afstreams_options
	|
	;

source_afstreams_option
	: KW_DOOR '(' string ')'		{ afstreams_set_sundoor(last_source_driver, $3); free($3); }
	;

dest_items
	: dest_item ';' dest_items		{ append_dest_driver($1, $3); $$ = $1; }
	|					{ $$ = NULL; }
	;

dest_item
	: dest_affile				{ $$ = $1; }
	| dest_afpipe				{ $$ = $1; }
	| dest_afsocket				{ $$ = $1; }
	| dest_afuser				{ $$ = $1; }
	| dest_afprogram			{ $$ = $1; }
	| dest_afremctrl			{ $$ = $1; }
	;

dest_item_option
	: KW_LOG_FIFO_SIZE '(' NUMBER ')'	{ last_dest_driver->log_fifo_size = $3; }
	;

dest_affile
	: KW_FILE '(' dest_affile_params ')'	{ $$ = $3; }
	;

dest_affile_params
	: string 
	  { 
	    last_dest_driver = make_affile_dest($1, 0); 
	    free($1); 
	  } 
	  dest_affile_options
	  					{ $$ = last_dest_driver; }
	;

dest_affile_options
	: dest_affile_option dest_affile_options		
        |
	;	

dest_affile_option
	: dest_item_option
	| KW_SYNC_FREQ '(' NUMBER ')'		{ affile_set_syncfreq(last_dest_driver, $3); }
	| KW_COMPRESS '(' yesno ')'		{ affile_set_compress(last_dest_driver, $3); }
	| KW_ENCRYPT '(' yesno ')'		{ affile_set_encrypt(last_dest_driver, $3); }
	| KW_OWNER '(' string ')'		{ affile_set_owner(last_dest_driver, $3); free($3); }
	| KW_GROUP '(' string ')'		{ affile_set_group(last_dest_driver, $3); free($3); }
	| KW_PERM '(' NUMBER ')'		{ affile_set_perm(last_dest_driver, $3); }
	| KW_DIR_OWNER '(' string ')'		{ affile_set_dir_owner(last_dest_driver, $3); free($3); }
	| KW_DIR_GROUP '(' string ')'		{ affile_set_dir_group(last_dest_driver, $3); free($3); }
	| KW_DIR_PERM '(' NUMBER ')'		{ affile_set_dir_perm(last_dest_driver, $3); }
	| KW_CREATE_DIRS '(' yesno ')'		{ affile_set_create_dirs(last_dest_driver, $3); }
	| KW_REMOVE_IF_OLDER '(' NUMBER ')'	{ affile_set_remove_if_older(last_dest_driver, $3); }
	| KW_TEMPLATE '(' string ')'            { affile_set_file_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ affile_set_template_escape(last_dest_driver, $3); }
	| KW_FSYNC '(' yesno ')'		{ affile_set_fsync(last_dest_driver, $3); }
	| KW_SIZE_LIMIT '(' NUMBER ')'	{ affile_dd_set_file_size_limit(last_dest_driver, $3); }
	| KW_STOP_ON_FULL '(' yesno ')'		{ affile_set_stop_on_full(last_dest_driver, $3); }
	;

dest_afpipe
	: KW_PIPE '(' dest_afpipe_params ')'    { $$ = $3; }
	;

dest_afpipe_params
	: string 
	  { 
	    last_dest_driver = make_affile_dest($1, AFFILE_NO_EXPAND | AFFILE_PIPE);
	    affile_set_syncfreq(last_dest_driver, 0);
	    free($1); 
	  } 
	  dest_afpipe_options                   { $$ = last_dest_driver; }
	;

dest_afpipe_options
	: dest_afpipe_option dest_afpipe_options
	|
	;

dest_afpipe_option
	: KW_OWNER '(' string ')'		{ affile_set_owner(last_dest_driver, $3); free($3); }
	| KW_GROUP '(' string ')'		{ affile_set_group(last_dest_driver, $3); free($3); }
	| KW_PERM '(' NUMBER ')'		{ affile_set_perm(last_dest_driver, $3); }
	| KW_TEMPLATE '(' string ')'            { affile_set_file_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ affile_set_template_escape(last_dest_driver, $3); }
	;


dest_afsocket
	: KW_UNIX_DGRAM '(' dest_afunix_dgram_params ')'	{ $$ = $3; }
	| KW_UNIX_STREAM '(' dest_afunix_stream_params ')'	{ $$ = $3; }
	| KW_UDP '(' dest_afinet_udp_params ')'			{ $$ = $3; }
	| KW_TCP '(' dest_afinet_tcp_params ')'			{ $$ = $3; } 
	;

/* options are common between dgram & stream */
dest_afunix_option
	: KW_TEMPLATE '(' string ')'            { afunix_dest_set_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ afunix_dest_set_template_escape(last_dest_driver, $3); }
	; 

dest_afunix_options
	: dest_afunix_options dest_afunix_option
	|
	;

dest_afunix_dgram_params
	: string				{ last_dest_driver = make_afunix_dest(
							&make_unix_address_c($1)->super, 
							AFSOCKET_DGRAM);
							free($1);
						}
	dest_afunix_options			{ $$ = last_dest_driver; }
	;

dest_afunix_stream_params
	: string				{ last_dest_driver = make_afunix_dest(
							&make_unix_address_c($1)->super, 
							AFSOCKET_STREAM);
							free($1);
						}
	dest_afunix_options			{ $$ = last_dest_driver; }
	;

dest_afinet_udp_params
	: string 	
	  { 
	    last_dest_driver = make_afinet_dest(
			&make_inet_address_c($1, "514")->super, 
			AFSOCKET_DGRAM);
	    free($1);
	  }
	  dest_afinet_udp_options		{ $$ = last_dest_driver; }
	;

dest_afinet_udp_options
        : dest_afinet_udp_options dest_afinet_udp_option
	|
	;

dest_afinet_option
	: dest_item_option
	| KW_LOCALIP '(' string ')'		{ afinet_dest_set_localip(last_dest_driver, $3); free($3); }
	| KW_LOCALPORT '(' NUMBER ')'		{ afinet_dest_set_localport(last_dest_driver, $3, NULL, NULL); }
	| KW_PORT '(' NUMBER ')'		{ afinet_dest_set_destport(last_dest_driver, $3, NULL, NULL); }
	| KW_DESTPORT '(' NUMBER ')'		{ afinet_dest_set_destport(last_dest_driver, $3, NULL, NULL); }
	| KW_TEMPLATE '(' string ')'            { afinet_dest_set_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ afinet_dest_set_template_escape(last_dest_driver, $3); }
	;

dest_afinet_udp_option
	: dest_afinet_option
	| KW_LOCALPORT '(' string ')'		{ afinet_dest_set_localport(last_dest_driver, 0, $3, "udp"); free($3); }
	| KW_PORT '(' string ')'		{ afinet_dest_set_destport(last_dest_driver, 0, $3, "udp"); free($3); }
	| KW_DESTPORT '(' string ')'		{ afinet_dest_set_destport(last_dest_driver, 0, $3, "udp"); free($3); }
	| KW_SPOOF_SOURCE '(' yesno ')'		{ afinet_dest_set_spoof_source(last_dest_driver, $3); }
	;

dest_afinet_tcp_params
	: string 	
	  { 
	    last_dest_driver = make_afinet_dest(
			&make_inet_address_c($1, "514")->super, 
			AFSOCKET_STREAM); 
	    free($1);
	  }
	  dest_afinet_tcp_options		{ $$ = last_dest_driver; }
	;

dest_afinet_tcp_options
	: dest_afinet_tcp_options dest_afinet_tcp_option
	|
	;

dest_afinet_tcp_option
	: dest_afinet_option
        | KW_TCP_KEEP_ALIVE '(' yesno ')'	{ afinet_dest_set_tcp_keepalive(last_dest_driver, $3); }
	| KW_LOCALPORT '(' string ')'		{ afinet_dest_set_localport(last_dest_driver, 0, $3, "tcp"); free($3); }
	| KW_PORT '(' string ')'		{ afinet_dest_set_destport(last_dest_driver, 0, $3, "tcp"); free($3); }
	| KW_DESTPORT '(' string ')'		{ afinet_dest_set_destport(last_dest_driver, 0, $3, "tcp"); free($3); }
	| KW_MAC '(' yesno ')'
	| KW_AUTH '(' yesno ')'
	| KW_ENCRYPT '(' yesno ')'
	| KW_SYNC_FREQ '(' NUMBER ')'		{ afinet_dest_set_syncfreq(last_dest_driver, $3); }
	;

dest_afuser
	: KW_USER '(' dest_afuser_params ')'	{ $$ = $3; }
	;

dest_afuser_params
	: string				{ last_dest_driver = make_afuser_dest($1); free($1); }
	dest_afuser_options			{ $$ = last_dest_driver; }
	;

dest_afuser_option
	: KW_TEMPLATE '(' string ')'            { afuser_dest_set_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ afuser_dest_set_template_escape(last_dest_driver, $3); }
	; 

dest_afuser_options
	: dest_afuser_options dest_afuser_option
	|
	;

dest_afprogram
	: KW_PROGRAM '(' dest_afprogram_params ')'	{ $$ = $3; }
	;

dest_afprogram_params
	: string				{ last_dest_driver = make_afprogram_dest($1); free($1); }
	dest_afprogram_options			{ $$ = last_dest_driver; }
	;

dest_afprogram_option
	: KW_TEMPLATE '(' string ')'            { afprogram_dest_set_template(last_dest_driver, $3); free($3); }
	| KW_TEMPLATE_ESCAPE '(' yesno ')'	{ afprogram_dest_set_template_escape(last_dest_driver, $3); }
	; 

dest_afprogram_options
	: dest_afprogram_options dest_afprogram_option
	|
	;

dest_afremctrl
	: KW_REMCTRL '(' ')'			{ $$ = make_afremctrl_dest(); }
	;

log_items
	: log_item ';' log_items		{ append_endpoint_info($1, $3); $$ = $1; }
	|					{ $$ = NULL; }
	;

log_item
	: KW_SOURCE '(' string ')'		{ $$ = make_log_endpoint_info(EP_SOURCE, $3); free($3); }
	| KW_FILTER '(' string ')'		{ $$ = make_log_endpoint_info(EP_FILTER, $3); free($3); }
	| KW_DESTINATION '(' string ')'		{ $$ = make_log_endpoint_info(EP_DESTINATION, $3); free($3); }
	;

log_flags
	: KW_FLAGS '(' log_flags_items ')' ';'	{ $$ = $3; }
	|					{ $$ = 0; }
	;


log_flags_items
	: log_flags_item log_flags_items	{ $$ |= $2; }
	|					{ $$ = 0; }
	;

log_flags_item
	: KW_CATCHALL				{ $$ = LOG_CONN_CATCHALL; }
	| KW_FALLBACK				{ $$ = LOG_CONN_FALLBACK; }
	| KW_FINAL				{ $$ = LOG_CONN_FINAL; }
	;

options_items
	: options_item ';' options_items	{ $$ = $1; }
	|					{ $$ = NULL; }
	;

options_item
	: KW_MARK_FREQ '(' NUMBER ')'		{ configuration->mark_freq = $3; }
	| KW_SYNC_FREQ '(' NUMBER ')'		{ configuration->sync_freq = $3; }
	| KW_STATS_FREQ '(' NUMBER ')'		{ configuration->stats_freq = $3; }
	| KW_CHAIN_HOSTNAMES '(' yesno ')'	{ configuration->chain_hostnames = $3; }
	| KW_KEEP_HOSTNAME '(' yesno ')'	{ configuration->keep_hostname = $3; }
	| KW_CHECK_HOSTNAME '(' yesno ')'	{ configuration->check_hostname = $3; }
	| KW_BAD_HOSTNAME '(' STRING ')'	{ cfg_set_bad_hostname($3); }
	| KW_USE_TIME_RECVD '(' yesno ')'	{ configuration->use_time_recvd = $3; }
	| KW_USE_FQDN '(' yesno ')'		{ configuration->use_fqdn = $3; }
	| KW_USE_DNS '(' yesno ')'		{ configuration->use_dns = $3; }
	| KW_TIME_REOPEN '(' NUMBER ')'		{ configuration->time_reopen = $3; }
	| KW_TIME_SLEEP '(' NUMBER ')'		{ configuration->time_sleep = $3; }
	| KW_TIME_REAP '(' NUMBER ')'		{ configuration->time_reap = $3; }
	| KW_LOG_FIFO_SIZE '(' NUMBER ')'	{ configuration->log_fifo_size = $3; }
	| KW_GC_BUSY_THRESHOLD '(' NUMBER ')' 	{ configuration->gc_busy_threshold = $3; }
	| KW_GC_IDLE_THRESHOLD '(' NUMBER ')'	{ configuration->gc_idle_threshold = $3; }
	| KW_CREATE_DIRS '(' yesno ')'		{ configuration->create_dirs = $3; }
	| KW_SANITIZE_FILENAMES '(' yesno ')'	{ configuration->sanitize_filenames = $3; }
	| KW_OWNER '(' string ')'		{ cfg_set_owner($3); free($3); }
	| KW_GROUP '(' string ')'		{ cfg_set_group($3); free($3); }
	| KW_PERM '(' NUMBER ')'		{ cfg_set_perm($3); }
	| KW_DIR_OWNER '(' string ')'		{ cfg_set_dir_owner($3); free($3); }
	| KW_DIR_GROUP '(' string ')'		{ cfg_set_dir_group($3); free($3); }
	| KW_DIR_PERM '(' NUMBER ')'		{ cfg_set_dir_perm($3); }
	| KW_DNS_CACHE '(' yesno ')' 		{ configuration->dns_cache = $3; }
	| KW_DNS_CACHE_SIZE '(' NUMBER ')'	{ configuration->dns_cache_size = $3; }
	| KW_DNS_CACHE_EXPIRE '(' NUMBER ')'	{ configuration->dns_cache_expire = $3; }
	| KW_DNS_CACHE_EXPIRE_FAILED '(' NUMBER ')'
			  			{ configuration->dns_cache_expire_failed = $3; }
	| KW_LOG_MSG_SIZE '(' NUMBER ')'	{ configuration->log_msg_size = $3; }
	;

filter_stmt
	: string '{' filter_expr ';' '}'	{ $$ = make_filter_rule($1, $3); free($1); }
	;

filter_expr
	: filter_simple_expr			{ $$ = $1; if (!$1) return 1; }
        | KW_NOT filter_expr			{ $2->comp = !($2->comp); $$ = $2; }
	| filter_expr KW_OR filter_expr		{ $$ = make_filter_or($1, $3); }
	| filter_expr KW_AND filter_expr	{ $$ = make_filter_and($1, $3); }
	| '(' filter_expr ')'			{ $$ = $2; }
	;

filter_simple_expr
	: KW_FACILITY '(' filter_fac_list ')'	{ $$ = make_filter_facility($3);  }
	| KW_FACILITY '(' NUMBER ')'		{ $$ = make_filter_facility(0x80000000 | $3); }
	| KW_LEVEL '(' filter_level_list ')' 	{ $$ = make_filter_level($3); }
	| KW_PROGRAM '(' string ')'		{ $$ = make_filter_prog($3); free($3); }
	| KW_NETMASK '(' string ')'             { $$ = make_filter_netmask($3); free($3); }
	| KW_HOST '(' string ')'		{ $$ = make_filter_host($3); free($3); }	
	| KW_MATCH '(' string ')'		{ $$ = make_filter_match($3); free($3); }
	| KW_FILTER '(' string ')'		{ $$ = make_filter_call($3); free($3); }
	;

filter_fac_list
	: filter_fac filter_fac_list		{ $$ = $1 + $2; }
	| filter_fac				{ $$ = $1; }
	;

filter_fac
	: IDENTIFIER				
	  { 
	    int n = syslog_lookup_facility($1);
	    if (n == -1)
	      {
	        werror("Warning: Unknown facility %z\n", $1);
	        $$ = 0;
	      }
	    else
	      $$ = (1 << n); 
	    free($1); 
	  }
	;

filter_level_list
	: filter_level filter_level_list	{ $$ = $1 + $2; }
	| filter_level				{ $$ = $1; }
	;

filter_level
	: IDENTIFIER DOTDOT IDENTIFIER		
	  { 
	    int r1, r2;
	    r1 = syslog_lookup_level($1);
	    if (r1 == -1)
	      werror("Warning: Unknown priority level %z\n", $1);
	    else
	      r1 = sl_levels[r1].value;
	    r2 = syslog_lookup_level($3);
	    if (r2 == -1)
	      werror("Warning: Unknown priority level %z\n", $1);
	    else
	      r2 = sl_levels[r2].value;
	    if (r1 != -1 && r2 != -1)
	      $$ = syslog_make_range(r1, r2); 
	    else
	      $$ = 0;
	    free($1); 
	    free($3); 
	  }
	| IDENTIFIER				
	  { 
	    int n = syslog_lookup_level($1); 
	    if (n == -1)
	      {
	        werror("Warning: Unknown priority level %z\n", $1);
	        $$ = 0;
	      }
	    else
	      $$ = 1 << sl_levels[n].value; 
	    free($1); 
	  }
	;

yesno
	: KW_YES				{ $$ = 1; }
	| KW_NO					{ $$ = 0; }
	| NUMBER				{ $$ = $1; }
	;

tripleoption
	: KW_REQUIRED				{ $$ = 2; }
	| KW_ALLOW				{ $$ = 1; }
	| KW_DENY				{ $$ = 0; }
	;

string
	: IDENTIFIER
	| STRING
	| NUMBER				{ char buf[32]; snprintf(buf, sizeof(buf), "%d", $1); $$ = strdup(buf); }
	;

%%

extern int linenum;

void yyerror(char *msg)
{
	fprintf(stderr, "%s at %d\n", msg, linenum);
}
