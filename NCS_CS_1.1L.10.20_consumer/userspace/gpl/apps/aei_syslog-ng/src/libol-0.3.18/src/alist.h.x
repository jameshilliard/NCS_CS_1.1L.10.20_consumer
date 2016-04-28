#ifndef CLASS_DEFINE
struct alist_meta
{
  struct ol_class super;
  void * (*get)(struct alist *self, int atom);
  void (*set)(struct alist *self, int atom, void *value);
};
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DEFINE
struct alist
{
  struct ol_object super;
  unsigned size;
};
extern struct alist_meta alist_class_extended;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct alist_meta alist_class_extended =
{ { STATIC_HEADER,
  0, "alist", sizeof(struct alist),
  NULL,
  NULL
},
  NULL,
  NULL};
#define alist_class (alist_class_extended.super)
#endif /* !CLASS_DECLARE */

