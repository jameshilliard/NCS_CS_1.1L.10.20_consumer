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
 * $Id: queue.c,v 1.3 1999/11/22 18:26:24 bazsi Exp $
 *
 ***************************************************************************/

#include "queue.h"

#include "werror.h"

#include <assert.h>

static void do_queue_mark(struct ol_queue *q,
                                 void (*mark)(struct ol_object *o));

static void do_queue_free(struct ol_queue *q);

#define CLASS_DEFINE
#include "queue.h.x"
#undef CLASS_DEFINE

/* Short cuts */
#define next np_links[LSH_QUEUE_NEXT]
#define prev np_links[LSH_QUEUE_PREV]

#define head ht_links[LSH_QUEUE_HEAD]
#define tail ht_links[LSH_QUEUE_TAIL]
#define tailprev ht_links[LSH_QUEUE_TAILPREV]

#define EMPTYP(q) ((q)->tailprev == (struct ol_queue_node *) (q))

#if 0
static void sanity_check_queue(struct ol_queue *q)
{
  struct ol_queue_node *n;

  debug("sanity_check_queue: q = %xi\n", (UINT32) q);
  if (q->tail)
    fatal("sanity_check_queue: q->tail not NULL!\n");

  n = q->head;

#if 0
  if (EMPTYP(q))
    {
      debug("  queue is empty\n");
      if (n->prev)
	fatal("sanity_check_queue: "
	      "Queue looks empty, but n->prev not NULL!\n");
      if (q->tail != (struct ol_queue_node *) q)
	fatal("sanity_check_queue: "
	      "Queue looks empty, but q->tail != q !\n");
      return;
    }
#endif
  if (n->prev != (struct ol_queue_node *) q)
    fatal("sanity_check_queue: head->next != &q->head !\n");

  while (n->next)
    {
      debug("  n = %xi\n", (UINT32) n);
      
      if (n->prev->next != n)
	fatal("n->prev->next != n !\n");

      n = n->next;
    }
  if (n != (struct ol_queue_node *) &(q->tail))
    fatal("n != n &t->tail!\n");
}
#else
#define sanity_check_queue(x)
#endif

void ol_queue_init(struct ol_queue *q)
{
  q->head = (struct ol_queue_node *) &(q->tail);
  q->tail = NULL;
  q->tailprev = (struct ol_queue_node *) &(q->head);
  sanity_check_queue(q);
}

int ol_queue_is_empty(struct ol_queue *q)
{
  sanity_check_queue(q);
  return EMPTYP(q);
}

void ol_queue_add_head(struct ol_queue *q, struct ol_queue_node *n)
{
  sanity_check_queue(q);
  n->next = q->head;
  n->prev = (struct ol_queue_node *) &(q->head);
  n->prev->next = n;
  n->next->prev = n;
  sanity_check_queue(q);
}

void ol_queue_add_tail(struct ol_queue *q, struct ol_queue_node *n)
{
  sanity_check_queue(q);
  n->next = (struct ol_queue_node *) &(q->tail);
  n->prev = q->tailprev;
  n->prev->next = n;
  n->next->prev = n;
  sanity_check_queue(q);
}

void ol_queue_remove(struct ol_queue_node *n)
{
  assert(n->next);
  assert(n->prev);
  n->next->prev = n->prev;
  n->prev->next = n->next;
}

struct ol_queue_node *ol_queue_remove_head(struct ol_queue *q)
{
  struct ol_queue_node *n = q->head;

  sanity_check_queue(q);
  assert(!EMPTYP(q));
  ol_queue_remove(n);
  sanity_check_queue(q);

  return n;
}

struct ol_queue_node *ol_queue_remove_tail(struct ol_queue *q)
{
  struct ol_queue_node *n = q->tailprev;
  
  sanity_check_queue(q);
  assert(!EMPTYP(q));
  ol_queue_remove(n);
  sanity_check_queue(q);

  return n;
}

  
/* object_queue */
static struct object_queue_node *
make_object_queue_node(struct ol_object *o)
{
  struct object_queue_node *n;

  NEW_SPACE(n);
  n->o = o;

  return n;
}

struct object_queue *make_object_queue(void)
{
  NEW(object_queue, q);
  ol_queue_init(&q->q);
  return q;
}

int object_queue_is_empty(struct object_queue *q)
{
  return EMPTYP(&q->q);
}

struct object_queue_node *
object_queue_add_head(struct object_queue *q, struct ol_object *o)
{
  struct object_queue_node *n = make_object_queue_node(o);

  ol_queue_add_head(&q->q, &n->header);
  return n;
}

struct object_queue_node *
object_queue_add_tail(struct object_queue *q, struct ol_object *o)
{
  struct object_queue_node *n = make_object_queue_node(o);
  ol_queue_add_tail(&q->q, &n->header);
  return n;
}

static struct ol_object *
object_queue_get_contents(struct ol_queue_node *l)
{
  struct object_queue_node *n = (struct object_queue_node *) l;

  struct ol_object *res = n->o;
  ol_space_free(n);

  return res;
}

static struct ol_object *
object_queue_peek(struct ol_queue_node *n)
{
  return ( (struct object_queue_node *) n)->o;
}

void object_queue_remove(struct object_queue_node *n)
{
  ol_queue_remove(&n->header);
  ol_space_free(n);
}

struct ol_object *object_queue_remove_head(struct object_queue *q)
{
  return object_queue_get_contents(ol_queue_remove_head(&q->q));
}

struct ol_object *object_queue_remove_tail(struct object_queue *q)
{
  return object_queue_get_contents(ol_queue_remove_tail(&q->q));
}

struct ol_object *object_queue_peek_head(struct object_queue *q)
{
  return EMPTYP(&q->q) ? NULL : object_queue_peek(q->q.head);
}

struct ol_object *object_queue_peek_tail(struct object_queue *q)
{
  return EMPTYP(&q->q) ? NULL : object_queue_peek(q->q.tailprev);
}

/* For gc */
static void do_queue_mark(struct ol_queue *q,
                                 void (*mark)(struct ol_object *o))
{
  FOR_QUEUE(q, struct object_queue_node *, n)
    mark(n->o);
}

static void do_queue_free(struct ol_queue *q)
{
  FOR_QUEUE(q, struct object_queue_node *, n)
    ol_space_free(n);
}

void object_queue_kill(struct object_queue *q)
{
  do_queue_free(&q->q);
}
