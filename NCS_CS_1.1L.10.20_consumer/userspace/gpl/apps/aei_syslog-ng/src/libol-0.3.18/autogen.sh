#!/bin/sh
#
# $Id: autogen.sh,v 1.5 2004/08/05 10:34:48 bazsi Exp $
#
# This script is needed to setup build environment from checked out
# source tree. 
#

libtoolize -f
aclocal
automake --foreign --add-missing
autoheader
autoconf

./configure $@
