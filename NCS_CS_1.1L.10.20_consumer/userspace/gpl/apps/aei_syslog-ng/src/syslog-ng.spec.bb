#
#  Specfile used by BalaBit internally.
#
Summary: Next generation system logging daemon
Name: syslog-ng
Version: 1.6.12
Release: 1
License: GPL
Group: System Environment/Daemons
Source: syslog-ng_%{version}.tar.gz
URL: http://www.balabit.com
Packager: Tamas Pal <folti@balabit.com>
Vendor: Balabit IT Ltd.
BuildRoot: %{_tmppath}/%{name}-root
BuildRequires: bison, flex, gcc-c++, libnet
Provides: syslog
#Exclusivearch: i386

%define prefix /

%description
 Syslog-ng is a next generation system logger daemon which provides more
 capabilities and is has a more flexible configuration then the traditional
 syslog daemon.

%prep
%setup -q -n syslog-ng-%{version}

%build

# build syslog-ng using the bundled libol

./configure --prefix=/ --mandir=/usr/share/man --infodir=/usr/share/info \
  --sysconfdir=/etc --enable-spoof-source
make

%install
make install DESTDIR="$RPM_BUILD_ROOT"
# strip the binaries/ libraries
strip ${RPM_BUILD_ROOT}/sbin/syslog-ng

install -D -o root -g root -m 0644 contrib/rhel-packaging/syslog-ng.conf \
		  ${RPM_BUILD_ROOT}/etc/syslog-ng/syslog-ng.conf
if [ "%{_host_vendor}" = "redhat" ];then
	install -D -o root -g root -m 0755 contrib/rhel-packaging/syslog-ng.init \
		  ${RPM_BUILD_ROOT}/etc/rc.d/init.d/syslog-ng
elif [ "%{_host_vendor}" = "suse" ];then
	install -D -o root -g root -m 0755 contrib/init.d.SuSE \
		  ${RPM_BUILD_ROOT}/etc/rc.d/init.d/syslog-ng
fi

install -D -o root -g root -m 0644 contrib/rhel-packaging/syslog-ng.logrotate \
		  ${RPM_BUILD_ROOT}/etc/logrotate.d/syslog-ng

# install documentation
install -D -o root -g root -m 0644 doc/sgml/syslog-ng.html.tar.gz \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/syslog-ng.html.tar.gz
install -D -o root -g root -m 0644 doc/sgml/syslog-ng.txt \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/syslog-ng.txt
install -D -o root -g root -m 0644 ChangeLog \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/ChangeLog
install -D -o root -g root -m 0644 NEWS \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/NEWS
install -D -o root -g root -m 0644 README \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/README
install -D -o root -g root -m 0644 AUTHORS \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/AUTHORS
install -D -o root -g root -m 0644 COPYING \
  ${RPM_BUILD_ROOT}/usr/share/doc/syslog-ng-1.6.12/COPYING

%files
%defattr(-,root,root)
/sbin/syslog-ng
/etc/rc.d/init.d/syslog-ng
/usr/share/man/*
%docdir /usr/share/doc/syslog-ng-1.6.12
/usr/share/doc/syslog-ng-1.6.12/*
%config(noreplace) /etc/syslog-ng/syslog-ng.conf
%config(noreplace) /etc/logrotate.d/syslog-ng

%post
if [ "%{_host_vendor}" = "redhat" ];then
	sh /etc/rc.d/init.d/syslog stop || true
elif [ "%{_host_vendor}" = "suse" ];then
	sh /etc/init.d/syslog stop || true
	if [ -L /etc/init.d/syslog-ng ]; then
	    rm -f /etc/init.d/syslog-ng
	fi
	ln -s /etc/rc.d/init.d/syslog-ng /etc/init.d/syslog-ng
fi
chkconfig --force --del syslog
chkconfig --add syslog-ng
sh /etc/rc.d/init.d/syslog-ng start || exit 0

%preun
sh /etc/rc.d/init.d/syslog-ng stop || exit 0 
if [ "%{_host_vendor}" = "redhat" ];then
	sh /etc/rc.d/init.d/syslog start || true
elif [ "%{_host_vendor}" = "suse" ];then
	if [ -L /etc/init.d/syslog-ng ]; then
	    rm -f /etc/init.d/syslog-ng
	fi
	sh /etc/init.d/syslog start || true
fi

%postun
chkconfig --del syslog-ng
chkconfig --add syslog

%check
%clean
[ $RPM_BUILD_ROOT = / ] || rm -rf $RPM_BUILD_ROOT

%changelog
* Fri Nov 03 2006 Tamas Pal <folti@balabit.com>
- Added SuSE packaging.
- Changed Packager.
- Install no longer overwrites syslog-ng.conf and logrotate config.
- Provides syslog.
- Updated post, pstun, preun scripts.
- enabled spoof source option
* Fri Jun 30 2006 Tamas Pal <folti@balabit.com>
- fixed typo in RHEL config file.
* Mon Mar 27 2006 Balazs Scheidler <bazsi@balabit.com>
- removed postscript version of the documentation
* Fri Sep 9 2005 Sandor Geller <wildy@balabit.com>
- fixed permissions of /etc/rc.d/init.d/syslog-ng
* Thu Jun 30 2005 Sandor Geller <wildy@balabit.com>
- packaging fixes, added logrotate script
* Thu Jun 23 2005 Sandor Geller <wildy@balabit.com>
- added upstream's documentation to the package
* Mon Jun 20 2005 Sandor Geller <wildy@balabit.com>
- initial RPM packaging for RHEL ES

# vim: ft=spec ts=2
