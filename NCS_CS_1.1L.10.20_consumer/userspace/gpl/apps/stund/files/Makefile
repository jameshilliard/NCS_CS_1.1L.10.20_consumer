

TARS= client.cxx  server.cxx  stun.cxx  stun.h  tlsServer.cxx  udp.cxx  udp.h \
	Makefile rfc3489.txt\
	client.sln  client.vcproj  server.sln  server.vcproj  Stun.sln \
	id.pem id_key.pem root.pem \
	nattestwarning.txt nattest wnattest.bat \
	WinStun/resource.h WinStun/stdafx.cpp WinStun/stdafx.h \
	WinStun/WinStun.cpp WinStun/WinStunDlg.cpp WinStun/WinStunDlg.h WinStun/WinStun.h \
	WinStun/WinStun.rc WinStun/WinStun.vcproj \
	WinStun/res/WinStun.ico WinStun/res/WinStun.manifest WinStun/res/WinStun.rc2 \
	WinStunSetup/WinStunSetup.vdproj

# if you chnage this version, change in stun.h too 
VERSION=0.96

#CXX = g++
#CXX = rsdk-linux-g++

#CXXFLAGS+=-O2
#LDFLAGS+=-O2 -lssl
STUNLIB=libstun.a

#
# Alternatively, for debugging.
#
CXXFLAGS+=-g -O -Wall 
#LDFLAGS+=-g -O -Wall 
# for solaris
#LDFLAGS+= -lnsl -lsocket

CURDIR ?=  $(shell pwd)

#CXXFLAGS+= -DS_DEBUG
CXXFLAGS+= -DAEI_WECB=1 
CXXFLAGS+= -DSUPPORT_CTL

# Use openssl or not
#CXXFLAGS+=-DNOSSL
CXXFLAGS += -I$(PROFILE_DIR)/fs.build/include -I$(PROFILE_DIR)/fs.build/public/include -I../../../../public/libs/openssl-1.0.1e/openssl-1.0.1e/include

LDFLAGS += -L$(PROFILE_DIR)/fs.build/lib
LDFLAGS += -L$(PROFILE_DIR)/fs.build/public/lib
 
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/public
LDFLAGS += -L$(PROFILE_DIR)/fs.install/lib/private
LDFLAGS+= -L../../../../public/libs/openssl-1.0.1e/openssl-1.0.1e -lssl -lcrypto

CLIENT_OBJ = act_stunc.o stun.o udp.o

ACTIONTEC_CTL_INCLUDE=$(BUILD_DIR)/userspace/private/apps/ctl/files/include
ACTIONTEC_CTL_LIB= $(BUILD_DIR)/userspace/private/apps/ctl/files/libs/private/framework/lib 
CXXFLAGS += -I$(ACTIONTEC_CTL_INCLUDE)
LDFLAGS+= -L$(ACTIONTEC_CTL_LIB) -ltr69c_dl

LDFLAGS+= -lpthread

CLIENT_OBJ+= act_common.o act_tr69.o  

all: stunc 
#all: server client libstun.a 

clean:
	- rm -f *.o server client libstun.a stunc

tar: $(TARS)
	cd ..; tar cvfz `date +"stund/stund_$(VERSION)_$(PROG)%b%d.tgz"` \
			 $(addprefix stund/, $(TARS))

server: server.o stun.o udp.o 
	$(CXX) $(LDFLAGS) -o $@  $^

tlsServer: tlsServer.o stun.o udp.o
	$(CXX) $(LDFLAGS) -o $@  $^

client: $(CLIENT_OBJ)  
	$(CXX)  -o $@  $^ $(LDFLAGS)

stunc: $(CLIENT_OBJ)
	$(CXX)  -o $@  $^ $(LDFLAGS)

%.o:%.cxx
	$(CXX)  -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

libstun.a: stun.o udp.o
	ar rv $@ $<

%:RCS/%
	co $@

# Dependancies
# server.o: stun.h udp.h 
# client.o: stun.h udp.h 
# stun.o: stun.h udp.h
# udp.o: stun.h udp.h 
# tlsServer.o: stun.h udp.h
# act_common.o: act_common.h
# act_tr69.o: act_tr69.h libtr69_client.h 
