# =============================================================================
# lib_module.sample.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
PROJECT_ROOT ?= ../
AEI_MKSYS_ROOT ?= $(PROJECT_ROOT)/aei_makesys
MAKEFILE_ROOT = $(AEI_MKSYS_ROOT)
#========= Include project configuration
include $(MAKEFILE_ROOT)/project_config.mk

MODULE_ID:=module1
MODULE_VERSION:=draft

#Just specify direct depended modules in $(MAKEFILE_ROOT)/projectname.prj is ok

# SRC_DIR specify a relative directory to this makefile,
# all the source files specified in SOURCES starts from this directory.
# Note: to avoid multi-adjacent slash in source code file location, do not add the last slash, makefile system will add it;
#
# We found that duplicate slash ("//") in source code file location will raise issue when use "sh4gdb -w" to debug program.
#
SRC_DIR = src
SOURCES = test.c \
          or/any/other/dir1/under/src/test.cpp

#========= Include of common lib makefile
include $(MAKEFILE_ROOT)/makelib.mk

#Extend some default targets
help: $(MODULE_ID)_help

$(MODULE_ID)_help:
	@echo "This is $(MODULE_ID) help"
