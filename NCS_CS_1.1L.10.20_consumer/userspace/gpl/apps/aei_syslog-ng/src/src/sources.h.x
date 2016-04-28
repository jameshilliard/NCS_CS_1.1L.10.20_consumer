#ifndef CLASS_DEFINE
struct log_source_driver
{
  struct log_handler_pipe super;
  struct log_source_driver *next_driver;
};
extern struct ol_class log_source_driver_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_source_driver_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_source_driver *i = (struct log_source_driver *) o;
  mark((struct ol_object *) i->next_driver);
}

struct ol_class log_source_driver_class =
{ STATIC_HEADER,
  &log_handler_pipe_class, "log_source_driver", sizeof(struct log_source_driver),
  do_log_source_driver_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct log_source_group
{
  struct log_handler_pipe super;
  struct log_source_group *next_source_group;
  int used;
  int chain_hostnames;
  int use_fqdn;
  int use_dns;
  struct nscache * cache;
  int keep_hostname;
  struct log_source_driver *drivers;
  struct ol_string *name;
};
extern struct ol_class log_source_group_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_source_group_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_source_group *i = (struct log_source_group *) o;
  mark((struct ol_object *) i->next_source_group);
  mark((struct ol_object *) i->drivers);
}

static void do_log_source_group_free(struct ol_object *o)
{
  struct log_source_group *i = (struct log_source_group *) o;
  ol_string_free(i->name);
}

struct ol_class log_source_group_class =
{ STATIC_HEADER,
  &log_handler_pipe_class, "log_source_group", sizeof(struct log_source_group),
  do_log_source_group_mark,
  do_log_source_group_free
};
#endif /* !CLASS_DECLARE */

