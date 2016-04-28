#ifndef CLASS_DEFINE
struct afuser_dest
{
  struct log_dest_driver super;
  struct ol_string *username;
  struct ol_string *template_output;
  int template_escape;
  struct syslog_config *cfg;
};
extern struct ol_class afuser_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afuser_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afuser_dest *i = (struct afuser_dest *) o;
  mark((struct ol_object *) i->cfg);
}

static void do_afuser_dest_free(struct ol_object *o)
{
  struct afuser_dest *i = (struct afuser_dest *) o;
  ol_string_free(i->username);
  ol_string_free(i->template_output);
}

struct ol_class afuser_dest_class =
{ STATIC_HEADER,
  &log_dest_driver_class, "afuser_dest", sizeof(struct afuser_dest),
  do_afuser_dest_mark,
  do_afuser_dest_free
};
#endif /* !CLASS_DECLARE */

