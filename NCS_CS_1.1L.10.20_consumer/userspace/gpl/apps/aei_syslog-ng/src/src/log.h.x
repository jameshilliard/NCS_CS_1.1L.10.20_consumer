#ifndef CLASS_DEFINE
struct log_handler
{
  struct ol_object super;
  struct log_handler_params *params;
  void (*(handler))(struct log_handler *self, struct log_info *);
  int (*(init))(struct log_handler *self, struct syslog_config *, struct persistent_config *);
  void (*(destroy))(struct log_handler *self, struct syslog_config *, struct persistent_config *);
};
extern struct ol_class log_handler_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_handler_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_handler *i = (struct log_handler *) o;
  mark((struct ol_object *) i->params);
}

struct ol_class log_handler_class =
{ STATIC_HEADER,
  0, "log_handler", sizeof(struct log_handler),
  do_log_handler_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct log_handler_pipe
{
  struct log_handler super;
  struct log_handler *next;
};
extern struct ol_class log_handler_pipe_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_handler_pipe_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_handler_pipe *i = (struct log_handler_pipe *) o;
  mark((struct ol_object *) i->next);
}

struct ol_class log_handler_pipe_class =
{ STATIC_HEADER,
  &log_handler_class, "log_handler_pipe", sizeof(struct log_handler_pipe),
  do_log_handler_pipe_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

