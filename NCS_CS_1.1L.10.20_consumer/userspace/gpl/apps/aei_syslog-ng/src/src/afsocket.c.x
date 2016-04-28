#ifndef CLASS_DEFINE
struct afsocket_source_close_callback
{
  struct close_callback super;
  struct afsocket_source_connection *connection;
};
extern struct ol_class afsocket_source_close_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_source_close_callback_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_source_close_callback *i = (struct afsocket_source_close_callback *) o;
  mark((struct ol_object *) i->connection);
}

struct ol_class afsocket_source_close_callback_class =
{ STATIC_HEADER,
  &close_callback_class, "afsocket_source_close_callback", sizeof(struct afsocket_source_close_callback),
  do_afsocket_source_close_callback_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afsocket_accept_callback
{
  struct fd_listen_callback super;
  struct io_backend *backend;
  struct afsocket_source *source;
};
extern struct ol_class afsocket_accept_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_accept_callback_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_accept_callback *i = (struct afsocket_accept_callback *) o;
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->source);
}

struct ol_class afsocket_accept_callback_class =
{ STATIC_HEADER,
  &fd_listen_callback_class, "afsocket_accept_callback", sizeof(struct afsocket_accept_callback),
  do_afsocket_accept_callback_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afsocket_dest_connected
{
  struct fd_callback super;
  struct io_backend *backend;
  struct afsocket_dest *dest;
};
extern struct ol_class afsocket_dest_connected_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_dest_connected_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_dest_connected *i = (struct afsocket_dest_connected *) o;
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->dest);
}

struct ol_class afsocket_dest_connected_class =
{ STATIC_HEADER,
  &fd_callback_class, "afsocket_dest_connected", sizeof(struct afsocket_dest_connected),
  do_afsocket_dest_connected_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

