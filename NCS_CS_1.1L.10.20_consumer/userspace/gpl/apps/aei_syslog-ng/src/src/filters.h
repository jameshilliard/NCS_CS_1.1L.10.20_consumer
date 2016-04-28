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
 * $Id: filters.h,v 1.9 2002/02/04 16:07:50 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __FILTERS_H_INCLUDED
#define __FILTERS_H_INCLUDED

#include "syslog-ng.h"
#include "log.h"

#define CLASS_DECLARE
#include "filters.h.x"
#undef CLASS_DECLARE

/* CLASS:
   (class
     (name log_filter)
     (vars
       (name string)
       (root object filter_expr_node)
       (next_filter object log_filter)
       (prev_filter object log_filter)))
*/

/* CLASS:
   (class
     (name filter_expr_node)
     (vars
       ; complement
       (comp simple int)
       (eval method int "struct log_filter *" "struct log_info *")))
*/

#define FILTER_RULE_EVAL(n, r, l) ((n)->eval(n, r, l))

extern int lex_filter_params;

struct log_filter *make_filter_rule(const char *name, struct filter_expr_node *root);

struct filter_expr_node *make_filter_and(struct filter_expr_node *l, struct filter_expr_node *r);
struct filter_expr_node *make_filter_or(struct filter_expr_node *l, struct filter_expr_node *r);
struct filter_expr_node *make_filter_facility(UINT32 valid);
struct filter_expr_node *make_filter_level(UINT32 valid);
struct filter_expr_node *make_filter_prog(const char *name);
struct filter_expr_node *make_filter_netmask(const char *nm);
struct filter_expr_node *make_filter_host(const char *re);
struct filter_expr_node *make_filter_match(const char *re);
struct filter_expr_node *make_filter_call(const char *name);

#endif
