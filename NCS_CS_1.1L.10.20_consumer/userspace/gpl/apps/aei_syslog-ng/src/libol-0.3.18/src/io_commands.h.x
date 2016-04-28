#ifndef CLASS_DEFINE
struct listen_value
{
  struct ol_object super;
  struct io_fd *fd;
  struct address_info *peer;
};
extern struct ol_class listen_value_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_listen_value_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct listen_value *i = (struct listen_value *) o;
  mark((struct ol_object *) i->fd);
  mark((struct ol_object *) i->peer);
}

struct ol_class listen_value_class =
{ STATIC_HEADER,
  0, "listen_value", sizeof(struct listen_value),
  do_listen_value_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

