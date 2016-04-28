#ifndef CLASS_DEFINE
struct log_center
{
  struct log_handler super;
  struct log_connection *connections;
};
extern struct ol_class log_center_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_center_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_center *i = (struct log_center *) o;
  mark((struct ol_object *) i->connections);
}

struct ol_class log_center_class =
{ STATIC_HEADER,
  &log_handler_class, "log_center", sizeof(struct log_center),
  do_log_center_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct driver_reinit
{
  struct callback super;
  struct log_handler *handler;
  struct syslog_config *cfg;
};
extern struct ol_class driver_reinit_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_driver_reinit_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct driver_reinit *i = (struct driver_reinit *) o;
  mark((struct ol_object *) i->handler);
  mark((struct ol_object *) i->cfg);
}

struct ol_class driver_reinit_class =
{ STATIC_HEADER,
  &callback_class, "driver_reinit", sizeof(struct driver_reinit),
  do_driver_reinit_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct driver_reinit_on_close
{
  struct close_callback super;
  struct log_handler *handler;
  struct resource_node * res;
  struct syslog_config *cfg;
  struct ol_string *desc;
  int time_reopen;
};
extern struct ol_class driver_reinit_on_close_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_driver_reinit_on_close_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct driver_reinit_on_close *i = (struct driver_reinit_on_close *) o;
  mark((struct ol_object *) i->handler);
  mark((struct ol_object *) i->cfg);
}

static void do_driver_reinit_on_close_free(struct ol_object *o)
{
  struct driver_reinit_on_close *i = (struct driver_reinit_on_close *) o;
  ol_string_free(i->desc);
}

struct ol_class driver_reinit_on_close_class =
{ STATIC_HEADER,
  &close_callback_class, "driver_reinit_on_close", sizeof(struct driver_reinit_on_close),
  do_driver_reinit_on_close_mark,
  do_driver_reinit_on_close_free
};
#endif /* !CLASS_DECLARE */

