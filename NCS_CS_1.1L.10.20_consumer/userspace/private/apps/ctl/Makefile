# Makefile for Data_center and TR69 software
#
# This Makefile may be run "standalone" to create native software
# for a Linux PC.
# For cross-compiling, some pre-definitions are needed,
# see "buildmips" script as an example.
# (You can still do "make clean" even if cross-compiling).

# Pseudo-targets:
#      clean -- remove all(?) created files
#      firsttime -- do clean, then initialize all subdirectories
#      update (default if .firsttime exists) -- limited rebuild
#                      suitable for typical source file changes
# With no arguments, does update (and firsttime if .firsttime does not exist)
#
# Environmental variables used:
# INSTALL_ROOT=...  where to put output files
#       If missing, uses install subdirectory (useful for development
#       on a linux PC).
#
# Output files:
#      .firsttime -- writtten after "firsttime" make
#              If missing, "update" will do "firsttime"
#      $(INSTALL_ROOT) -- important run-time files copied here,
#              and also some files for cross-module compilation.
#              (By default this is "install").

INSTALL_ROOT = $(DIR_ROMFS)
INSTALL_ROOT	?=	$(shell pwd)/install

CFLAGS += $(ACTION_TEC_DFLAGS) $(CMS_COMPILE_FLAGS)
export CFLAGS
export TARGET_DIR

ROMFSINST = install -m 755


# for control layer customer flag
# Default
CTL_TARGET:=wecb

ifeq ($(ACT_PROFILE),963268GW)
CTL_TARGET:=wecb
endif

ifeq ($(ACT_PROFILE),963138GW)
CTL_TARGET:=wecbarm
endif

all: mk_lib mk_app

mk_lib:
#	make -C ../boa ctl_config
	if [ ! -f .firsttime ] ; then echo MUST DO FIRSTTIME BUILD FIRST; $(MAKE) firsttime; fi
	@echo DOING UPDATE
	@echo UPDATING tags file
	make -C files $(CTL_TARGET)_mk_lib
	mkdir -p $(INSTALL_DIR)/lib
	mkdir -p $(INSTALL_DIR)/bin
