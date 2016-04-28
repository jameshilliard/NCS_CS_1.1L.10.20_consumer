#ifndef CLASS_DEFINE
struct alist_linear
{
  struct alist super;
  struct ol_object *((table)[MAX_LINEAR_ALIST_SIZE]);
};
extern struct alist_meta alist_linear_class_extended;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_alist_linear_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct alist_linear *i = (struct alist_linear *) o;
  {
  unsigned k;
  for (k=0; k<MAX_LINEAR_ALIST_SIZE; k++)
    mark((struct ol_object *) (i->table)[k]);
}
}

struct alist_meta alist_linear_class_extended =
{ { STATIC_HEADER,
  &alist_class, "alist_linear", sizeof(struct alist_linear),
  do_alist_linear_mark,
  NULL
},
  do_linear_get,
  do_linear_set};
#define alist_linear_class (alist_linear_class_extended.super)
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct alist_linked
{
  struct alist super;
  struct alist_node * head;
};
extern struct alist_meta alist_linked_class_extended;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_alist_linked_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct alist_linked *i = (struct alist_linked *) o;
  do_mark_list(i->head, mark);
}

static void do_alist_linked_free(struct ol_object *o)
{
  struct alist_linked *i = (struct alist_linked *) o;
  do_free_list(i->head);
}

struct alist_meta alist_linked_class_extended =
{ { STATIC_HEADER,
  &alist_class, "alist_linked", sizeof(struct alist_linked),
  do_alist_linked_mark,
  do_alist_linked_free
},
  do_linked_get,
  do_linked_set};
#define alist_linked_class (alist_linked_class_extended.super)
#endif /* !CLASS_DECLARE */

