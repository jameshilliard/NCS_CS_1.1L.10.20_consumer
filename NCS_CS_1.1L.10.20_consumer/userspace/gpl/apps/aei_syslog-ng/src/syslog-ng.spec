#--------------------------------------------------------------------------------
#  Program:  syslog-ng.spec
#
#  Purpose:  This is the data file user to genertae RPM files so that we can
#            distrbute 'canned' versions of what we have done more easily.
#--------------------------------------------------------------------------------
#  08-Dec-02 - REP - Initial version

#--------------------------------------------------------------------------------
#  Some basic definitions for use to use later in the file.  We really only want
#  to define things once, and have to change things in only one place.
#--------------------------------------------------------------------------------
%define name       syslog-ng
%define version    1.6.12
%define release    1
%define libver     0.3.9
%define mandir     /usr/share/man
%define sbindir    /sbin

#--------------------------------------------------------------------------------
#  Basic package information
#--------------------------------------------------------------------------------
Summary:           Syslog replacement daemon
Name:              %{name}
Version:           %{version}
Release:           %{release}
Group:             System/Kernel and hardware
License:           GPL
Url:               http://www.balabit.com/products/syslog_ng/
Source0:           %{name}-%{version}.tar.gz
Source1:           syslog-ng.conf
Source2:           syslog-ng.init
Source3:           mysql-syslog.pipe
Source4:           mysql-syslog.buffer
Source5:           syslog-ng.sysconfig
Requires:          libol >= %{libver}
Buildroot:         %{_tmppath}/%{name}-buildroot

#--------------------------------------------------------------------------------
#  Description of the package
#--------------------------------------------------------------------------------
%description
syslog-ng, as the name shows, is a syslogd replacement, but with new 
functionality for the new generation. The original syslogd allows 
messages only to be sorted based on priority/facility pairs; syslog-ng 
adds the possibility to filter based on message contents using regular 
expressions. The new configuration scheme is intuitive and powerful. 
Forwarding logs over TCP and remembering all forwarding hops makes it 
ideal for firewalled environments.

#--------------------------------------------------------------------------------
#  What things to do in preperation of making the package
#--------------------------------------------------------------------------------
%prep
%setup

#--------------------------------------------------------------------------------
#  The build process for the package
#--------------------------------------------------------------------------------
%build

#--------------------------------------------------------------------------------
#  Configuration process for the package
#--------------------------------------------------------------------------------
%configure --prefix=%{_prefix} \
           --sbindir=%{sbindir} \
	   --sysconfdir=%{_sysconfdir}  \
	   --mandir=%{mandir} \
	   --enable-tcp-wrapper
make

#--------------------------------------------------------------------------------
#  The install process for the package
#--------------------------------------------------------------------------------
%install
[ -n "%{buildroot}" -a "%{buildroot}" != / ] && rm -rf %{buildroot}
mkdir -p %{buildroot}%{_initrddir}
mkdir -p %{buildroot}%{_sysconfdir}/%{name}
mkdir -p %{buildroot}/etc/sysconfig

make DESTDIR=%{buildroot} mandir=%{_mandir} install-strip

install -m644 %{SOURCE1} %{buildroot}%{_sysconfdir}/%{name}/syslog-ng.conf
install -m755 %{SOURCE2} %{buildroot}%{_initrddir}/syslog-ng
install -m700 %{SOURCE3} %{buildroot}%{_sysconfdir}/%{name}/mysql-syslog.pipe
install -m700 %{SOURCE4} %{buildroot}%{_sysconfdir}/%{name}/mysql-syslog.buffer
install -m700 %{SOURCE5} %{buildroot}/etc/sysconfig/syslog-ng

#--------------------------------------------------------------------------------
#  Things to run after it has been installed.  We will be adding syslog-ng to the
#  startup, and shutting down the old syslog
#--------------------------------------------------------------------------------
%post
/sbin/chkconfig --add syslog-ng
/sbin/chkconfig --del syslog
sed -e "s/syslogd/syslog-ng/g" /etc/logrotate.d/syslog > /tmp/syslog-ng.tmp.$$
cp -f /tmp/syslog-ng.tmp.$$ /etc/logrotate.d/syslog
rm -f /tmp/syslog-ng.tmp.$$

#--------------------------------------------------------------------------------
#  Well, if we uninstall, then we need to put some syslog in place to run for
#  this system.
#--------------------------------------------------------------------------------
%preun
/sbin/chkconfig --del syslog-ng
/sbin/chkconfig --add syslog
sed -e "s/syslog-ng/syslogd/g" /etc/logrotate.d/syslog > /tmp/syslog-ng.tmp.$$
cp -f /tmp/syslog-ng.tmp.$$ /etc/logrotate.d/syslog
rm -f /tmp/syslog-ng.tmp.$$

#--------------------------------------------------------------------------------
#  What files and permissions are included in the package
#--------------------------------------------------------------------------------
%files
%defattr(-,root,root)
%doc AUTHORS COPYING README ChangeLog INSTALL NEWS PORTS
%doc doc/sgml/syslog-ng.txt
%doc doc/*.demo doc/*.sample doc/*.solaris
%config(noreplace) %{_sysconfdir}/%{name}/syslog-ng.conf
%config(noreplace) %{_sysconfdir}/%{name}/mysql-syslog.pipe
%config(noreplace) %{_sysconfdir}/%{name}/mysql-syslog.buffer
%config(noreplace) %{_initrddir}/syslog-ng
%config(noreplace) /etc/sysconfig/syslog-ng
%{sbindir}/syslog-ng
%{_mandir}/man5/syslog-ng.conf.5*
%{_mandir}/man8/syslog-ng.8*

#--------------------------------------------------------------------------------
#  What final cleanup should occur after the package construction has been
#  completed
#--------------------------------------------------------------------------------
%clean
[ -n "%{buildroot}" -a "%{buildroot}" != / ] && rm -rf %{buildroot}
rm -r $RPM_BUILD_DIR/%{name}-%{version}

#--------------------------------------------------------------------------------
#  Changelog
#--------------------------------------------------------------------------------
%changelog
* Thu Feb 27 2003 Richard E. Perlotto II <richard@perlotto.com> 1.6.0rc1-1
  - Updated for new version

* Mon Feb 17 2003 Richard E. Perlotto II <richard@perlotto.com> 1.5.26-1
  - Updated for new version

* Sun Dec 20 2002 Richard E. Perlotto II <richard@perlotto.com> 1.5.24-1
  - Updated for new version

* Sun Dec 13 2002 Richard E. Perlotto II <richard@perlotto.com> 1.5.23-2
  - Corrected the mass of errors that occured with rpmlint
  - Continue to clean up for the helpful hints on how to write to a database

* Sun Dec 08 2002 Richard E. Perlotto II <richard@perlotto.com> 1.5.23-1
  - Updated file with notes and PGP signatures
