#ifndef CLASS_DEFINE
struct command_apply
{
  struct command_frame super;
  struct command *f;
};
extern struct ol_class command_apply_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_apply_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_apply *i = (struct command_apply *) o;
  mark((struct ol_object *) i->f);
}

struct ol_class command_apply_class =
{ STATIC_HEADER,
  &command_frame_class, "command_apply", sizeof(struct command_apply),
  do_command_apply_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_state_1
{
  struct command_simple super;
  struct collect_info_2 *info;
  struct ol_object *a;
};
extern struct ol_class collect_state_1_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_state_1_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_state_1 *i = (struct collect_state_1 *) o;
  mark((struct ol_object *) i->info);
  mark((struct ol_object *) i->a);
}

struct ol_class collect_state_1_class =
{ STATIC_HEADER,
  &command_simple_class, "collect_state_1", sizeof(struct collect_state_1),
  do_collect_state_1_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_state_2
{
  struct command_simple super;
  struct collect_info_3 *info;
  struct ol_object *a;
  struct ol_object *b;
};
extern struct ol_class collect_state_2_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_state_2_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_state_2 *i = (struct collect_state_2 *) o;
  mark((struct ol_object *) i->info);
  mark((struct ol_object *) i->a);
  mark((struct ol_object *) i->b);
}

struct ol_class collect_state_2_class =
{ STATIC_HEADER,
  &command_simple_class, "collect_state_2", sizeof(struct collect_state_2),
  do_collect_state_2_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_state_3
{
  struct command_simple super;
  struct collect_info_4 *info;
  struct ol_object *a;
  struct ol_object *b;
  struct ol_object *c;
};
extern struct ol_class collect_state_3_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_state_3_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_state_3 *i = (struct collect_state_3 *) o;
  mark((struct ol_object *) i->info);
  mark((struct ol_object *) i->a);
  mark((struct ol_object *) i->b);
  mark((struct ol_object *) i->c);
}

struct ol_class collect_state_3_class =
{ STATIC_HEADER,
  &command_simple_class, "collect_state_3", sizeof(struct collect_state_3),
  do_collect_state_3_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct parallell_progn
{
  struct command super;
  struct object_list *body;
};
extern struct ol_class parallell_progn_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_parallell_progn_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct parallell_progn *i = (struct parallell_progn *) o;
  mark((struct ol_object *) i->body);
}

struct ol_class parallell_progn_class =
{ STATIC_HEADER,
  &command_class, "parallell_progn", sizeof(struct parallell_progn),
  do_parallell_progn_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

