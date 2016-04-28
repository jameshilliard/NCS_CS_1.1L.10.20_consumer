#ifndef CLASS_DEFINE
struct read_line
{
  struct read_handler super;
  struct line_handler *handler;
  UINT32 pos;
  UINT8 ((buffer)[MAX_LINE]);
};
extern struct ol_class read_line_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_read_line_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct read_line *i = (struct read_line *) o;
  mark((struct ol_object *) i->handler);
}

struct ol_class read_line_class =
{ STATIC_HEADER,
  &read_handler_class, "read_line", sizeof(struct read_line),
  do_read_line_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct string_read
{
  struct abstract_read super;
  struct read_line *line;
  UINT32 index;
};
extern struct ol_class string_read_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_string_read_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct string_read *i = (struct string_read *) o;
  mark((struct ol_object *) i->line);
}

struct ol_class string_read_class =
{ STATIC_HEADER,
  &abstract_read_class, "string_read", sizeof(struct string_read),
  do_string_read_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

