#ifndef CLASS_DEFINE
struct log_filter
{
  struct ol_object super;
  struct ol_string *name;
  struct filter_expr_node *root;
  struct log_filter *next_filter;
  struct log_filter *prev_filter;
};
extern struct ol_class log_filter_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_log_filter_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct log_filter *i = (struct log_filter *) o;
  mark((struct ol_object *) i->root);
  mark((struct ol_object *) i->next_filter);
  mark((struct ol_object *) i->prev_filter);
}

static void do_log_filter_free(struct ol_object *o)
{
  struct log_filter *i = (struct log_filter *) o;
  ol_string_free(i->name);
}

struct ol_class log_filter_class =
{ STATIC_HEADER,
  0, "log_filter", sizeof(struct log_filter),
  do_log_filter_mark,
  do_log_filter_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct filter_expr_node
{
  struct ol_object super;
  int comp;
  int (*(eval))(struct filter_expr_node *self, struct log_filter *, struct log_info *);
};
extern struct ol_class filter_expr_node_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class filter_expr_node_class =
{ STATIC_HEADER,
  0, "filter_expr_node", sizeof(struct filter_expr_node),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

