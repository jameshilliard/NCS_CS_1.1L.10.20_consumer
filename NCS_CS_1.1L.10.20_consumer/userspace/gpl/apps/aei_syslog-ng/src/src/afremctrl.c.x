#ifndef CLASS_DEFINE
struct afremctrl_dest
{
  struct log_dest_driver super;
  struct syslog_config *cfg;
};
extern struct ol_class afremctrl_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afremctrl_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afremctrl_dest *i = (struct afremctrl_dest *) o;
  mark((struct ol_object *) i->cfg);
}

struct ol_class afremctrl_dest_class =
{ STATIC_HEADER,
  &log_dest_driver_class, "afremctrl_dest", sizeof(struct afremctrl_dest),
  do_afremctrl_dest_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

