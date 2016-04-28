#ifndef CLASS_DEFINE
struct stream_buffer
{
  struct abstract_buffer super;
  UINT32 block_size;
  UINT8 (*(buffer));
  int empty;
  UINT32 length;
  struct ol_queue q;
  UINT32 pos;
  struct ol_string *partial;
  UINT32 start;
  UINT32 end;
};
extern struct ol_class stream_buffer_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_stream_buffer_free(struct ol_object *o)
{
  struct stream_buffer *i = (struct stream_buffer *) o;
  ol_space_free(i->buffer);
  do_free_buffer(&(i->q));
  ol_string_free(i->partial);
}

struct ol_class stream_buffer_class =
{ STATIC_HEADER,
  &abstract_buffer_class, "stream_buffer", sizeof(struct stream_buffer),
  NULL,
  do_stream_buffer_free
};
#endif /* !CLASS_DECLARE */

