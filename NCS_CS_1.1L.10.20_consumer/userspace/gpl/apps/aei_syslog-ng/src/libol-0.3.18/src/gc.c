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
 * $Id: gc.c,v 1.13 2005/05/03 15:49:35 bazsi Exp $
 *
 ***************************************************************************/

#include "objects.h"

#include <assert.h>

/* Global variables */
static struct ol_object *all_objects = NULL;
static unsigned number_of_objects = 0;
static unsigned live_objects = 0;
unsigned gc_busy_threshold = 500; 
unsigned gc_idle_threshold = 50;

#if 0
static struct ol_object *globals = NULL;
#endif

#ifdef DEBUG_ALLOC
static void sanity_check_object_list(void)
{
  unsigned i = 0;
  struct ol_object *o;

#if 0
  wwrite("sanity_check_object_list: Objects on list:\n");
  for(o = all_objects; o; o = o->next)
    werror("  %xi, class: %z\n", (void *) o, o->isa ? o->isa->name : "UNKNOWN");
#endif
  
  for(o = all_objects; o; o = o->next)
    i++;

  if (i != number_of_objects)
    fatal("sanity_check_object_list: Found %i objects, expected %i.\n",
	  i, number_of_objects);
}
#endif

/* FIXME: This function recurses heavily. One could use some trickery
 * to emulate tail recursion, which would help marking linked list. Or
 * one could use some more efficient datastructures than the C stack
 * for keeping track of the marked but not yet traced objects. */
static void gc_mark(struct ol_object *o)
{
  static int depth = 0;
  if (!o)
    return;
  
  switch(o->alloc_method)
    {
    case OL_ALLOC_STACK:
      fatal("gc_mark: Unexpected stack object!\n");

    case OL_ALLOC_HEAP:
      if (o->marked)
	return;
      o->marked = 1;
      /* Fall through */
    case OL_ALLOC_STATIC:
      /* Can't use mark bit on static objects, as there's no way to
       * reset all the bits */
      assert(!o->dead);
      {
	struct ol_class *class;

#if 1
	debug("gc_mark: Marking object of class '%z' (%i)\n",
	      o->isa ? o->isa->name : "UNKNOWN", depth);
#endif

        depth++;
	for (class = o->isa; class; class = class->super_class)
	  {
	    if (class->mark_instance)
	      MARK_INSTANCE(class, o, gc_mark);
	  }
        depth--;
      }
      break;
    default:
      fatal("gc_mark: Memory corrupted!\n");
    }
}

static void gc_sweep(void)
{
  struct ol_object *o;
  struct ol_object **o_p;

  live_objects = 0;
  
  for(o_p = &all_objects; (o = *o_p); )
    {
      if (o->marked)
	{
	  /* Keep object */
	  live_objects++;
	  o->marked = 0;
	}
      else
	{
	  struct ol_class *class;

#if 0
	  debug("gc_sweep: Freeing object of class '%z'\n",
		o->isa->name);
#endif  
	  for (class = o->isa; class; class = class->super_class)
	    if (class->free_instance)
	      FREE_INSTANCE(class, o);

	  /* Unlink object */
	  *o_p = o->next;
	  number_of_objects--;
	  
	  ol_object_free(o);
	  continue;
	}
      o_p = &o->next;
    }
  assert(live_objects == number_of_objects);
}

void gc_register(struct ol_object *o)
{
#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif
  o->marked = o->dead = 0;
  o->next = all_objects;
  all_objects = o;

  number_of_objects ++;
#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif
}

#if 0
/* FIXME: This function is utterly broken, and should be deleted. The
 * problem is that the object must be unlinked from the all_objects
 * list before linked into the globals list. */
void gc_register_global(struct ol_object *o)
{
#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif
  o->marked = o->dead = 0;
  o->next = globals;
  globals = o;

#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif
}
#endif

/* FIXME: This function should really deallocate and forget the object
 * early. But we keep it until the next gc, in order to catch any
 * references to killed objects. */
void gc_kill(struct ol_object *o)
{
#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif

  if (!o)
    return;
  
  o->dead++;

#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif
}

void gc(struct ol_object *root)
{
  unsigned before = number_of_objects;

  gc_mark(root);  
  gc_sweep();
  
  verbose("Objects alive: %i, garbage collected: %i\n", live_objects,
	  before - live_objects);
}

extern UINT32 strcount;

void gc_maybe(struct ol_object *root, int busy)
{
#ifdef DEBUG_ALLOC
  sanity_check_object_list();
#endif

  if ((busy && (number_of_objects > gc_busy_threshold)) ||
      (!busy && (number_of_objects > gc_idle_threshold)))
    { 

      verbose("Garbage collecting while %z...\n", busy ? "busy" : "idle");
      gc(root);
    }


}
