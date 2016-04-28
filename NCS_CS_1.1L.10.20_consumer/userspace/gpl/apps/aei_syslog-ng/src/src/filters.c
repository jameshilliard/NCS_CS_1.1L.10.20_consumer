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
 * $Id: filters.c,v 1.17.4.3 2006/05/08 09:26:37 bazsi Exp $
 *
 ***************************************************************************/

#include "filters.h"
#include "format.h"
#include "syslog-names.h"

#include <syslog.h>
#include <sys/types.h>
#include <regex.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define CLASS_DEFINE
#include "filters.h.x"
#undef CLASS_DEFINE

static void free_regexp(regex_t *re);

#include "filters.c.x"

struct log_filter *make_filter_rule(const char *name, struct filter_expr_node *root)
{
	NEW(log_filter, self);

	self->name = c_format("%z", name);
	self->root = root;
	return self;
}

/* CLASS:
   (class
     (name filter_expr_op)
     (super filter_expr_node)
     (vars
       (left object filter_expr_node)
       (right object filter_expr_node)))
*/

static int do_filter_and(struct filter_expr_node *c, 
			 struct log_filter *rule, 
			 struct log_info *log)
{
	CAST(filter_expr_op, self, c);

	return (FILTER_RULE_EVAL(self->left, rule, log) && FILTER_RULE_EVAL(self->right, rule, log)) ^ c->comp;
}

struct filter_expr_node *make_filter_and(struct filter_expr_node *l, struct filter_expr_node *r)
{
	NEW(filter_expr_op, self);
       
	self->super.eval = do_filter_and;
	self->left = l;
	self->right = r;
	return &self->super;
}

static int do_filter_or(struct filter_expr_node *c, 
			struct log_filter *rule,
			struct log_info *log)
{
	CAST(filter_expr_op, self, c);

	return (FILTER_RULE_EVAL(self->left, rule, log) || FILTER_RULE_EVAL(self->right, rule, log)) ^ c->comp;
}
 
struct filter_expr_node *make_filter_or(struct filter_expr_node *l, struct filter_expr_node *r)
{
	NEW(filter_expr_op, self);

	self->super.eval = do_filter_or;
	self->left = l;
	self->right = r;
	return &self->super;
}

/* CLASS:
   (class
     (name filter_expr_pri)
     (super filter_expr_node)
     (vars
       (valid simple UINT32)))
*/

static int do_filter_fac(struct filter_expr_node *c, 
			 struct log_filter *rule UNUSED,
			 struct log_info *log)
{
	CAST(filter_expr_pri, self, c);
	UINT32 pri = log->pri & LOG_FACMASK;
	int i;
	UINT32 bits = self->valid;
	if (bits & 0x80000000) {
		return ((bits & ~0x80000000) == pri) ^ self->super.comp;
	}
	else {
		for (i = 0; bits && sl_facilities[i].name; i++, bits >>= 1) {
			if ((bits & 1) && sl_facilities[i].value == pri) {
				return !self->super.comp;
			}
		}
	}
	return self->super.comp;
}

struct filter_expr_node *make_filter_facility(UINT32 valid)
{
	NEW(filter_expr_pri, self);

	self->super.eval = do_filter_fac;
	self->valid = valid;
	return &self->super;
}

static int do_filter_lev(struct filter_expr_node *c, 
			 struct log_filter *rule UNUSED,
			 struct log_info *log)
{
	CAST(filter_expr_pri, self, c);
	UINT32 pri = log->pri & LOG_PRIMASK;

	return (!!(self->valid & (1 << pri))) ^ self->super.comp;
}


struct filter_expr_node *make_filter_level(UINT32 valid)
{
	NEW(filter_expr_pri, self);

	self->super.eval = do_filter_lev;
	self->valid = valid;
	return &self->super;
}

/* CLASS:
   (class
     (name filter_expr_re)
     (super filter_expr_node)
     (vars
       (regex special-struct regex_t #f free_regexp)))
*/

static void free_regexp(regex_t *re)
{
	regfree(re);
}

static int compile_regex(const char *re, regex_t *regex)
{
	int regerr;
	regerr = regcomp(regex, re, REG_NOSUB | REG_EXTENDED);
	if (regerr) {
		char errorbuf[256];

		regerror(regerr, regex, errorbuf, sizeof(errorbuf));
		werror("Error compiling regular expression: \"%z\" (%z)\n", re, errorbuf);
		return 0;
	}
	return 1;
}

static int do_filter_host(struct filter_expr_node *c, 
			  struct log_filter *rule UNUSED,
			  struct log_info *log)
{
	CAST(filter_expr_re, self, c);
	return (!regexec(&self->regex, (char *) log->host->data, 0, NULL, 0)) ^ c->comp;
}

