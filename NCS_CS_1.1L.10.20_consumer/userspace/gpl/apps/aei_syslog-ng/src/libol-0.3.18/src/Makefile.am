
lib_LTLIBRARIES = libol.la

dist_c_x_files = \
	alist.c.x \
	io.c.x \
	pkt_buffer.c.x \
	read_line.c.x \
	stream_buffer.c.x \
	command.c.x \
	io_commands.c.x \
	combinators.c.x

dist_h_x_files = \
	abstract_io.h.x \
	alist.h.x \
	io.h.x \
	list.h.x \
	read_line.h.x \
	resource.h.x \
	io_commands.h.x \
	command.h.x \
	queue.h.x

dist_h_files = abstract_io.h alist.h format.h gc.h io.h jpoll.h list.h \
	objbase.h objects.h objtypes.h pkt_buffer.h queue.h \
	read_line.h resource.h stream_buffer.h werror.h xalloc.h \
	command.h io_commands.h

pkginclude_HEADERS = $(dist_h_files) $(dist_h_x_files) olconfig.h

noinst_HEADERS = $(dist_c_x_files)

libol_la_SOURCES = format.c io.c werror.c queue.c \
	abstract_io.c gc.c list.c read_line.c xalloc.c alist.c stream_buffer.c \
	pkt_buffer.c resource.c
# combinators.c io_commands.c command.c

libol_la_LIBADD = @LIBOBJS@

EXTRA_DIST = olconfig.h.in jpoll.c

#.deps/%.P : %.c
#	$(CC) $(CPPFLAGS) $(DEFS) -M -MG $< > $@

%.x: %
	$(MAKE_CLASS) <$< >$@t
	test -s $@t || rm $@t
	test -s $@t && mv $@t $@
