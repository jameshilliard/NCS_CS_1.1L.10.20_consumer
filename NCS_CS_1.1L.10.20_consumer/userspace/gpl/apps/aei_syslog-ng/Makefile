all: build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

build:
	if [ ! -e .firsttime ] ; then echo MUST DO FIRSTTIME BUILD FIRST; $(MAKE) firsttime; fi
	@echo DOING UPDATE
	@echo UPDATING tags file
	make -C src
	mkdir -p $(INSTALL_DIR)/sbin
	install -m 755 ./src/src/syslog-ng $(INSTALL_DIR)/sbin/syslog-ng
	mkdir -p $(INSTALL_DIR)/etc/syslog-ng
	install ./src/files/syslog-ng.conf $(INSTALL_DIR)/etc/syslog-ng/
	mkdir -p $(INSTALL_DIR)/etc/init.d
	install ./src/files/syslog-ng.init $(INSTALL_DIR)/etc/init.d/syslog-ng
	

firsttime:  clean
	echo Did first time inits >.firsttime
	@echo DONE with firsttime
	cd src &&./configure --host=$(TOOLCHAIN_PREFIX) --target=$(TOOLCHAIN_PREFIX) --disable-dependency-tracking --disable-full-static --enable-tcp-wrapper --with-libol=libol-0.3.18/src/.libs 

clean:
	@echo DOING CLEAN
	-rm -rf install tags .firsttime
	-find . -name '.deps' -exec rm -rf '{}' ';'
	-find . -name '.obj' -exec rm -rf '{}' ';'
	-find . -name '*.log' -exec rm -rf '{}' ';'
	-(cd src && $(MAKE) clean)
	@echo All cleaned


