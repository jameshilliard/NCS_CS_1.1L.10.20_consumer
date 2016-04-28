#ifndef CLASS_DEFINE
struct resource
{
  struct ol_object super;
  int alive;
  void (*(kill))(struct resource *self);
};
extern struct ol_class resource_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_resource_free(struct ol_object *o)
{
  struct resource *i = (struct resource *) o;
  dont_free_live_resource(i->alive);
}

struct ol_class resource_class =
{ STATIC_HEADER,
  0, "resource", sizeof(struct resource),
  NULL,
  do_resource_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct resource_list
{
  struct ol_object super;
  struct ol_queue q;
  struct resource_node * (*(remember))(struct resource_list *self, struct resource *r);
  void (*(kill_resource))(struct resource_list *self, struct resource_node *n);
  void (*(kill_all))(struct resource_list *self);
};
extern struct ol_class resource_list_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_resource_list_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct resource_list *i = (struct resource_list *) o;
  do_mark_resources(&(i->q), mark);
}

static void do_resource_list_free(struct ol_object *o)
{
  struct resource_list *i = (struct resource_list *) o;
  do_free_resources(&(i->q));
}

struct ol_class resource_list_class =
{ STATIC_HEADER,
  0, "resource_list", sizeof(struct resource_list),
  do_resource_list_mark,
  do_resource_list_free
};
#endif /* !CLASS_DECLARE */

