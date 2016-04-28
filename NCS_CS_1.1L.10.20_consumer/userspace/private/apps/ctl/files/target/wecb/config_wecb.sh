#! /bin/sh

cat >global.def <<_ACEOF

CROSS=/opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/
#J_CC=/opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/mips-linux-gcc
#J_AR=/opt/toolchains/crosstools-mips-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21/usr/bin/mips-linux-ar
#############For WiFi###############
export CONFIG_WIFI=y
export CONFIG_WIFI_TR181=y
export CONFIG_WIFI_DRIVER_BROADCOM=y
export CONFIG_WIFI_DRIVER_QUANTENNA=y
####################################
#############For MoCA###############
export CONFIG_MOCA=y
export CONFIG_MOCA_DRIVER_BROADCOM=y
####################################
INSTALL=`/bin/pwd`/target/wecb
CFLAGS+= -Wall -D_LINUX -g 
IFLAGS+= -I./ -I`/bin/pwd`/include -I`/bin/pwd`/include/libxml2
CFLAGS+= -I${BUILD_DIR}/userspace/public/include/ \
        -I${BUILD_DIR}/userspace/public/include/linux/ \
        -I${BUILD_DIR}/userspace/private/include/ \
        -I${BUILD_DIR}/userspace/private/include/linux/ \
        -I${BUILD_DIR}/shared/opensource/include/bcm963xx/ \
        -I${BUILD_DIR}/bcmdrivers/opensource/include/bcm963xx/
LFLAGS+= -L`/bin/pwd`/target/wecb/lib -lpthread
TARGET_PLATFORM=WECB
CTL_DIR=`/bin/pwd`
TARGET_DIR=`/bin/pwd`/target/wecb/
_ACEOF

TARGET_DIR=`/bin/pwd`/target/wecb/
mkdir -p ${TARGET_DIR}/lib/
mkdir -p ${TARGET_DIR}/bin/
mkdir -p ${TARGET_DIR}/etc/

