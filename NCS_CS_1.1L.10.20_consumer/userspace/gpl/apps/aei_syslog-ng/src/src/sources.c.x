#ifndef CLASS_DEFINE
struct log_reader
{
  struct read_handler super;
  UINT32 dgram;
  UINT8 (*(prefix));
  UINT32 pos;
  UINT8 (*(buffer));
  UINT32 max_log_line;
  UINT32 pad_size;
  UINT32 msg_flags;
  regex_t bad_hostname;
  struct log_handler *next;
};
extern struct ol_class log_reader_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_reader_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_reader *i = (struct log_reader *) o;
  mark((struct ol_object *) i->next);
}

static void do_log_reader_free(struct ol_object *o)
{
  struct log_reader *i = (struct log_reader *) o;
  ol_space_free(i->buffer);
  regfree(&(i->bad_hostname));
}

struct ol_class log_reader_class =
{ STATIC_HEADER,
  &read_handler_class, "log_reader", sizeof(struct log_reader),
  do_log_reader_mark,
  do_log_reader_free
};
#endif /* !CLASS_DECLARE */

