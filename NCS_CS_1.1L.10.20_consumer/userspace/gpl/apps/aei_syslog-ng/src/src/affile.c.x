#ifndef CLASS_DEFINE
struct affile_source
{
  struct log_source_driver super;
  UINT32 flags;
  struct io_fd *src;
  UINT8 (*(prefix));
  UINT32 pad_size;
  struct ol_string *name;
};
extern struct ol_class affile_source_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_affile_source_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct affile_source *i = (struct affile_source *) o;
  mark((struct ol_object *) i->src);
}

static void do_affile_source_free(struct ol_object *o)
{
  struct affile_source *i = (struct affile_source *) o;
  ol_string_free(i->name);
}

struct ol_class affile_source_class =
{ STATIC_HEADER,
  &log_source_driver_class, "affile_source", sizeof(struct affile_source),
  do_affile_source_mark,
  do_affile_source_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct affile_dest_reaper
{
  struct callback super;
  struct syslog_config *cfg;
  struct affile_dest_writer *affile_dest;
};
extern struct ol_class affile_dest_reaper_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_affile_dest_reaper_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct affile_dest_reaper *i = (struct affile_dest_reaper *) o;
  mark((struct ol_object *) i->cfg);
  mark((struct ol_object *) i->affile_dest);
}

struct ol_class affile_dest_reaper_class =
{ STATIC_HEADER,
  &callback_class, "affile_dest_reaper", sizeof(struct affile_dest_reaper),
  do_affile_dest_reaper_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct affile_dest_writer
{
  struct log_handler super;
  struct resource_node (*(res));
  UINT32 time_reap;
  struct affile_dest *owner;
  struct io_fd *dest;
  struct callout * reap;
  struct ol_string *expanded_fname;
  struct affile_dest_writer *prev;
  struct affile_dest_writer *next;
};
extern struct ol_class affile_dest_writer_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_affile_dest_writer_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct affile_dest_writer *i = (struct affile_dest_writer *) o;
  mark((struct ol_object *) i->owner);
  mark((struct ol_object *) i->dest);
  mark((struct ol_object *) i->prev);
  mark((struct ol_object *) i->next);
}

static void do_affile_dest_writer_free(struct ol_object *o)
{
  struct affile_dest_writer *i = (struct affile_dest_writer *) o;
  ol_string_free(i->expanded_fname);
}

struct ol_class affile_dest_writer_class =
{ STATIC_HEADER,
  &log_handler_class, "affile_dest_writer", sizeof(struct affile_dest_writer),
  do_affile_dest_writer_mark,
  do_affile_dest_writer_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct affile_dest
{
  struct log_dest_driver super;
  int uid;
  int gid;
  int perm;
  int dir_uid;
  int dir_gid;
  int dir_perm;
  int flags;
  int remove_if_older;
  int sync_freq;
int size_limit;
  struct ol_string *template_fname;
  struct ol_string *template_output;
  int template_escape;
  struct affile_dest_writer *writers;
  struct syslog_config *cfg;
};
extern struct ol_class affile_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_affile_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct affile_dest *i = (struct affile_dest *) o;
  mark((struct ol_object *) i->writers);
  mark((struct ol_object *) i->cfg);
}

static void do_affile_dest_free(struct ol_object *o)
{
  struct affile_dest *i = (struct affile_dest *) o;
  ol_string_free(i->template_fname);
  ol_string_free(i->template_output);
}

struct ol_class affile_dest_class =
{ STATIC_HEADER,
  &log_dest_driver_class, "affile_dest", sizeof(struct affile_dest),
  do_affile_dest_mark,
  do_affile_dest_free
};
#endif /* !CLASS_DECLARE */

