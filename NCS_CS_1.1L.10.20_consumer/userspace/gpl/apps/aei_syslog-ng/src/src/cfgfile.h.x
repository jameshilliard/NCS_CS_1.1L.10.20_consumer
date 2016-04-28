#ifndef CLASS_DEFINE
struct syslog_config
{
  struct ol_object super;
  int (*(init))(struct syslog_config *self, struct persistent_config *);
  int (*(destroy))(struct syslog_config *self, struct persistent_config *);
  struct resource_list *resources;
  struct io_backend *backend;
  int living;
  UINT32 chain_hostnames;
  UINT32 keep_hostname;
  UINT32 mark_freq;
  UINT32 sync_freq;
  UINT32 stats_freq;
  UINT32 time_sleep;
  UINT32 time_reopen;
  UINT32 time_reap;
  int log_fifo_size;
  UINT32 use_time_recvd;
  UINT32 use_fqdn;
  UINT32 use_dns;
  UINT32 check_hostname;
  UINT8 (*(bad_hostname));
  UINT32 create_dirs;
  UINT32 sanitize_filenames;
  int uid;
  int gid;
  int perm;
  int dir_uid;
  int dir_gid;
  int dir_perm;
  UINT32 dns_cache;
  UINT32 dns_cache_size;
  UINT32 dns_cache_expire;
  UINT32 dns_cache_expire_failed;
  struct nscache * cache;
  UINT32 gc_busy_threshold;
  UINT32 gc_idle_threshold;
  UINT32 log_msg_size;
  struct log_handler *internal;
  struct log_source_group *sources;
  struct log_filter *filters;
  struct log_dest_group *destinations;
  struct log_connection *last_connection;
  struct log_connection *connections;
  UINT32 file_size_limit;
};
extern struct ol_class syslog_config_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_syslog_config_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct syslog_config *i = (struct syslog_config *) o;
  mark((struct ol_object *) i->resources);
  mark((struct ol_object *) i->backend);
  mark((struct ol_object *) i->internal);
  mark((struct ol_object *) i->sources);
  mark((struct ol_object *) i->filters);
  mark((struct ol_object *) i->destinations);
  mark((struct ol_object *) i->last_connection);
  mark((struct ol_object *) i->connections);
}

struct ol_class syslog_config_class =
{ STATIC_HEADER,
  0, "syslog_config", sizeof(struct syslog_config),
  do_syslog_config_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct persistent_info
{
  struct ol_object super;
  struct ol_string *name;
  struct ol_object *o;
  void (*(kill))(struct ol_string *name, struct ol_object *o);
  struct persistent_info *next;
};
extern struct ol_class persistent_info_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_persistent_info_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct persistent_info *i = (struct persistent_info *) o;
  mark((struct ol_object *) i->o);
  mark((struct ol_object *) i->next);
}

static void do_persistent_info_free(struct ol_object *o)
{
  struct persistent_info *i = (struct persistent_info *) o;
  ol_string_free(i->name);
}

struct ol_class persistent_info_class =
{ STATIC_HEADER,
  0, "persistent_info", sizeof(struct persistent_info),
  do_persistent_info_mark,
  do_persistent_info_free
};
#endif /* !CLASS_DECLARE */

#ifndef CLASS_DEFINE
struct persistent_config
{
  struct ol_object super;
  struct persistent_info *nodes;
};
extern struct ol_class persistent_config_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
static void do_persistent_config_mark(struct ol_object *o, 
void (*mark)(struct ol_object *o))
{
  struct persistent_config *i = (struct persistent_config *) o;
  mark((struct ol_object *) i->nodes);
}

struct ol_class persistent_config_class =
{ STATIC_HEADER,
  0, "persistent_config", sizeof(struct persistent_config),
  do_persistent_config_mark,
  NULL
};
#endif /* !CLASS_DECLARE */

