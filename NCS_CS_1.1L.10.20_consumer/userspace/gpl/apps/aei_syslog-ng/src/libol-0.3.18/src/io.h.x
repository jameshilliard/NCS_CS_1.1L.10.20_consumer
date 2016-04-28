#ifndef CLASS_DEFINE
struct fd_callback
{
  struct ol_object super;
  int (*(f))(struct fd_callback **self, int fd);
};
extern struct ol_class fd_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class fd_callback_class =
{ STATIC_HEADER,
  0, "fd_callback", sizeof(struct fd_callback),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct close_callback
{
  struct ol_object super;
  int (*(f))(struct close_callback *self, int reason);
};
extern struct ol_class close_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class close_callback_class =
{ STATIC_HEADER,
  0, "close_callback", sizeof(struct close_callback),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct nonblocking_fd
{
  struct resource super;
  struct nonblocking_fd *next;
  int fd;
  struct ol_string *fname;
  int to_be_closed;
  int close_reason;
  struct close_callback *close_callback;
  void (*(prepare))(struct nonblocking_fd *self);
  int want_read;
  void (*(read))(struct nonblocking_fd *self);
  int want_write;
  void (*(write))(struct nonblocking_fd *self);
  void (*(really_close))(struct nonblocking_fd *self);
};
extern struct ol_class nonblocking_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_nonblocking_fd_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct nonblocking_fd *i = (struct nonblocking_fd *) o;
  mark((struct ol_object *) i->next);
  mark((struct ol_object *) i->close_callback);
}

static void do_nonblocking_fd_free(struct ol_object *o)
{
  struct nonblocking_fd *i = (struct nonblocking_fd *) o;
  ol_string_free(i->fname);
}

struct ol_class nonblocking_fd_class =
{ STATIC_HEADER,
  &resource_class, "nonblocking_fd", sizeof(struct nonblocking_fd),
  do_nonblocking_fd_mark,
  do_nonblocking_fd_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct io_fd
{
  struct nonblocking_fd super;
  int fsync;
  struct read_handler *handler;
  struct abstract_buffer *buffer;
};
extern struct ol_class io_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_io_fd_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct io_fd *i = (struct io_fd *) o;
  mark((struct ol_object *) i->handler);
  mark((struct ol_object *) i->buffer);
}

struct ol_class io_fd_class =
{ STATIC_HEADER,
  &nonblocking_fd_class, "io_fd", sizeof(struct io_fd),
  do_io_fd_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct address_info
{
  struct ol_object super;
  int family;
  int (*(convert2sockaddr))(struct address_info *self, int, struct sockaddr *);
  int (*(bind_socket))(struct address_info *self, int);
  int (*(connect_socket))(struct address_info *self, int);
};
extern struct ol_class address_info_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class address_info_class =
{ STATIC_HEADER,
  0, "address_info", sizeof(struct address_info),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct unix_address_info
{
  struct address_info super;
  struct ol_string *path;
};
extern struct ol_class unix_address_info_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_unix_address_info_free(struct ol_object *o)
{
  struct unix_address_info *i = (struct unix_address_info *) o;
  ol_string_free(i->path);
}

struct ol_class unix_address_info_class =
{ STATIC_HEADER,
  &address_info_class, "unix_address_info", sizeof(struct unix_address_info),
  NULL,
  do_unix_address_info_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct inet_address_info
{
  struct address_info super;
  struct ol_string *ip;
  struct sockaddr_in sa;
  UINT32 port;
};
extern struct ol_class inet_address_info_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_inet_address_info_free(struct ol_object *o)
{
  struct inet_address_info *i = (struct inet_address_info *) o;
  ol_string_free(i->ip);
}

struct ol_class inet_address_info_class =
{ STATIC_HEADER,
  &address_info_class, "inet_address_info", sizeof(struct inet_address_info),
  NULL,
  do_inet_address_info_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct fd_listen_callback
{
  struct ol_object super;
  int (*(f))(struct fd_listen_callback *self, int, struct address_info *);
};
extern struct ol_class fd_listen_callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class fd_listen_callback_class =
{ STATIC_HEADER,
  0, "fd_listen_callback", sizeof(struct fd_listen_callback),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct listen_fd
{
  struct nonblocking_fd super;
  struct fd_listen_callback *callback;
};
extern struct ol_class listen_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_listen_fd_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct listen_fd *i = (struct listen_fd *) o;
  mark((struct ol_object *) i->callback);
}

struct ol_class listen_fd_class =
{ STATIC_HEADER,
  &nonblocking_fd_class, "listen_fd", sizeof(struct listen_fd),
  do_listen_fd_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct connect_fd
{
  struct nonblocking_fd super;
  struct fd_callback *callback;
};
extern struct ol_class connect_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_connect_fd_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct connect_fd *i = (struct connect_fd *) o;
  mark((struct ol_object *) i->callback);
}

struct ol_class connect_fd_class =
{ STATIC_HEADER,
  &nonblocking_fd_class, "connect_fd", sizeof(struct connect_fd),
  do_connect_fd_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct callback
{
  struct ol_object super;
  void (*(f))(struct callback *self);
};
extern struct ol_class callback_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class callback_class =
{ STATIC_HEADER,
  0, "callback", sizeof(struct callback),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct io_backend
{
  struct ol_object super;
  int reloading;
  struct nonblocking_fd *files;
  struct ol_queue callouts;
};
extern struct ol_class io_backend_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_io_backend_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct io_backend *i = (struct io_backend *) o;
  mark((struct ol_object *) i->files);
  do_mark_callouts(&(i->callouts), mark);
}

static void do_io_backend_free(struct ol_object *o)
{
  struct io_backend *i = (struct io_backend *) o;
  do_free_callouts(&(i->callouts));
}

struct ol_class io_backend_class =
{ STATIC_HEADER,
  0, "io_backend", sizeof(struct io_backend),
  do_io_backend_mark,
  do_io_backend_free
};
#endif /* !CLASS_DECLARE */

