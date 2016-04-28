include $(CTL_DIR)/global.def

LDFLAGS += -shared -fPIC
dns_lib = $(INSTALL)/lib/libdnssrv.so
DNS_OBJS = dns.o

all: $(dns_lib) 

$(dns_lib): $(DNS_OBJS)
	$(CC) $(IFLAGS) $(LDFLAGS) -o $@ $(DNS_OBJS)
	
clean:
	rm -rf $(DNS_OBJS) 
	rm -rf $(dns_lib)
