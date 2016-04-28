# =============================================================================
# toolchains.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
# =============================================================================
# toolchains.mk
# Copyright (C) 2006 xxx company name
#
# This file sets various variables based on the toolchains used to build the system.


export TOOL_ROOTDIR=

CPU_CORE=
# -----------------------------------------
#The C Compiler
CC=$(TOOL_ROOTDIR)$(CPU_CORE)gcc

#	The Ass Compiler
AS=$(TOOL_ROOTDIR)$(CPU_CORE)gcc

#The archiver/librarian tool
AR=$(TOOL_ROOTDIR)$(CPU_CORE)ar -rc

#The linker
LINK=$(TOOL_ROOTDIR)$(CPU_CORE)gcc

#The launcher
RUN=$(TOOL_ROOTDIR)

# -----------------------------------------
#	The debugger
DBG         = $(TOOL_ROOTDIR)$(CPU_CORE)gdb -w

# -----------------------------------------
#	The stripper will remove debugging information and non global symbol
STRIP         = $(TOOL_ROOTDIR)$(CPU_CORE)strip -d -x

# -----------------------------------------
#	The image generator perl script
MKIMAGE     =

# -----------------------------------------
#	Prefix to compiler for include paths (-I or -J).
INCLUDE_PREFIX = -I

# -----------------------------------------
# Prefix to compiler for library paths
LIBRARY_PREFIX = -L

# -----------------------------------------
# Prefix to compiler for library paths
INCLUDE_LIBRARY_PREFIX = -l

# -----------------------------------------
# Settings useful commands
        CAT     = cat
        CD      = cd
        CLS     = clear
        CP      = cp
        DIFF    = diff
        NULL    = /dev/null
        RM      = -rm -f
        RMDIR   = -rm -r
        MKDIR   = mkdir -p
        DOXYGEN = doxygen

# Slash Management depending on platform
UNIX_SLASH = /
DOS_SLASH = $(subst /,\,$(UNIX_SLASH))

SLASH = $(UNIX_SLASH)
GOOD_SLASH = $(UNIX_SLASH)
BAD_SLASH = $(DOS_SLASH)
