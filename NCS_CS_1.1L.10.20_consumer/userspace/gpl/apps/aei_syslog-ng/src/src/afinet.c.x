#ifndef CLASS_DEFINE
struct afinet_source
{
  struct afsocket_source super;
  struct listen_fd *listen;
  UINT32 so_rcvbuf;
  struct syslog_config *cfg;
};
extern struct ol_class afinet_source_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afinet_source_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afinet_source *i = (struct afinet_source *) o;
  mark((struct ol_object *) i->listen);
  mark((struct ol_object *) i->cfg);
}

struct ol_class afinet_source_class =
{ STATIC_HEADER,
  &afsocket_source_class, "afinet_source", sizeof(struct afinet_source),
  do_afinet_source_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct afinet_dest
{
  struct afsocket_dest super;
  struct io_fd *dest;
  int spoof_source;
  libnet_t * lnet_ctx;
  struct abstract_buffer *dest_buf;
  UINT32 sync_freq;
  struct connect_fd *conn_fd;
  struct ol_string *template_output;
  int template_escape;
  struct syslog_config *cfg;
};
extern struct ol_class afinet_dest_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_afinet_dest_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct afinet_dest *i = (struct afinet_dest *) o;
  mark((struct ol_object *) i->dest);
  mark((struct ol_object *) i->dest_buf);
  mark((struct ol_object *) i->conn_fd);
  mark((struct ol_object *) i->cfg);
}

static void do_afinet_dest_free(struct ol_object *o)
{
  struct afinet_dest *i = (struct afinet_dest *) o;
  ol_string_free(i->template_output);
}

struct ol_class afinet_dest_class =
{ STATIC_HEADER,
  &afsocket_dest_class, "afinet_dest", sizeof(struct afinet_dest),
  do_afinet_dest_mark,
  do_afinet_dest_free
};
#endif /* !CLASS_DECLARE */

