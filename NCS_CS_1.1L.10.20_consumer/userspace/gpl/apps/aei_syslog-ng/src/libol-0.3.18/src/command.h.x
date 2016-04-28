#ifndef CLASS_DEFINE
struct command_continuation
{
  struct ol_object super;
  int (*(c))(struct command_continuation *self, struct ol_object *result);
};
extern struct ol_class command_continuation_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class command_continuation_class =
{ STATIC_HEADER,
  0, "command_continuation", sizeof(struct command_continuation),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command
{
  struct ol_object super;
  int (*(call))(struct command *self, struct ol_object *arg, struct command_continuation *c);
};
extern struct ol_class command_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class command_class =
{ STATIC_HEADER,
  0, "command", sizeof(struct command),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_simple
{
  struct command super;
  struct ol_object * (*(call_simple))(struct command_simple *self, struct ol_object *);
};
extern struct ol_class command_simple_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class command_simple_class =
{ STATIC_HEADER,
  &command_class, "command_simple", sizeof(struct command_simple),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_frame
{
  struct command_continuation super;
  struct command_continuation *up;
};
extern struct ol_class command_frame_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_frame_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_frame *i = (struct command_frame *) o;
  mark((struct ol_object *) i->up);
}

struct ol_class command_frame_class =
{ STATIC_HEADER,
  &command_continuation_class, "command_frame", sizeof(struct command_frame),
  do_command_frame_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_info_4
{
  struct ol_object super;
  struct ol_object * (*(f))(struct collect_info_4 *self, struct ol_object *, struct ol_object *, struct ol_object *, struct ol_object *);
};
extern struct ol_class collect_info_4_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class collect_info_4_class =
{ STATIC_HEADER,
  0, "collect_info_4", sizeof(struct collect_info_4),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_info_3
{
  struct ol_object super;
  struct ol_object * (*(f))(struct collect_info_3 *self, struct ol_object *, struct ol_object *, struct ol_object *);
  struct collect_info_4 *next;
};
extern struct ol_class collect_info_3_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_info_3_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_info_3 *i = (struct collect_info_3 *) o;
  mark((struct ol_object *) i->next);
}

struct ol_class collect_info_3_class =
{ STATIC_HEADER,
  0, "collect_info_3", sizeof(struct collect_info_3),
  do_collect_info_3_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_info_2
{
  struct ol_object super;
  struct ol_object * (*(f))(struct collect_info_2 *self, struct ol_object *, struct ol_object *);
  struct collect_info_3 *next;
};
extern struct ol_class collect_info_2_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_info_2_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_info_2 *i = (struct collect_info_2 *) o;
  mark((struct ol_object *) i->next);
}

struct ol_class collect_info_2_class =
{ STATIC_HEADER,
  0, "collect_info_2", sizeof(struct collect_info_2),
  do_collect_info_2_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct collect_info_1
{
  struct command_simple super;
  struct ol_object * (*(f))(struct collect_info_1 *self, struct ol_object *);
  struct collect_info_2 *next;
};
extern struct ol_class collect_info_1_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_collect_info_1_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct collect_info_1 *i = (struct collect_info_1 *) o;
  mark((struct ol_object *) i->next);
}

struct ol_class collect_info_1_class =
{ STATIC_HEADER,
  &command_simple_class, "collect_info_1", sizeof(struct collect_info_1),
  do_collect_info_1_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

