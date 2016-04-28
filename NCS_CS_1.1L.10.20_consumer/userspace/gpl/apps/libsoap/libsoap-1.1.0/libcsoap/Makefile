#CC = mips-linux-gcc
#AR = mips-linux-ar
#RANLIB = mips-linux-ranlib
DEFS += -DHAVE_CONFIG_H #-DAEI_CISCO_HNAP_INDENT
opensslpath =../../../../../public/libs/openssl-1.0.1e/openssl-1.0.1e/include 


CFLAGS += -I. -I../  -I$(PROFILE_DIR)/fs.build/include -I$(PROFILE_DIR)/fs.build/public/include -I$(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/include/libxml2 -I$(opensslpath) 

LDFLAGS += -L$(PROFILE_DIR)/fs.build/lib
LDFLAGS += -L$(PROFILE_DIR)/fs.build/public/lib
 
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/public
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/privateivate

LDFLAGS += -L$(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/lib  \
	-lxml2 -lm -pthread -lssl -lcrypto

OBJECTS = soap-xml.o \
	soap-fault.o soap-env.o \
	soap-service.o soap-router.o \
	soap-client.o soap-server.o \
	soap-ctx.o soap-admin.o

lib_LTLIBRARIES = libcsoap.a

all: $(lib_LTLIBRARIES) 

%.o : %.c
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

$(lib_LTLIBRARIES): $(OBJECTS)
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

clean:
	rm -f $(OBJECTS)
	rm -f $(lib_LTLIBRARIES)

