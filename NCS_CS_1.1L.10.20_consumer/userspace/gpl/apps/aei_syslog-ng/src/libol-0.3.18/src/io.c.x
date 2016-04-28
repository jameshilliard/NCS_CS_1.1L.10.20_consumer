#ifndef CLASS_DEFINE
struct fd_read
{
  struct abstract_read super;
  int fd;
};
extern struct ol_class fd_read_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class fd_read_class =
{ STATIC_HEADER,
  &abstract_read_class, "fd_read", sizeof(struct fd_read),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct fd_write
{
  struct abstract_write super;
  int fd;
  int fsync;
};
extern struct ol_class fd_write_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class fd_write_class =
{ STATIC_HEADER,
  &abstract_write_class, "fd_write", sizeof(struct fd_write),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

