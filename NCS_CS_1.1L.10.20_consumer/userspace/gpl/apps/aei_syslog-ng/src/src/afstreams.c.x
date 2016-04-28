#ifndef CLASS_DEFINE
struct door_fd
{
  struct resource super;
  int fd;
};
extern struct ol_class door_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class door_fd_class =
{ STATIC_HEADER,
  &resource_class, "door_fd", sizeof(struct door_fd),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct stream_fd
{
  struct nonblocking_fd super;
  regex_t bad_hostname;
  UINT32 max_log_line;
  struct log_handler *pipe;
};
extern struct ol_class stream_fd_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_stream_fd_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct stream_fd *i = (struct stream_fd *) o;
  mark((struct ol_object *) i->pipe);
}

static void do_stream_fd_free(struct ol_object *o)
{
  struct stream_fd *i = (struct stream_fd *) o;
  regfree(&(i->bad_hostname));
}

struct ol_class stream_fd_class =
{ STATIC_HEADER,
  &nonblocking_fd_class, "stream_fd", sizeof(struct stream_fd),
  do_stream_fd_mark,
  do_stream_fd_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afstreams_source
{
  struct log_source_driver super;
  struct ol_string *name;
  struct ol_string *door_name;
  struct nonblocking_fd *stream_fd;
  struct door_fd *door;
};
extern struct ol_class afstreams_source_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afstreams_source_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afstreams_source *i = (struct afstreams_source *) o;
  mark((struct ol_object *) i->stream_fd);
  mark((struct ol_object *) i->door);
}

static void do_afstreams_source_free(struct ol_object *o)
{
  struct afstreams_source *i = (struct afstreams_source *) o;
  ol_string_free(i->name);
  ol_string_free(i->door_name);
}

struct ol_class afstreams_source_class =
{ STATIC_HEADER,
  &log_source_driver_class, "afstreams_source", sizeof(struct afstreams_source),
  do_afstreams_source_mark,
  do_afstreams_source_free
};
#endif /* !CLASS_DECLARE */

