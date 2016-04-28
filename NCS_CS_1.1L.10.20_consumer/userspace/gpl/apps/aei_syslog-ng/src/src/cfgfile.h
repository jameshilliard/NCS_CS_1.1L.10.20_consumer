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
 * $Id: cfgfile.h,v 1.26.4.2 2006/03/13 23:34:13 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __CFGFILE_H_INCLUDED
#define __CFGFILE_H_INCLUDED

#include "sources.h"
#include "filters.h"
#include "destinations.h"
#include "center.h"

#define CLASS_DECLARE
#include "cfgfile.h.x"
#undef CLASS_DECLARE

/* CLASS:
    (class
      (name syslog_config)
      (vars
        (init method int "struct persistent_config *")
	(destroy method int "struct persistent_config *")
	(resources object resource_list)
        (backend object io_backend)
        (living simple int)
	; global options
	(chain_hostnames simple UINT32)
	(keep_hostname simple UINT32)
	(mark_freq simple UINT32)
	(sync_freq simple UINT32)
	(stats_freq simple UINT32)
	(time_sleep simple UINT32)
	(time_reopen simple UINT32)
	(time_reap simple UINT32)
	(log_fifo_size simple int)
	(use_time_recvd simple UINT32)
	(use_fqdn simple UINT32)
	(use_dns simple UINT32)
	(check_hostname simple UINT32)
	(bad_hostname pointer UINT8)
	(create_dirs simple UINT32)
	(sanitize_filenames simple UINT32)
	(uid simple int)
	(gid simple int)
	(perm simple int)
	(dir_uid simple int)
	(dir_gid simple int)
	(dir_perm simple int)
	(dns_cache simple UINT32)
	(dns_cache_size simple UINT32)
	(dns_cache_expire simple UINT32)
	(dns_cache_expire_failed simple UINT32)
	(cache simple "struct nscache *")
	(gc_busy_threshold simple UINT32)
	(gc_idle_threshold simple UINT32)
	;
	(log_msg_size simple UINT32)
	; internal messages should come from this source
	(internal object log_handler)
        (sources object log_source_group)
        (filters object log_filter)
        (destinations object log_dest_group)
	; connections between sources and destinations
	(last_connection object log_connection)
        (connections object log_connection))) 
*/

#define CONFIG_INIT(c, p) (c)->init(c, p)
#define CONFIG_DESTROY(c, p) (c)->destroy(c, p)

/* CLASS:
   (class 
     (name persistent_info)
     (vars
       (name string)
       (o object ol_object)
       (kill pointer (function void "struct ol_string *name" "struct ol_object *o"))
       (next object persistent_info)))
*/

struct persistent_info *
make_persistent_info(struct ol_string *name, 
		     struct ol_object *o, 
		     void (*kill)(struct ol_string *name, struct ol_object *obj));

/* info needed to kept persistent upon restart */
/* CLASS:
   (class
     (name persistent_config)
     (vars
       (nodes object persistent_info)))
*/

struct persistent_info *find_persistent_info(struct persistent_config *p, UINT32 length, const char *name);

void add_persistent_info(struct persistent_config *p, struct persistent_info *node);

void kill_persistent_config(struct persistent_config *persistent);

struct persistent_config *make_persistent_config(void);

extern struct syslog_config *configuration;

void add_dest_group(struct log_dest_group *grp);
void add_source_group(struct log_source_group *grp);
void add_log_connection(struct log_connection *conn);
void add_filter_rule(struct log_filter *filter);
void cfg_set_owner(char *uid);
void cfg_set_group(char *gid);
void cfg_set_perm(int perm);
void cfg_set_dir_owner(char *uid);
void cfg_set_dir_group(char *gid);
void cfg_set_dir_perm(int perm);
void cfg_set_bad_hostname(char *bad_hostname);


struct syslog_config *make_syslog_config(const char *name, struct io_backend *backend);

#endif
