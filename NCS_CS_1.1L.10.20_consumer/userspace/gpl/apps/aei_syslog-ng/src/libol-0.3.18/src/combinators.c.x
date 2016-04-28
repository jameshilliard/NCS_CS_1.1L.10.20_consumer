#ifndef CLASS_DEFINE
struct command_K_1
{
  struct command_simple super;
  struct ol_object *x;
};
extern struct ol_class command_K_1_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_K_1_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_K_1 *i = (struct command_K_1 *) o;
  mark((struct ol_object *) i->x);
}

struct ol_class command_K_1_class =
{ STATIC_HEADER,
  &command_simple_class, "command_K_1", sizeof(struct command_K_1),
  do_command_K_1_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_S_2
{
  struct command_simple super;
  struct command *f;
  struct command *g;
};
extern struct ol_class command_S_2_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_S_2_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_S_2 *i = (struct command_S_2 *) o;
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->g);
}

struct ol_class command_S_2_class =
{ STATIC_HEADER,
  &command_simple_class, "command_S_2", sizeof(struct command_S_2),
  do_command_S_2_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_S_continuation
{
  struct command_frame super;
  struct command *g;
  struct ol_object *x;
};
extern struct ol_class command_S_continuation_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_S_continuation_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_S_continuation *i = (struct command_S_continuation *) o;
  mark((struct ol_object *) i->g);
  mark((struct ol_object *) i->x);
}

struct ol_class command_S_continuation_class =
{ STATIC_HEADER,
  &command_frame_class, "command_S_continuation", sizeof(struct command_S_continuation),
  do_command_S_continuation_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_Sp_3
{
  struct command_simple super;
  struct command *c;
  struct command *f;
  struct command *g;
};
extern struct ol_class command_Sp_3_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_Sp_3_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_Sp_3 *i = (struct command_Sp_3 *) o;
  mark((struct ol_object *) i->c);
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->g);
}

struct ol_class command_Sp_3_class =
{ STATIC_HEADER,
  &command_simple_class, "command_Sp_3", sizeof(struct command_Sp_3),
  do_command_Sp_3_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_B_2
{
  struct command_simple super;
  struct command *f;
  struct command *g;
};
extern struct ol_class command_B_2_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_B_2_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_B_2 *i = (struct command_B_2 *) o;
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->g);
}

struct ol_class command_B_2_class =
{ STATIC_HEADER,
  &command_simple_class, "command_B_2", sizeof(struct command_B_2),
  do_command_B_2_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_Bp_3
{
  struct command_simple super;
  struct command *c;
  struct command *f;
  struct command *g;
};
extern struct ol_class command_Bp_3_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_Bp_3_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_Bp_3 *i = (struct command_Bp_3 *) o;
  mark((struct ol_object *) i->c);
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->g);
}

struct ol_class command_Bp_3_class =
{ STATIC_HEADER,
  &command_simple_class, "command_Bp_3", sizeof(struct command_Bp_3),
  do_command_Bp_3_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_C_2
{
  struct command_simple super;
  struct command *f;
  struct ol_object *y;
};
extern struct ol_class command_C_2_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_C_2_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_C_2 *i = (struct command_C_2 *) o;
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->y);
}

struct ol_class command_C_2_class =
{ STATIC_HEADER,
  &command_simple_class, "command_C_2", sizeof(struct command_C_2),
  do_command_C_2_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_C_continuation
{
  struct command_frame super;
  struct ol_object *y;
};
extern struct ol_class command_C_continuation_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_C_continuation_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_C_continuation *i = (struct command_C_continuation *) o;
  mark((struct ol_object *) i->y);
}

struct ol_class command_C_continuation_class =
{ STATIC_HEADER,
  &command_frame_class, "command_C_continuation", sizeof(struct command_C_continuation),
  do_command_C_continuation_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct command_Cp_3
{
  struct command_simple super;
  struct command *c;
  struct command *f;
  struct ol_object *y;
};
extern struct ol_class command_Cp_3_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_command_Cp_3_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct command_Cp_3 *i = (struct command_Cp_3 *) o;
  mark((struct ol_object *) i->c);
  mark((struct ol_object *) i->f);
  mark((struct ol_object *) i->y);
}

struct ol_class command_Cp_3_class =
{ STATIC_HEADER,
  &command_simple_class, "command_Cp_3", sizeof(struct command_Cp_3),
  do_command_Cp_3_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

