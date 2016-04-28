#ifndef CLASS_DEFINE
struct filter_expr_op
{
  struct filter_expr_node super;
  struct filter_expr_node *left;
  struct filter_expr_node *right;
};
extern struct ol_class filter_expr_op_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_filter_expr_op_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct filter_expr_op *i = (struct filter_expr_op *) o;
  mark((struct ol_object *) i->left);
  mark((struct ol_object *) i->right);
}

struct ol_class filter_expr_op_class =
{ STATIC_HEADER,
  &filter_expr_node_class, "filter_expr_op", sizeof(struct filter_expr_op),
  do_filter_expr_op_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct filter_expr_pri
{
  struct filter_expr_node super;
  UINT32 valid;
};
extern struct ol_class filter_expr_pri_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class filter_expr_pri_class =
{ STATIC_HEADER,
  &filter_expr_node_class, "filter_expr_pri", sizeof(struct filter_expr_pri),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct filter_expr_re
{
  struct filter_expr_node super;
  regex_t regex;
};
extern struct ol_class filter_expr_re_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_filter_expr_re_free(struct ol_object *o)
{
  struct filter_expr_re *i = (struct filter_expr_re *) o;
  free_regexp(&(i->regex));
}

struct ol_class filter_expr_re_class =
{ STATIC_HEADER,
  &filter_expr_node_class, "filter_expr_re", sizeof(struct filter_expr_re),
  NULL,
  do_filter_expr_re_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct filter_expr_netmask
{
  struct filter_expr_node super;
  struct in_addr network;
  struct in_addr netmask;
};
extern struct ol_class filter_expr_netmask_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class filter_expr_netmask_class =
{ STATIC_HEADER,
  &filter_expr_node_class, "filter_expr_netmask", sizeof(struct filter_expr_netmask),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct filter_expr_call
{
  struct filter_expr_node super;
  struct ol_string *name;
  struct log_filter *call_rule;
  UINT32 error_logged;
};
extern struct ol_class filter_expr_call_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_filter_expr_call_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct filter_expr_call *i = (struct filter_expr_call *) o;
  mark((struct ol_object *) i->call_rule);
}

static void do_filter_expr_call_free(struct ol_object *o)
{
  struct filter_expr_call *i = (struct filter_expr_call *) o;
  ol_string_free(i->name);
}

struct ol_class filter_expr_call_class =
{ STATIC_HEADER,
  &filter_expr_node_class, "filter_expr_call", sizeof(struct filter_expr_call),
  do_filter_expr_call_mark,
  do_filter_expr_call_free
};
#endif /* !CLASS_DECLARE */

