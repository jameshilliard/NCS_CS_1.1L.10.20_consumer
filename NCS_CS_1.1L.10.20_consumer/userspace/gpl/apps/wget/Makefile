#
# Copyright (C) 2007-2014 Actiontec.com
#
#
EXE = wget

BUILD_VARIANT ?= ssl

CONFIGURE_ARGS+= \
    --host=mips-unknown-linux-uclibc \
    --disable-rpath \
    --disable-iri

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
	install -m 755 wget-1.15/src/$(EXE) $(INSTALL_DIR)/sbin/

$(EXE):
	@echo "need to do configure WGET first"
	cd wget-1.15; \
	./configure $(CONFIGURE_ARGS); \
	make

clean:
	cd wget-1.15; \
	make clean
	
