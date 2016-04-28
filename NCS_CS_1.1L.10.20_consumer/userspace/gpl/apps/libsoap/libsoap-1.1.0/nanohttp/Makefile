#CC = mips-linux-gcc
#AR = mips-linux-ar
#RANLIB = mips-linux-ranlib
DEFS += -DHAVE_CONFIG_H  #-DHNAP_MEM_CHECK
opensslpath = ../../../../../public/libs/openssl-1.0.1e/openssl-1.0.1e/include

CFLAGS += -I. -I../ -I$(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/include/libxml2 -I$(BUILD_DIR)/userspace/private/apps/ctl/files/include  -I$(opensslpath)

OBJECTS =  nanohttp-common.o nanohttp-socket.o nanohttp-client.o     \
                           nanohttp-server.o nanohttp-stream.o nanohttp-mime.o   \
                           nanohttp-request.o nanohttp-response.o                \
                           nanohttp-base64.o nanohttp-ssl.o nanohttp-logging.o

lib_LTLIBRARIES = libnanohttp.a

all: $(lib_LTLIBRARIES) 

%.o : %.c
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

$(lib_LTLIBRARIES): $(OBJECTS)
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

clean:
	rm -f $(OBJECTS)
	rm -f $(lib_LTLIBRARIES)

