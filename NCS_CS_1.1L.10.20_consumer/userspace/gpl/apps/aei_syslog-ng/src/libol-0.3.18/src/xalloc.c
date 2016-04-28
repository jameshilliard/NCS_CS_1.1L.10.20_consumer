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
 * $Id: xalloc.c,v 1.10 2003/04/30 21:58:12 bazsi Exp $
 *
 ***************************************************************************/

#include "objects.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* There are two sets of allocation functions: Low level allocation *
 * that can allocate memory for any purpose, and object allocators
 * that assume that the allocated object begins with a type field. */

/* NOTE: This doesn't work if there are types that require other
 * alignment than int boundaries. But it doesn't matter much if the
 * optional debug code isn't fully portable. */

#ifdef DEBUG_ALLOC

/* This is a very dirty malloc debugger, because I couldn't find a leaked 
 * block otherwise 
 */

struct malloc_entry *first_malloc = NULL;

#define SIZE_IN_INTS(x) (((x) + sizeof(int)-1) / sizeof(int))
/* #define REALLY_DEBUG_ALLOC 1 */

#if REALLY_DEBUG_ALLOC
UINT32 strcount = 0;
#endif

static void *debug_malloc(size_t real_size)
{
  static int count = 4711;
  int *res;
  int size = SIZE_IN_INTS(real_size);
  
  res = malloc((size + 3)*sizeof(int));

  if (!res)
    return NULL;

  res += 2;
  
  res[-2] = count;
  res[-1] = real_size;
  res[size] = ~count;
  count++;


#if REALLY_DEBUG_ALLOC
  strcount++;
  {
	  struct malloc_entry *p = malloc(sizeof(struct malloc_entry));
	  p->ptr = res;
	  p->size = real_size;
	  p->next = first_malloc;
	  first_malloc = p;
  }
#endif
  return (void *) res;
}

void debug_free(void *m)
{
  if (m)
    {
      int *p = (int *) m;
      int real_size = p[-1];
      int size = SIZE_IN_INTS(real_size);
      
      if (~p[-2] != p[size])
	fatal("Memory corrupted!\n");
      
      p[-2] = 0;
      p[size] = 1;
      

      free(p-2);
#if REALLY_DEBUG_ALLOC
      strcount--;
      {
	      struct malloc_entry *e, *ep;
	      for (ep = NULL, e = first_malloc; e; ep = e, e = e->next) {
		      if (e->ptr == p) {
			      if (ep) 
				      ep->next = e->next;
			      else
				      first_malloc = e->next;
			      break;
		      }
	      }
	      if (e) free(e);
      }
#endif
    }
}

#endif /* !DEBUG_ALLOC */

void *xalloc(size_t size)
{
  void *res = ol_malloc(size);
  if (!res)
    fatal("Virtual memory exhausted");

  /* FIXME: The gc can't handle uninitialized pointers. The simple way
   * is to zero-fill all memory as it is allocated. But initialization
   * is only necessary for objects, strings need no initialization. By
   * moving initializing to some higher level, we could avoid
   * unnecessary clearing, and also initialize mpz objects
   * automatically. */
  memset(res, 0, size);

  return res;
}

struct ol_object *ol_object_alloc(struct ol_class *class)
{
  struct ol_object *instance = xalloc(class->size);
  instance->isa = class;
  instance->alloc_method = OL_ALLOC_HEAP;

  gc_register(instance);

  return instance;
}

struct ol_object *ol_object_clone(struct ol_object *o)
{
  struct ol_object *i = xalloc(o->isa->size);

  /* Copy header and all instance variables. Note that the header is
   * now invalid, as the next pointer can't be copied directly. This
   * is fixed by the gc_register call below. */
  memcpy(i, o, o->isa->size);

  i->alloc_method = OL_ALLOC_HEAP;
  gc_register(i);

  return i;
}

struct list_header *ol_list_alloc(struct ol_class *class,
				   unsigned length, size_t element_size)
{
  struct list_header *list = xalloc(class->size
				    + element_size * length
				    - element_size);

  assert(element_size < 1024);
  /* assert(length < 65536); */
  
  list->super.isa = class;
  list->super.alloc_method = OL_ALLOC_HEAP;

  list->allocated = length;
  
  gc_register(&list->super);

  return list;
}
    
/* Should be called *only* by the gc */
void ol_object_free(struct ol_object *o)
{
  if (!o)
    return;
  
  if (o->alloc_method != OL_ALLOC_HEAP)
    fatal("ol_object_free: Object not allocated on the heap!\n");

#if 0
  if (o->isa->free_instance)
    o->isa->free_instance(o);
#endif
  
  ol_free(o);
}

struct ol_string *ol_string_use(struct ol_string *str)
{
	str->use_cnt++;
	return str;
}

struct ol_string *ol_string_alloc(UINT32 length)
{
	struct ol_string *s
		= xalloc(sizeof(struct ol_string) - 1 + length);
#ifdef DEBUG_ALLOC
	s->magic = -1717;
#endif
	s->length = length;
	s->use_cnt = 1;
	return s;
}

void ol_string_free(struct ol_string *s)
{
	if (!s)
		return;
	
#if 0
	debug("ol_string_free: freeing %xi.\n", (void *) s);
#endif
	assert(s->use_cnt);
	if (--s->use_cnt == 0) {
#ifdef DEBUG_ALLOC
		if (s->magic != -1717)
			fatal("ol_string_free: Not string!\n");
		s->magic = 9999;
#endif
		ol_free(s);
	}
}

#ifdef DEBUG_ALLOC
struct ol_object *ol_object_check(struct ol_class *class,
				    struct ol_object *instance)
{
  if (!instance)
    return instance;
  
  if (instance->marked)
    fatal("ol_object_check: Unexpected marked object!\n");

  if (instance->dead)
    fatal("ol_object_check: Reference to dead object!\n");

  if ( (instance->alloc_method == OL_ALLOC_HEAP)
       && (instance->isa != class))
    fatal("ol_object_check: Type error!\n");

  return instance;
}

struct ol_object *ol_object_check_subtype(struct ol_class *class,
					    struct ol_object *instance)
{
  struct ol_class *type;
  
  if (!instance)
    return instance;

  if (instance->marked)
    fatal("ol_object_check: Unexpected marked object!\n");

  if (instance->dead)
    fatal("ol_object_check: Reference to dead object!\n");

  /* Only heap allocated objects have a valid isa-pointer */
  switch (instance->alloc_method)
    {
    case OL_ALLOC_STATIC:
    case OL_ALLOC_STACK:
      return instance;
    case OL_ALLOC_HEAP:
      break;
    default:
      fatal("ol_object_check_subtype: Memory corrupted!\n");
    }
  
  for (type = instance->isa; type; type=type->super_class)
    if (type == class)
      return instance;

  fatal("ol_object_check_subtype: Type error!\n");
  return 0;
}
#endif /* DEBUG_ALLOC */

#ifdef DEBUG_ALLOC
void *ol_space_alloc(size_t size)
{
  int * p = xalloc(size + sizeof(int));

  *p = -1919;

  return (void *) (p + 1);
}

void ol_space_free(void *p)
{
  int *m;
  
  if (!p)
    return;
  
  m = ((int *) p) - 1;

  if (*m != -1919)
    fatal("ol_free_space: Type error!\n");

  ol_free(m);
}

#else /* !DEBUG_ALLOC */

void *ol_space_alloc(size_t size)
{
  return xalloc(size);
}

void ol_space_free(void *p)
{
  ol_free(p);
}

#endif /* !DEBUG_ALLOC */

