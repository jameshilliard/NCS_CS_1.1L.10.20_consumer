## Run automake to produce Makefile.in
## $Id: Makefile.am,v 1.48 2003/04/10 16:40:00 bazsi Exp $

YFLAGS = @YFLAGS@
MAKE_CLASS = @MAKE_CLASS@

SUBDIRS = . tests

dist_h_files = \
	syslog-ng.h \
	cfgfile.h \
	sources.h \
	center.h \
	filters.h \
	destinations.h \
	log.h \
	afinter.h \
	affile.h \
	afsocket.h \
	afuser.h \
	afstreams.h \
	afprogram.h \
	afremctrl.h \
	utils.h \
	syslog-names.h \
	nscache.h \
	macros.h

dist_x_files = \
	cfgfile.h.x \
	destinations.h.x \
	log.h.x \
	sources.c.x \
	sources.h.x \
	cfgfile.h.x \
	center.h.x \
	center.c.x \
	afsocket.h.x \
	afsocket.c.x \
	afunix.c.x \
	afinet.c.x \
	affile.c.x \
	afuser.c.x \
	afstreams.c.x \
	afprogram.c.x \
	afremctrl.c.x \
	filters.h.x \
	filters.c.x \
	main.c.x

AUTOMAKE_OPTIONS = foreign
sbin_PROGRAMS = syslog-ng

syslog_ng_SOURCES = \
	main.c \
	sources.c \
	center.c \
	filters.c \
	destinations.c \
	log.c \
	cfgfile.c \
	cfg-grammar.y \
	cfg-lex.l \
	affile.c \
	afsocket.c \
	afunix.c \
	afinet.c \
	afinter.c \
	afuser.c \
	afstreams.c \
	afprogram.c \
	afremctrl.c \
	nscache.c \
	utils.c \
	syslog-names.c \
	macros.c

syslog_ng_LDADD = @LIBOBJS@

EXTRA_DIST = $(dist_x_files) $(dist_h_files) \
	getopt.c getopt.h getopt1.c strcasecmp.c \
	cfg-grammar.h snprintf.c strptime.c strptime.h \
	macros-gperf.c macros.gprf

cfg-lex.l: cfg-grammar.h

##.deps/%.P : %.c
##	$(CC) $(CPPFLAGS) $(DEFS) -M -MG $< > $@

%.x: %
	$(MAKE_CLASS) <$< >$@T
	if [ -s $@T ]; then mv $@T $@; else rm $@T; fi

macros-gperf.c: macros.gprf
	gperf -LANSI-C -t -c -l -k1,3,5  -N find_macro < macros.gprf >macros-gperf.c

macros.c: macros-gperf.c
