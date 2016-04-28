#ifndef CLASS_DEFINE
struct log_dest_driver
{
  struct log_handler super;
  int log_fifo_size;
  struct log_dest_driver *next_driver;
};
extern struct ol_class log_dest_driver_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_dest_driver_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_dest_driver *i = (struct log_dest_driver *) o;
  mark((struct ol_object *) i->next_driver);
}

struct ol_class log_dest_driver_class =
{ STATIC_HEADER,
  &log_handler_class, "log_dest_driver", sizeof(struct log_dest_driver),
  do_log_dest_driver_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct log_dest_group
{
  struct log_handler super;
  struct log_dest_group *next_dest_group;
  int used;
  struct ol_string *name;
  struct log_dest_driver *drivers;
};
extern struct ol_class log_dest_group_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_dest_group_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_dest_group *i = (struct log_dest_group *) o;
  mark((struct ol_object *) i->next_dest_group);
  mark((struct ol_object *) i->drivers);
}

static void do_log_dest_group_free(struct ol_object *o)
{
  struct log_dest_group *i = (struct log_dest_group *) o;
  ol_string_free(i->name);
}

struct ol_class log_dest_group_class =
{ STATIC_HEADER,
  &log_handler_class, "log_dest_group", sizeof(struct log_dest_group),
  do_log_dest_group_mark,
  do_log_dest_group_free
};
#endif /* !CLASS_DECLARE */