struct filter_expr_node *make_filter_host(const char *re)
{
	NEW(filter_expr_re, self);

	self->super.eval = do_filter_host;
	if (!compile_regex(re, &self->regex)) {
		KILL(self);
		return NULL;
	}
	return &self->super;
}

static int do_filter_match(struct filter_expr_node *c, 
			   struct log_filter *rule UNUSED,
			   struct log_info *log)
{
	CAST(filter_expr_re, self, c);
	return (!regexec(&self->regex, (char *) log->msg->data, 0, NULL, 0)) ^ c->comp;
}

struct filter_expr_node *make_filter_match(const char *re)
{
	NEW(filter_expr_re, self);

	self->super.eval = do_filter_match;
	if (!compile_regex(re, &self->regex)) {
		KILL(self);
		return NULL;
	}

	return &self->super;
}

static int do_filter_prog(struct filter_expr_node *c, 
			  struct log_filter *rule UNUSED,
			  struct log_info *log)
{
	CAST(filter_expr_re, self, c);
	return (log->program && !regexec(&self->regex, (char *) log->program->data, 0, NULL, 0)) ^ c->comp;
}

struct filter_expr_node *make_filter_prog(const char *re)
{
	NEW(filter_expr_re, self);

	self->super.eval = do_filter_prog;
	if (!compile_regex(re, &self->regex)) {
		KILL(self);
		return NULL;
	}

	return &self->super;
}

/* CLASS:
   (class
     (name filter_expr_netmask)
     (super filter_expr_node)
     (vars
       (network simple "struct in_addr")
       (netmask simple "struct in_addr")))
*/

static int do_filter_netmask(struct filter_expr_node *c,
                             struct log_filter *rule UNUSED,
                             struct log_info *log)
{
	CAST(filter_expr_netmask, self, c);
       
	if (log->saddr) {
		CAST(inet_address_info, adrinf, log->saddr);
		unsigned long host;
		unsigned long netw;
		unsigned long mask;

		host = adrinf->sa.sin_addr.s_addr;
		netw = self->network.s_addr;
		mask = self->netmask.s_addr;

		return ((host & mask) == (netw & mask)) ^ c->comp;
	}
	else {
		return c->comp;
	}
}

struct filter_expr_node *make_filter_netmask(const char *nm)
{
       char a_nw[16]="";
       char a_nm[16]="255.255.255.255";
       int i;

       NEW(filter_expr_netmask, self);

       self->super.eval = do_filter_netmask;

       for (i=0; nm[i] != 0 && nm[i] != '/'; i++) {
               if(i >= 15) {
                       /* no ip address can be longer than 15 bytes! */
                       KILL(self);
                       return NULL;
               }
               a_nw[i] = nm[i];
       }
       a_nw[i] = 0;

       if (nm[i]) {
               nm += i + 1;
               for (i=0; nm[i] != 0 && nm[i] != '/'; i++) {
                       if (i >= 15) {
                               /* no ip address can be longer than 15 bytes! */
                               KILL(self);
                               return NULL;
                       }
                       a_nm[i] = nm[i];
               }
               a_nm[i] = 0;
       }

       if (!(inet_aton(a_nw, &self->network) && inet_aton(a_nm, &self->netmask)) ) {
               KILL(self);
               return NULL;
       }

       return &self->super;
}


/* CLASS:
   (class
     (name filter_expr_call)
     (super filter_expr_node)
     (vars
       (name string)
       (call_rule object log_filter)
       (error_logged . UINT32)))
*/

static int do_filter_call(struct filter_expr_node *c, 
			  struct log_filter *rule,
			  struct log_info *log)
{
	CAST(filter_expr_call, self, c);

	if (!self->call_rule) {
		struct log_filter *p = rule->prev_filter;
		while (p && 
		       (p->name->length != self->name->length ||
		        memcmp(p->name->data, self->name->data, self->name->length))) {
			p = p->prev_filter;
		}
		if (!p) {
			p = rule->next_filter;
			while (p && 
			       (p->name->length != self->name->length ||
			        memcmp(p->name->data, self->name->data, self->name->length))) {
				p = p->next_filter;
			}
		}
		self->call_rule = p;
	}
	if (self->call_rule)
		return (FILTER_RULE_EVAL(self->call_rule->root, self->call_rule, log)) ^ c->comp;
	else {
		static int recursed = 0;
		if (!recursed && !self->error_logged) {
			recursed++;
			werror("Error evaluating filter(%S), filter not found\n", self->name);
			recursed--;
			self->error_logged = 1;
		}
		return 1 ^ c->comp;
	}
}

struct filter_expr_node *make_filter_call(const char *name)
{
	NEW(filter_expr_call, self);

	self->name = c_format("%z", name);
	self->super.eval = do_filter_call;
	return &self->super;
}

