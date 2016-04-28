
EXTRA_DIST = syslog-ng.xml \
	syslog-ng.txt syslog-ng.html.tar.gz

syslog-ng.txt: syslog-ng.html.tar.gz
	w3m -dump syslog-ng.html/index.html > $@

syslog-ng.html.tar.gz: syslog-ng.xml 
	(set -e;                                                \
	xsltproc --nonet --path $(top_builddir)/doc/sgml -o syslog-ng.html/index.html      \
		/usr/share/xml/docbook/stylesheet/nwalsh/html/docbook.xsl		   \
		$(top_srcdir)/doc/sgml/syslog-ng.xml;      \
	tar cfz syslog-ng.html.tar.gz syslog-ng.html)
