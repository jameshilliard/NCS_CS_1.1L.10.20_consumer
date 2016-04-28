#ifndef CLASS_DEFINE
struct abstract_read
{
  struct ol_object super;
  int (*(read))(struct abstract_read **self, UINT32 length, UINT8 *buffer);
  int (*(recv))(struct abstract_read **self, UINT32 length, UINT8 *buffer, abstract_addr *addr, size_t *addrsize);
};
extern struct ol_class abstract_read_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class abstract_read_class =
{ STATIC_HEADER,
  0, "abstract_read", sizeof(struct abstract_read),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct read_handler
{
  struct ol_object super;
  int (*(handler))(struct read_handler **self, struct abstract_read *read);
};
extern struct ol_class read_handler_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class read_handler_class =
{ STATIC_HEADER,
  0, "read_handler", sizeof(struct read_handler),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct abstract_write
{
  struct ol_object super;
  int (*(write))(struct abstract_write *self, UINT32 length, UINT8 *data);
  int (*(writestr))(struct abstract_write *self, struct ol_string *str);
};
extern struct ol_class abstract_write_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class abstract_write_class =
{ STATIC_HEADER,
  0, "abstract_write", sizeof(struct abstract_write),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct abstract_buffer
{
  struct abstract_write super;
  int (*(writable));
  int closed;
  int (*(flush))(struct abstract_buffer *self, struct abstract_write *);
  int (*(prepare))(struct abstract_buffer *self);
  void (*(close))(struct abstract_buffer *self);
};
extern struct ol_class abstract_buffer_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class abstract_buffer_class =
{ STATIC_HEADER,
  &abstract_write_class, "abstract_buffer", sizeof(struct abstract_buffer),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct abstract_write_pipe
{
  struct abstract_write super;
  struct abstract_write *next;
};
extern struct ol_class abstract_write_pipe_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_abstract_write_pipe_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct abstract_write_pipe *i = (struct abstract_write_pipe *) o;
  mark((struct ol_object *) i->next);
}

struct ol_class abstract_write_pipe_class =
{ STATIC_HEADER,
  &abstract_write_class, "abstract_write_pipe", sizeof(struct abstract_write_pipe),
  do_abstract_write_pipe_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

