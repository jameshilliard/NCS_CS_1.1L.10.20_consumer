#! /bin/sh

cat >global.def <<_ACEOF

CROSS=/opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/
#J_CC=/opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/mips-linux-gcc
#J_AR=/opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/mips-linux-ar
INSTALL=`/bin/pwd`/target/wecbarm
CFLAGS+= -Wall -D_LINUX -g 
IFLAGS+= -I./ -I`/bin/pwd`/include -I`/bin/pwd`/include/libxml2
CFLAGS+= -I${BUILD_DIR}/userspace/public/include/ \
        -I${BUILD_DIR}/userspace/public/include/linux/ \
        -I${BUILD_DIR}/userspace/private/include/ \
        -I${BUILD_DIR}/userspace/private/include/linux/ \
        -I${BUILD_DIR}/shared/opensource/include/bcm963xx/ \
        -I${BUILD_DIR}/bcmdrivers/opensource/include/bcm963xx/
LFLAGS+= -L`/bin/pwd`/target/wecbarm/lib -lpthread
TARGET_PLATFORM=WECBARM
CTL_DIR=`/bin/pwd`
TARGET_DIR=`/bin/pwd`/target/wecbarm/
_ACEOF

TARGET_DIR=`/bin/pwd`/target/wecbarm/
mkdir -p ${TARGET_DIR}/lib/
mkdir -p ${TARGET_DIR}/bin/
mkdir -p ${TARGET_DIR}/etc/

