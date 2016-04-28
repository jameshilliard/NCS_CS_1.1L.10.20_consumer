#
# Before changing the macro below see the README file included in
# the syslog-ng package.
#
%define    with_scheme_preprocessor 0

Summary:   Support library for syslog-ng
Name:      libol
Version:   0.3.18
Release:   1
Epoch:     0
License:   GPL
Group:     System Environment/Libraries
Url:       http://www.balabit.com/products/syslog-ng/
Source:    http://www.balabit.com/downloads/syslog-ng/libol/0.3/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
%if %{with_scheme_preprocessor}
BuildRequires: scsh
%endif

%description
Support library for syslog-ng.


%package devel
Summary:   Support library for syslog-ng: headers and libraries
Group:     Development/Libraries
Requires:  %{name} = %{epoch}:%{version}-%{release}
%if %{with_scheme_preprocessor}
Requires:  scsh
%endif

%description devel
Support library for syslog-ng: headers and libraries.


%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf ${RPM_BUILD_ROOT}
%makeinstall

%clean
rm -rf ${RPM_BUILD_ROOT}


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%doc ChangeLog COPYING
%{_libdir}/libol.so.0
%{_libdir}/libol.so.0.0.0

%files devel
%defattr(-,root,root,-)
%if %{with_scheme_preprocessor}
%{_bindir}/make_class
%else
%exclude %{_bindir}/make_class
%endif
%{_bindir}/libol-config
%{_libdir}/libol.a
%{_libdir}/libol.la
%{_libdir}/libol.so
%{_includedir}/libol


%changelog
* Thu May  6 2004 Jose Pedro Oliveira <jpo@di.uminho.pt> - 0:0.3.13
- update to version 0.3.13
- macro %{with_scheme_preprocessor}
- merged several changes from the fedora.us libol specfile

* Thu Apr 10 2003 Jose Pedro Oliveira <jpo@di.uminho.pt>
- specfile reformated 
- replaced hard coded values by RPM macros

* Sun Jul 30 2000 Hollis Blanchard <hollis@penguinppc.org>
- initial RPM
