#ifndef CLASS_DEFINE
struct object_queue
{
  struct ol_object super;
  struct ol_queue q;
};
extern struct ol_class object_queue_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_object_queue_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct object_queue *i = (struct object_queue *) o;
  do_queue_mark(&(i->q), mark);
}

static void do_object_queue_free(struct ol_object *o)
{
  struct object_queue *i = (struct object_queue *) o;
  do_queue_free(&(i->q));
}

struct ol_class object_queue_class =
{ STATIC_HEADER,
  0, "object_queue", sizeof(struct object_queue),
  do_object_queue_mark,
  do_object_queue_free
};
#endif /* !CLASS_DECLARE */

