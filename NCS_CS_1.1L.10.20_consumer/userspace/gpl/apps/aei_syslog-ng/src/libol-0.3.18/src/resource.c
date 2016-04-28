/***************************************************************************
 *
 * Copyright (c) 1998-1999 Niels Möller
 * Copyright (c) 1999 BalaBit Computing
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
 * $Id: resource.c,v 1.4 1999/07/10 13:23:09 bazsi Exp $
 *
 ***************************************************************************/


#include "resource.h"

#include "werror.h"
#include "xalloc.h"

/* Prototypes */
void do_mark_resources(struct ol_queue *q,
		       void (*mark)(struct ol_object *o));

void do_free_resources(struct ol_queue *q);
void dont_free_live_resource(int alive);

#define CLASS_DEFINE
#include "resource.h.x"
#undef CLASS_DEFINE

void do_mark_resources(struct ol_queue *q,
		       void (*mark)(struct ol_object *o))
{
  FOR_QUEUE(q, struct resource_node *, n)
    mark(&n->resource->super);
}

void do_free_resources(struct ol_queue *q)
{
  FOR_QUEUE(q, struct resource_node *, n)
    ol_space_free(n);
}

void dont_free_live_resource(int alive)
{
  if (alive)
    fatal("dont_free_live_resource: "
	  "About to garbage collect a live resource!\n");
}

static struct resource_node *do_remember_resource(struct resource_list *self,
						  struct resource *r)
{
  struct resource_node *n;

  NEW_SPACE(n);
  n->resource = r;

  ol_queue_add_head(&self->q, &n->header);

  return n;
}

static void do_kill_all(struct resource_list *self)
{
  /* FIXME: Doesn't deallocate any nodes (but gc should do that
   * later). */

  FOR_QUEUE(&self->q, struct resource_node *, n)
    KILL_RESOURCE(n->resource);
}

static void do_kill_resource(struct resource_list *self, struct resource_node *n)
{
	KILL_RESOURCE(n->resource);
	ol_queue_remove(&n->header);
	ol_space_free(n);
}
  
struct resource_list *empty_resource_list(void)
{
  NEW(resource_list, self);
  ol_queue_init(&self->q);

  self->remember = do_remember_resource;
  self->kill_all = do_kill_all;
  self->kill_resource = do_kill_resource;

  return self;
}
