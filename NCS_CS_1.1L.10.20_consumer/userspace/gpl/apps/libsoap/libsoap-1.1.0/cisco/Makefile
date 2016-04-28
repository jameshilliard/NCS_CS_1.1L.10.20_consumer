
cisco_hnap = cisco_hnap
httplib = ../nanohttp/libnanohttp.a
soaplib = ../libcsoap/libcsoap.a
#CC = mips-linux-gcc
xmlinc = $(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/include/libxml2
opensslpath = ../../../../../public/libs/openssl-1.0.1e/openssl-1.0.1e/include


CFLAGS = -DHAVE_CONFIG_H -g -O2 -I. -I../ -I$(PROFILE_DIR)/fs.build/include -I$(PROFILE_DIR)/fs.build/public/include  -I$(xmlinc) -I$(BUILD_DIR)/userspace/private/apps/ctl/files/include -I$(opensslpath) 

LFLAGS += -L$(PROFILE_DIR)/fs.build/lib
LFLAGS += -L$(PROFILE_DIR)/fs.build/public/lib
 
LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/
LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/public
LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/private
LDFLAGS =  -L$(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/lib  \
	-lxml2 -lm -pthread -ltr69c_dl -lssl -lcrypto

all:  $(cisco_hnap)


$(cisco_hnap): $(httplib) $(soaplib) cisco_hnap.c cisco_hnap.h
	$(CC) $(CFLAGS) -o $@ cisco_hnap.c $(soaplib) $(httplib) $(LFLAGS) $(LDFLAGS) 


clean:
	rm -f *.o
	rm -f $(cisco_hnap)
	
