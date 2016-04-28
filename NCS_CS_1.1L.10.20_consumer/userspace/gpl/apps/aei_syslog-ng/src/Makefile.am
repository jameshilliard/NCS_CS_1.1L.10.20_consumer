SUBDIRS = src doc contrib 

EXTRA_DIST = PORTS syslog-ng.spec syslog-ng.spec.in syslog-ng.spec.bb syslog-ng.spec.bb.in README.spoof dist.conf dist.conf.in

# bundle libol to the resulting tarball
dist-hook:
	if test -n "$(LIBOL_DIR)" ; then \
		test -f $(LIBOL_DIR)/libol-$(LIBOL_VERSION).tar.gz || (cd $(LIBOL_DIR); make dist); \
		tar xfz $(LIBOL_DIR)/libol-$(LIBOL_VERSION).tar.gz -C $(distdir); \
	fi
	cp -a debian/ solbuild/ $(distdir)/
	$(top_builddir)/config.status --file $(distdir)/debian/changelog:debian/changelog.in
	$(top_builddir)/config.status --file $(distdir)/solbuild/pkginfo:solbuild/pkginfo.in
