#ifndef CLASS_DEFINE
struct afunix_source
{
  struct afsocket_source super;
  int uid;
  int gid;
  int perm;
  struct listen_fd *listen;
  struct syslog_config *cfg;
};
extern struct ol_class afunix_source_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afunix_source_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afunix_source *i = (struct afunix_source *) o;
  mark((struct ol_object *) i->listen);
  mark((struct ol_object *) i->cfg);
}

struct ol_class afunix_source_class =
{ STATIC_HEADER,
  &afsocket_source_class, "afunix_source", sizeof(struct afunix_source),
  do_afunix_source_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afunix_dest
{
  struct afsocket_dest super;
  struct io_fd *dest;
  struct abstract_buffer *dest_buf;
  struct ol_string *template_output;
  int template_escape;
  struct syslog_config *cfg;
};
extern struct ol_class afunix_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afunix_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afunix_dest *i = (struct afunix_dest *) o;
  mark((struct ol_object *) i->dest);
  mark((struct ol_object *) i->dest_buf);
  mark((struct ol_object *) i->cfg);
}

static void do_afunix_dest_free(struct ol_object *o)
{
  struct afunix_dest *i = (struct afunix_dest *) o;
  ol_string_free(i->template_output);
}

struct ol_class afunix_dest_class =
{ STATIC_HEADER,
  &afsocket_dest_class, "afunix_dest", sizeof(struct afunix_dest),
  do_afunix_dest_mark,
  do_afunix_dest_free
};
#endif /* !CLASS_DECLARE */

