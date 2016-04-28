#ifndef CLASS_DEFINE
struct afsocket_source_connection
{
  struct log_handler_pipe super;
  struct afsocket_source *owner;
  struct object_queue_node * queue_node;
  struct io_fd *client;
  struct address_info *client_addr;
  int (*(close))(struct afsocket_source_connection *self, int close_reason);
};
extern struct ol_class afsocket_source_connection_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_source_connection_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_source_connection *i = (struct afsocket_source_connection *) o;
  mark((struct ol_object *) i->owner);
  mark((struct ol_object *) i->client);
  mark((struct ol_object *) i->client_addr);
}

struct ol_class afsocket_source_connection_class =
{ STATIC_HEADER,
  &log_handler_pipe_class, "afsocket_source_connection", sizeof(struct afsocket_source_connection),
  do_afsocket_source_connection_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afsocket_source
{
  struct log_source_driver super;
  UINT32 flags;
  struct address_info *bind_addr;
  UINT32 max_connections;
  UINT32 num_connections;
  struct object_queue *connections;
  int (*(open_connection))(struct afsocket_source *self, struct io_fd *, struct address_info *a);
};
extern struct ol_class afsocket_source_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_source_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_source *i = (struct afsocket_source *) o;
  mark((struct ol_object *) i->bind_addr);
  mark((struct ol_object *) i->connections);
}

struct ol_class afsocket_source_class =
{ STATIC_HEADER,
  &log_source_driver_class, "afsocket_source", sizeof(struct afsocket_source),
  do_afsocket_source_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afsocket_dest
{
  struct log_dest_driver super;
  UINT32 flags;
  struct address_info *bind_addr;
  struct address_info *dest_addr;
  int (*(connected))(struct afsocket_dest *self, struct io_fd *fd);
};
extern struct ol_class afsocket_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afsocket_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afsocket_dest *i = (struct afsocket_dest *) o;
  mark((struct ol_object *) i->bind_addr);
  mark((struct ol_object *) i->dest_addr);
}

struct ol_class afsocket_dest_class =
{ STATIC_HEADER,
  &log_dest_driver_class, "afsocket_dest", sizeof(struct afsocket_dest),
  do_afsocket_dest_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

