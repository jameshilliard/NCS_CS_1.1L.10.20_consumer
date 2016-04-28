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
 * $Id: list.c,v 1.6 2004/01/19 13:20:38 bazsi Exp $
 *
 ***************************************************************************/

#include "list.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define CLASS_DEFINE
#include "list.h.x"
#undef CLASS_DEFINE

#include "xalloc.h"

struct int_list *make_int_listv(unsigned n, va_list args)
{
  unsigned i;
  
  struct int_list *l = alloc_int_list(n);
  
  l->super.length = n;

  for (i=0; i<n; i++)
    {
      int atom = va_arg(args, int);
      assert(atom >= 0);
      LIST(l)[i] = atom;
    }

  assert(va_arg(args, int) == -1);

  return l;
}

struct int_list *make_int_list(unsigned n, ...)
{
  va_list args;
  struct int_list *l;
  
  va_start(args, n);
  l = make_int_listv(n, args);
  va_end(args);

  return l;
}

struct string_list *make_string_listv(unsigned n, va_list args)
{	
  unsigned i;
  
  struct string_list *l = alloc_string_list(n);
  l->super.length = n;

  for (i=0; i<n; i++)
    LIST(l)[i] = va_arg(args, struct ol_string *);

  assert(va_arg(args, int) == -1);

  return l;
}

struct string_list *make_string_list(unsigned n, ...)
{
  va_list args;
  struct string_list *l;

  va_start(args, n);
  l = make_string_listv(n, args);
  va_end(args);

  return l;
}

struct object_list *make_object_listv(unsigned n, va_list args)
{
  unsigned i;
  
  struct object_list *l = alloc_object_list(n);
  l->super.length = n;

  for (i=0; i<n; i++)
    LIST(l)[i] = va_arg(args, struct ol_object *);

  assert(va_arg(args, int) == -1);

  return l;
}

struct object_list *make_object_list(unsigned n, ...)
{
  va_list args;
  struct object_list *l;

  va_start(args, n);
  l = make_object_listv(n, args);
  va_end(args);

  return l;
}

/* binary search on the given list 
   returns non-zero, if the item is found, zero otherwise
   *pos is set to the position where the item is found */
static int do_sorted_search(struct sorted_list *list, void *key, unsigned *pos)
{
	int l, h, m;
	int c;
	
	l = 0; h = LIST_LENGTH(list) - 1; m = 0;
	while (l <= h) {
		m = (l + h) / 2;
		c = list->compare(LIST(list)[m], key);
		if (c < 0) {
			l = m + 1;
		} else if (c > 0) {
			h = m - 1;
		} else {
			*pos = m;
			return 1;
		}
	}
	*pos = l;
	return 0;
}

static int do_list_insert(struct sorted_list *list, int pos, struct ol_object *s)
{
	if (LIST_LENGTH(list) < LIST_MAX(list)) {
		memmove(&(LIST(list)[pos + 1]), &(LIST(list)[pos]), (LIST_LENGTH(list) - pos) * sizeof(struct ol_object *));
		LIST(list)[pos] = s;
		LIST_LENGTH(list)++;
		return 1;
	}
	else {
		return 0;
	}
}

static int do_sorted_add(struct list_header *c, struct ol_object *s)
{
	CAST(sorted_list, list, c);

	if (list->flags & LIST_ADD_SORTED) {
		unsigned int pos;
		
		if (!LIST_SEARCH(list, LIST_KEYOF(list, s), &pos) || (list->flags & LIST_ADD_DUPS)) {
			return do_list_insert(list, pos, s);
		}
	}
	else {
		return do_list_insert(list, LIST_LENGTH(list), s);
	}
	return 0;
}

struct sorted_list *make_sorted_list(unsigned n, int flags, ol_compare_fn cmp, ol_keyof_fn keyof)
{
	struct sorted_list *self = alloc_sorted_list(n);
  
	self->flags = flags;
	self->compare = cmp;
	self->keyof = keyof;
	self->super.add = do_sorted_add;
	self->search = do_sorted_search;
	return self;
}