ifeq ($(AEI_DBUS),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libdbus.so	     $(INSTALL_DIR)/lib/libdbus.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libdbusrecv_msg.so	$(INSTALL_DIR)/lib/libdbusrecv_msg.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libdbussend_msg.so	$(INSTALL_DIR)/lib/libdbussend_msg.so
endif
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libtr69c_dl.so   $(INSTALL_DIR)/lib/libtr69c_dl.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libtslutil_dl.so $(INSTALL_DIR)/lib/libtslutil_dl.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libxml2.so.2     $(INSTALL_DIR)/lib/libxml2.so.2
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/librt-0.9.32.so   $(INSTALL_DIR)/lib/librt-0.9.32.so
	ln -sf /lib/librt-0.9.32.so $(INSTALL_DIR)/lib/librt.so.0	
	ln -sf /lib/libxml2.so.2       $(INSTALL_DIR)/lib/libxml2.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libdal.so        $(INSTALL_DIR)/lib/libdal.so
	# $(ROMFSINST) files/exlib/libcrypto.so.0.9.8                  $(INSTALL_DIR)/lib/libcrypto.so.0.9.8
	# $(ROMFSINST) files/exlib/libssl.so.0.9.8                     $(INSTALL_DIR)/lib/libssl.so.0.9.8
	# ln -sf /lib/libssl.so.0.9.8    $(INSTALL_DIR)/lib/libssl.so
	# ln -sf /lib/libcrypto.so.0.9.8 $(INSTALL_DIR)/lib/libcrypto.so

ifdef CONFIG_MOCA
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libmoca.so  $(INSTALL_DIR)/lib/
endif
ifdef CONFIG_WIFI
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libdal_wifi.so  $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libwifi.so $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libsock.so $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libcommon.so $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libcustom.so $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libnl_user.so $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libwifiwps.so $(INSTALL_DIR)/lib/
ifdef CONFIG_WIFI_DRIVER_QUANTENNA
	ln -sf libqcsapi_client.so.1 $(INSTALL_DIR)/lib/libqcsapi_client.so
	ln -sf libqcsapi_client.so.1.0.1 $(INSTALL_DIR)/lib/libqcsapi_client.so.1
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libqcsapi_client.so.1.0.1 $(INSTALL_DIR)/lib/
	$(ROMFSINST)  files/target/$(CTL_TARGET)/bin/qcsapi_sockrpc $(INSTALL_DIR)/bin/
	ln -sf qcsapi_sockrpc $(INSTALL_DIR)/bin/call_qcsapi
endif
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libtr69_wifi.so $(INSTALL_DIR)/lib/
ifdef CONFIG_WIFI_TR098
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libtr098_wifi.so $(INSTALL_DIR)/lib/
endif
ifdef CONFIG_WIFI_TR181
	$(ROMFSINST)  files/target/$(CTL_TARGET)/lib/libtr181_wifi.so $(INSTALL_DIR)/lib/
endif
endif
ifeq ($(AEI_TR69),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libgsoap2.so   $(INSTALL_DIR)/lib/libgsoap2.so
	$(ROMFSINST) files/target/$(CTL_TARGET)/lib/libdnssrv.so   $(INSTALL_DIR)/lib/libdnssrv.so
endif

mk_app:
	make -C files $(CTL_TARGET)_mk_app
	@echo "INSTALL_DIR: $(INSTALL_DIR)"
	mkdir -p $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/sbin
	mkdir -p $(INSTALL_DIR)/etc
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/data_center	     $(INSTALL_DIR)/sbin/data_center
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/ssd	             $(INSTALL_DIR)/sbin/ssd
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/zip	             $(INSTALL_DIR)/bin/zip 
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/cli	             $(INSTALL_DIR)/sbin/cli
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/cfecmd              $(INSTALL_DIR)/sbin/cfecmd 
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/system_monitor	 $(INSTALL_DIR)/sbin/system_monitor 
ifeq ($(AEI_ZERO_CONF),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/zero_conf        $(INSTALL_DIR)/sbin/
	ln -sf /sbin/zero_conf         $(INSTALL_DIR)/sbin/zero_conf_telus
	ln -sf /sbin/zero_conf         $(INSTALL_DIR)/sbin/zero_conf_vz
#	$(STRIP) $(INSTALL_DIR)/bin/zero_conf
endif
ifeq ($(AEI_TR69),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/tr69	         $(INSTALL_DIR)/sbin/tr69
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/msg	             $(INSTALL_DIR)/sbin/msg
endif
ifeq ($(AEI_UPGRADE),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/wanupdate        $(INSTALL_DIR)/sbin/wanupdate
endif
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/tr69_cfg 	     $(INSTALL_DIR)/sbin/tr69_cfg 
	$(ROMFSINST) files/target/$(CTL_TARGET)/bin/mynetwork 	     $(INSTALL_DIR)/sbin/mynetwork

	$(ROMFSINST) files/target/$(CTL_TARGET)/etc/protype.xml      $(INSTALL_DIR)/etc/protype.xml
	$(ROMFSINST) files/target/$(CTL_TARGET)/etc/cfg.xml          $(INSTALL_DIR)/etc/cfg.xml
	$(ROMFSINST) files/target/$(CTL_TARGET)/etc/instoid.lst      $(INSTALL_DIR)/etc/instoid.lst
ifeq ($(AEI_IGMP_SNOOPING),1)
	$(ROMFSINST) files/target/$(CTL_TARGET)/etc/mcpd.conf        $(INSTALL_DIR)/etc/mcpd.conf
endif
ifdef CONFIG_WIFI
	$(ROMFSINST)  files/target/$(CTL_TARGET)/bin/wifid $(INSTALL_DIR)/bin/
endif

romfs:
	mkdir -p $(DIR_ROMFS)/sbin
	mkdir -p $(DIR_ROMFS)/etc
	mkdir -p $(DIR_ROMFS)/lib
ifeq ($(AEI_CRISTAL_ACCESS),1)
	mkdir -p $(DIR_ROMFS)/nagios
endif
	mkdir -p $(DIR_ROMFS)/var/rt_conf
	mkdir -p $(DIR_ROMFS)/mnt/rt_conf
	$(ROMFSINST) files/target/wecb/bin/data_center	/sbin/data_center
	$(ROMFSINST) files/target/wecb/bin/ssd	/sbin/ssd
	$(ROMFSINST) files/target/wecb/bin/zip	/bin/zip 
	$(ROMFSINST) files/target/wecb/bin/cli	/sbin/cli 
	$(ROMFSINST) files/target/wecb/bin/system_monitor	/sbin/system_monitor 
ifeq ($(AEI_ZERO_CONF),1)
	$(ROMFSINST) files/target/wecb/bin/zero_conf    /sbin/zero_conf
	ln -sf /sbin/zero_conf $(DIR_ROMFS)/sbin/zero_conf_telus
	ln -sf /sbin/zero_conf $(DIR_ROMFS)/sbin/zero_conf_vz
endif
ifeq ($(AEI_TR69),1)
	$(ROMFSINST) files/target/wecb/bin/tr69	/sbin/tr69
	$(ROMFSINST) files/target/wecb/bin/msg	/sbin/msg
endif
	$(ROMFSINST) files/target/wecb/bin/tr69_cfg	/sbin/tr69_cfg 
	$(ROMFSINST) files/target/wecb/bin/mynetwork	/sbin/mynetwork
	$(ROMFSINST) files/target/wecb/bin/setbasemac	/sbin/setbasemac
ifeq ($(AEI_DBUS),1)
	$(ROMFSINST) files/target/wecb/lib/libdbus.so	/lib/libdbus.so
	$(ROMFSINST) files/target/wecb/lib/libdbusrecv_msg.so	/lib/libdbusrecv_msg.so
	$(ROMFSINST) files/target/wecb/lib/libdbussend_msg.so	/lib/libdbussend_msg.so
endif
	$(ROMFSINST) files/target/wecb/lib/libtr69c_dl.so	/lib/libtr69c_dl.so
	$(ROMFSINST) files/target/wecb/lib/libtslutil_dl.so	/lib/libtslutil_dl.so
	$(ROMFSINST) files/target/wecb/lib/libxml2.so.2	  /lib/libxml2.so.2	
	$(ROMFSINST) -s libxml2.so.2  /lib/libxml2.so
	$(ROMFSINST) files/target/wecb/lib/libdal.so	/lib/libdal.so
	# $(ROMFSINST) files/exlib/libcrypto.so.0.9.8	/lib/libcrypto.so.0.9.8
	#$(ROMFSINST) files/exlib/libssl.so.0.9.8	/lib/libssl.so.0.9.8
	#$(ROMFSINST) -s libssl.so.0.9.8	/lib/libssl.so
	#$(ROMFSINST) -s libcrypto.so.0.9.8	/lib/libcrypto.so
	$(ROMFSINST) files/target/wecb/etc/protype.xml	/etc/protype.xml
	$(ROMFSINST) files/target/wecb/etc/cfg.xml	/etc/cfg.xml
	$(ROMFSINST) files/target/wecb/etc/instoid.lst	/etc/instoid.lst
ifeq ($(AEI_CRISTAL_ACCESS),1)
	$(ROMFSINST) files/target/wecb/bin/factoryctl  /sbin/factoryctl
endif
firsttime:  clean
	echo Did first time inits >.firsttime
	@echo DONE with firsttime

clean:
	@echo DOING CLEAN
	-rm -rf install tags .firsttime
	-find . -name 'junk*' -exec rm -rf '{}' ';'
	-find . -name '.deps' -exec rm -rf '{}' ';'
	-find . -name '.dirstamp' -exec rm -rf '{}' ';'
	-find . -name '.obj' -exec rm -rf '{}' ';'
	-find . -name '.moc' -exec rm -rf '{}' ';'
	-find . -name '.ui' -exec rm -rf '{}' ';'
	-find . -name '.rnd' -exec rm -rf '{}' ';'
	-find . -name '.*.swp' -exec rm -rf '{}' ';'
	-find . -name '*.log' -exec rm -rf '{}' ';'
	-(cd files && $(MAKE) clean PLAT=wecb)
	@echo All cleaned
