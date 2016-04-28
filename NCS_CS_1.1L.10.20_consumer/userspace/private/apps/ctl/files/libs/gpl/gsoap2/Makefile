include $(CTL_DIR)/global.def

CFLAGS += -D_DEBUG -DSUPPORT_WWW_AUTHENTICATION  -DSUPPORT_COOKIE -DWITH_COOKIES -DSUPPORT_CONNECTION_AUTH -DSUPPORT_SSL -DWITH_OPENSSL -DTR69_DEBUG_CWMP
IFLAGS += -I$(PROFILE_DIR)/fs.build/include -I$(PROFILE_DIR)/fs.build/public/include

LFLAGS += -L$(PROFILE_DIR)/fs.build/lib
LFLAGS += -L$(PROFILE_DIR)/fs.build/public/lib

LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/
LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/public
LFLAGS += -L$(PROFILE_DIR)/fs.install/lib/private

LDFLAGS += -shared -fPIC
gsoap_lib = $(INSTALL)/lib/libgsoap2.so
GSOAP_OBJS = soapClient2.o soapC.o stdsoap2.o soapServer2.o plugin/httpda.o plugin/md5evp.o plugin/threads.o

all: $(gsoap_lib) 

$(gsoap_lib): $(GSOAP_OBJS)
	$(CC) $(IFLAGS) $(LDFLAGS) -o $@ $?

%.o:%.c
	$(CC) -c $(CFLAGS) $(IFLAGS)  $< -o $@

plugin/*.o: plugin/*.c
	$(CC) -c $(CFLAGS) $(IFLAGS)  $< -o $@

clean:
	rm -rf  $(GSOAP_OBJS)
	rm -rf $(gsoap_lib)

