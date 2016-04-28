#ifndef CLASS_DEFINE
struct log_endpoint_info
{
  struct ol_object super;
  int type;
  struct ol_string *name;
  struct ol_object *ref;
  struct log_endpoint_info *next;
};
extern struct ol_class log_endpoint_info_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_endpoint_info_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_endpoint_info *i = (struct log_endpoint_info *) o;
  mark((struct ol_object *) i->ref);
  mark((struct ol_object *) i->next);
}

static void do_log_endpoint_info_free(struct ol_object *o)
{
  struct log_endpoint_info *i = (struct log_endpoint_info *) o;
  ol_string_free(i->name);
}

struct ol_class log_endpoint_info_class =
{ STATIC_HEADER,
  0, "log_endpoint_info", sizeof(struct log_endpoint_info),
  do_log_endpoint_info_mark,
  do_log_endpoint_info_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct log_connection
{
  struct ol_object super;
  UINT32 flags;
  struct log_endpoint_info *sources;
  struct log_endpoint_info *filters;
  struct log_endpoint_info *destinations;
  struct log_connection *next;
};
extern struct ol_class log_connection_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_connection_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_connection *i = (struct log_connection *) o;
  mark((struct ol_object *) i->sources);
  mark((struct ol_object *) i->filters);
  mark((struct ol_object *) i->destinations);
  mark((struct ol_object *) i->next);
}

struct ol_class log_connection_class =
{ STATIC_HEADER,
  0, "log_connection", sizeof(struct log_connection),
  do_log_connection_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

