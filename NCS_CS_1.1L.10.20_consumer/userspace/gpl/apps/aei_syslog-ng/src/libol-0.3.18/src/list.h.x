#ifndef CLASS_DEFINE
struct list_header
{
  struct ol_object super;
  unsigned length;
  unsigned allocated;
  int (*(add))(struct list_header *self, struct ol_object *s);
};
extern struct ol_class list_header_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class list_header_class =
{ STATIC_HEADER,
  0, "list_header", sizeof(struct list_header),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct int_list
{
  struct list_header super;
  int ((elements)[1]);
};
extern struct ol_class int_list_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class int_list_class =
{ STATIC_HEADER,
  &list_header_class, "int_list", sizeof(struct int_list),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct object_list
{
  struct list_header super;
  struct ol_object *((elements)[1]);
};
extern struct ol_class object_list_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_object_list_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct object_list *i = (struct object_list *) o;
  {
  unsigned k;
  for (k=0; k<i->super.length; k++)
    mark((struct ol_object *) (i->elements)[k]);
}
}

struct ol_class object_list_class =
{ STATIC_HEADER,
  &list_header_class, "object_list", sizeof(struct object_list),
  do_object_list_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct string_list
{
  struct list_header super;
  struct ol_string *((elements)[1]);
};
extern struct ol_class string_list_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_string_list_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct string_list *i = (struct string_list *) o;
  {
  unsigned k;
  for (k=0; k<i->super.length; k++)
    mark((struct ol_object *) (i->elements)[k]);
}
}

struct ol_class string_list_class =
{ STATIC_HEADER,
  &list_header_class, "string_list", sizeof(struct string_list),
  do_string_list_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct sorted_list
{
  struct list_header super;
  int flags;
  ol_keyof_fn keyof;
  ol_compare_fn compare;
  int (*(search))(struct sorted_list *self, void *, unsigned *);
  struct ol_object *((elements)[1]);
};
extern struct ol_class sorted_list_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_sorted_list_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct sorted_list *i = (struct sorted_list *) o;
  {
  unsigned k;
  for (k=0; k<i->super.length; k++)
    mark((struct ol_object *) (i->elements)[k]);
}
}

struct ol_class sorted_list_class =
{ STATIC_HEADER,
  &list_header_class, "sorted_list", sizeof(struct sorted_list),
  do_sorted_list_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

