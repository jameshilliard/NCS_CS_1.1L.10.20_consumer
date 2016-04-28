#ifndef CLASS_DEFINE
struct reread_config
{
  struct callback super;
  struct syslog_backend *backend;
  int state;
};
extern struct ol_class reread_config_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_reread_config_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct reread_config *i = (struct reread_config *) o;
  mark((struct ol_object *) i->backend);
}

struct ol_class reread_config_class =
{ STATIC_HEADER,
  &callback_class, "reread_config", sizeof(struct reread_config),
  do_reread_config_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct report_stats
{
  struct callback super;
  int timeout;
  struct syslog_backend *backend;
};
extern struct ol_class report_stats_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_report_stats_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct report_stats *i = (struct report_stats *) o;
  mark((struct ol_object *) i->backend);
}

struct ol_class report_stats_class =
{ STATIC_HEADER,
  &callback_class, "report_stats", sizeof(struct report_stats),
  do_report_stats_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct syslog_backend
{
  struct io_backend super;
  struct syslog_config *newconfig;
  struct syslog_config *oldconfig;
  struct persistent_config *persistent;
  struct syslog_config *configuration;
};
extern struct ol_class syslog_backend_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_syslog_backend_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct syslog_backend *i = (struct syslog_backend *) o;
  mark((struct ol_object *) i->newconfig);
  mark((struct ol_object *) i->oldconfig);
  mark((struct ol_object *) i->persistent);
  mark((struct ol_object *) i->configuration);
}

struct ol_class syslog_backend_class =
{ STATIC_HEADER,
  &io_backend_class, "syslog_backend", sizeof(struct syslog_backend),
  do_syslog_backend_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

