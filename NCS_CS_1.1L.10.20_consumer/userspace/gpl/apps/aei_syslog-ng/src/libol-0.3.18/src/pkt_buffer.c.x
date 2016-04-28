#ifndef CLASS_DEFINE
struct pkt_buffer
{
  struct abstract_buffer super;
  struct ol_queue queue;
  int pkt_flush;
  int queue_min;
  int queue_size;
  int queue_max;
};
extern struct ol_class pkt_buffer_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_pkt_buffer_free(struct ol_object *o)
{
  struct pkt_buffer *i = (struct pkt_buffer *) o;
  do_free_buffer(&(i->queue));
}

struct ol_class pkt_buffer_class =
{ STATIC_HEADER,
  &abstract_buffer_class, "pkt_buffer", sizeof(struct pkt_buffer),
  NULL,
  do_pkt_buffer_free
};
#endif /* !CLASS_DECLARE */

