#ifndef CLASS_DEFINE
struct listen_command_callback
{
  struct fd_listen_callback super;
  struct io_backend *backend;
  struct command_continuation *c;
};
extern struct ol_class listen_command_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_listen_command_callback_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct listen_command_callback *i = (struct listen_command_callback *) o;
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->c);
}

struct ol_class listen_command_callback_class =
{ STATIC_HEADER,
  &fd_listen_callback_class, "listen_command_callback", sizeof(struct listen_command_callback),
  do_listen_command_callback_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct listen_connection
{
  struct command super;
  struct command *callback;
  struct io_backend *backend;
};
extern struct ol_class listen_connection_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_listen_connection_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct listen_connection *i = (struct listen_connection *) o;
  mark((struct ol_object *) i->callback);
  mark((struct ol_object *) i->backend);
}

struct ol_class listen_connection_class =
{ STATIC_HEADER,
  &command_class, "listen_connection", sizeof(struct listen_connection),
  do_listen_connection_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct connect_command_callback
{
  struct fd_callback super;
  struct io_backend *backend;
  struct command_continuation *c;
};
extern struct ol_class connect_command_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_connect_command_callback_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct connect_command_callback *i = (struct connect_command_callback *) o;
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->c);
}

struct ol_class connect_command_callback_class =
{ STATIC_HEADER,
  &fd_callback_class, "connect_command_callback", sizeof(struct connect_command_callback),
  do_connect_command_callback_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct simple_io_command
{
  struct command super;
  struct io_backend *backend;
  struct resource_list *resources;
};
extern struct ol_class simple_io_command_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_simple_io_command_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct simple_io_command *i = (struct simple_io_command *) o;
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->resources);
}

struct ol_class simple_io_command_class =
{ STATIC_HEADER,
  &command_class, "simple_io_command", sizeof(struct simple_io_command),
  do_simple_io_command_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

