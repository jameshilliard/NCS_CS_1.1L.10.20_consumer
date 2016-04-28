#ifndef CLASS_DEFINE
struct line_handler
{
  struct ol_object super;
  int (*(handler))(struct line_handler **self, struct read_handler **r, UINT32 length, UINT8 *line);
};
extern struct ol_class line_handler_class;
#endif /* !CLASS_DEFINE */

#ifndef CLASS_DECLARE
struct ol_class line_handler_class =
{ STATIC_HEADER,
  0, "line_handler", sizeof(struct line_handler),
  NULL,
  NULL
};
#endif /* !CLASS_DECLARE */

