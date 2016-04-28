# udhcp makefile

#CROSS_COMPILE=mips-linux-
LDFLAGS = -Wl,--allow-shlib-undefined

prefix=/usr
SBINDIR=/sbin
USRSBINDIR=${prefix}/sbin
USRBINDIR=${prefix}/bin
USRSHAREDIR=${prefix}/share

# Uncomment this to get a shared binary. Call as udhcpd for the server,
# and udhcpc for the client
COMBINED_BINARY=1

# Uncomment this for extra output and to compile with debugging symbols
#DEBUG=1

# Uncomment this to output messages to syslog, otherwise, messages go to stdout
#CFLAGS += -DSYSLOG

#CROSS_COMPILE=arm-uclibc-
#CC = $(CROSS_COMPILE)gcc
#LD = $(CROSS_COMPILE)gcc

INSTALL = install

VER := 0.9.8


OBJS_SHARED = options.o socket.o packet.o pidfile.o
DHCPD_OBJS = dhcpd.o arpping.o files.o leases.o serverpacket.o
DHCPC_OBJS = dhcpc.o clientpacket.o script.o

ifdef COMBINED_BINARY
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(DHCPC_OBJS) $(OBJS_SHARED) frontend.o
CFLAGS += -DCOMBINED_BINARY
else
EXEC1 = udhcpd
OBJS1 = $(DHCPD_OBJS) $(OBJS_SHARED)

EXEC2 = udhcpc
OBJS2 = $(DHCPC_OBJS) $(OBJS_SHARED)
endif

EXEC3 = dumpleases
OBJS3 = dumpleases.o

BOOT_PROGRAMS = udhcpc
DAEMONS = udhcpd
COMMANDS = dumpleases

ifdef SYSLOG
CFLAGS += -DSYSLOG
endif

CFLAGS += -W -Wall -Wstrict-prototypes -DVERSION='"$(VER)"'

CFLAGS += -I$(BUILD_DIR)/userspace/public/include -I$(BUILD_DIR)/userspace/public/include/linux

CFLAGS += $(ACTION_TEC_DFLAGS) $(CMS_COMPILE_FLAGS)

ifeq ($(AEI_WECB),1)
CFLAGS += -I../../../private/apps/ctl/files/include
LIBS += -L../../../private/apps/ctl/files/target/wecb/lib -ltr69c
endif

ifdef AEI_ZERO_CONF
CFLAGS += -I../../../private/apps/ctl/files/apps/private/zero_conf/
endif

ifdef ACT_SW_VER
CFLAGS += -DACT_SW_VER='"$(ACT_SW_VER)"'
endif

ifdef DEBUG
CFLAGS += -g -DDEBUG
STRIP=true
else
CFLAGS += -Os -fomit-frame-pointer
STRIP=$(CROSS_COMPILE)strip
endif

all: $(EXEC1) $(EXEC2) $(EXEC3)
#	$(STRIP) --remove-section=.note --remove-section=.comment $(EXEC1) $(EXEC2) $(EXEC3)

$(OBJS1) $(OBJS2) $(OBJS3): *.h Makefile
$(EXEC1) $(EXEC2) $(EXEC3): Makefile

.c.o:
	$(CC) -c $(CFLAGS) $<
	
$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) $(OBJS1) -o $(EXEC1) $(LIBS)

$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) $(OBJS2) -o $(EXEC2)

$(EXEC3): $(OBJS3)
	$(CC) $(LDFLAGS) $(OBJS3) -o $(EXEC3)


install: udhcpd
	mkdir -p $(INSTALL_DIR)/bin
	install -m 755 udhcpd $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/udhcpd
	ln -sf udhcpd $(INSTALL_DIR)/bin/udhcpc
	mkdir -p $(INSTALL_DIR)/etc/udhcpc/
	install ./mips-scripts/* $(INSTALL_DIR)/etc/udhcpc
dynamic: all install


#	$(INSTALL) $(STRIP) $(DAEMONS) $(USRSBINDIR)
#	$(INSTALL) $(STRIP) $(COMMANDS) $(USRBINDIR)
#ifdef COMBINED_BINARY
#	ln -sf $(USRSBINDIR)/$(DAEMONS) $(SBINDIR)/$(BOOT_PROGRAMS)
#else
#	$(INSTALL) $(STRIP) $(BOOT_PROGRAMS) $(SBINDIR)
#endif
#	mkdir -p $(USRSHAREDIR)/udhcpc
#	for name in bound deconfig renew script ; do \
#		$(INSTALL) samples/sample.$$name \
#			$(USRSHAREDIR)/udhcpc/default.$$name ; \
#	done
#	mkdir -p $(USRSHAREDIR)/man/man1
#	$(INSTALL) dumpleases.1 $(USRSHAREDIR)/man/man1
#	mkdir -p $(USRSHAREDIR)/man/man5
#	$(INSTALL) udhcpd.conf.5 $(USRSHAREDIR)/man/man5
#	mkdir -p $(USRSHAREDIR)/man/man8
#	$(INSTALL) udhcpc.8 udhcpd.8 $(USRSHAREDIR)/man/man8

clean:
	-rm -f udhcpd udhcpc dumpleases *.o core

