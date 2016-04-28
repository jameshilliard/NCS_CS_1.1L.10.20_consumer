#ifndef CLASS_DEFINE
struct afprogram_dest
{
  struct log_dest_driver super;
  struct ol_string *progname;
  struct io_fd *dest;
  struct ol_string *template_output;
  int template_escape;
  struct syslog_config *cfg;
};
extern struct ol_class afprogram_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afprogram_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afprogram_dest *i = (struct afprogram_dest *) o;
  mark((struct ol_object *) i->dest);
  mark((struct ol_object *) i->cfg);
}

static void do_afprogram_dest_free(struct ol_object *o)
{
  struct afprogram_dest *i = (struct afprogram_dest *) o;
  ol_string_free(i->progname);
  ol_string_free(i->template_output);
}

struct ol_class afprogram_dest_class =
{ STATIC_HEADER,
  &log_dest_driver_class, "afprogram_dest", sizeof(struct afprogram_dest),
  do_afprogram_dest_mark,
  do_afprogram_dest_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afprogram_child
{
  struct resource super;
  pid_t pid;
  struct io_fd *dest;
};
extern struct ol_class afprogram_child_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afprogram_child_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afprogram_child *i = (struct afprogram_child *) o;
  mark((struct ol_object *) i->dest);
}

struct ol_class afprogram_child_class =
{ STATIC_HEADER,
  &resource_class, "afprogram_child", sizeof(struct afprogram_child),
  do_afprogram_child_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

