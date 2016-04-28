#
# Copyright (C) 2007-2014 Actiontec.com
#
#
EXE = curl

BUILD_VARIANT ?= ssl

CONFIGURE_ARGS+= \
    --host=mips-unknown-linux-uclibc 

AEI_CFLAGS += -O2 -Wall

ifeq ($(BUILD_VARIANT),ssl)
    CONFIGURE_ARGS+= \
	CFLAGS="$(AEI_CFLAGS)" \
        --with-ssl=openssl \
        --with-libssl-prefix="$(PROFILE_DIR)/fs.build"
endif

ifeq ($(BUILD_VARIANT),nossl)
    CONFIGURE_ARGS+= \
        --without-ssl
endif

install: $(EXE)
	install -m 755 curl-7.29.0/src/$(EXE) $(INSTALL_DIR)/sbin/

$(EXE):
	@echo "need to do configure curl first"
	cd curl-7.29.0; \
	./configure $(CONFIGURE_ARGS); \
	make

clean:
	cd curl-7.29.0; \
	make clean
	
