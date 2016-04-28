#CC = mips-linux-gcc
#AR = mips-linux-ar
#RANLIB = mips-linux-ranlib

CFLAGS += -I. -I../ 
OBJECTS =  unix_sock.o

lib_LTLIBRARIES = libsockmsg.a
bin_hnapmsg = hnap_msg

all: $(lib_LTLIBRARIES) $(bin_hnapmsg)

%.o : %.c
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

$(lib_LTLIBRARIES): $(OBJECTS)
	$(AR) cru $@ $(OBJECTS)
	$(RANLIB) $@

$(bin_hnapmsg): hnap_msg.o
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) hnap_msg.o

	
clean:
	rm -f $(OBJECTS) hnap_msg.o 
	rm -f $(lib_LTLIBRARIES)
	rm -f $(bin_hnapmsg)

