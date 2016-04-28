# =============================================================================
# makelib.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
#=============================================================================
# GENERIC MAKEFILE TO GENERATE A LIBRARY
#=============================================================================
# it requires:
# MODULE_ID    which reflects the library to build
# MODULES_USED which reflects the dependencies and so the includes path
# INCLUDES     which relects  some specific include needs
#
# general vaiables that reflects the files tree defined in toolchains.mk:
# MODULE_PATH_xxxx (that reflects the location of the modules)
# OBJ_DIR
# LIB_DIR
# MAKEFILE_ROOT where the general files to be include in makefile are
#
# general variables that reflect the toolset command:
# INCLUDE_PREFIX
# MKDIR
# OPTIONAL Flags used for compilation
# CFLAGS which is a compilation flag
# GENERAL_CFLAGS optionaly, it is added to CFLAGS
# OPTIONS used the same way as CFLAGS but should be used for conditionnal compilation flags

#=============================================================================
include $(MAKEFILE_ROOT)/defrules.mk

# Output library file name
LIBS      := $(LIB_FILE_PREFIX)$(MODULE_ID)$(LIB_FILE_SUFFIX)

.PHONY: clean check help config

all: check $(LIB_DIR)/$(LIBS)

check: check_module_dep

#=============================================================================
# Clean all the local object
clean:
	@$(ECHO) $(PROMPT_SYMBLO_L1) Cleaning in $(LIB_DIR) ...
	$(AT)$(RM) -r $(LIB_DIR)/*
	@$(ECHO) $(PROMPT_SYMBLO_L1) Cleaning in $(OBJ_DIR) ...
	$(AT)$(RM) -r $(OBJ_DIR)/*

#=============================================================================
# Configure this module build parameter
config:
	@$(ECHO) $(CENTER_SCREEN) "+++++++++++++++++++++++++++++++++++++++++++++++" ;
	@$(ECHO) $(CENTER_SCREEN) "++  $(MODULE_ID) Configure Modules Menu  +++" ;
	@$(ECHO) $(CENTER_SCREEN) "+++++++++++++++++++++++++++++++++++++++++++++++" ;
	$(AT)config_script="$(MODULE_PATH_$(MODULE_ID))/configure"; \
         if [ -f "$$config_script" ]; then\
              . $$config_script;\
         else \
              $(ECHO) $$config_script NOT FOUND;\
         fi;\

help: mklib_help common_help

mklib_help:
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) Make Lib Targets:
	@$(ECHO)
	@$(ECHO)   "- make            : create the librarie $(LIBS)"
	@$(ECHO)   "- make config     : to configure this module"
	@$(ECHO)   "- make clean      : clean all files and dirs in $(LIB_DIR)/ and $(OBJ_DIR)/ of $(MODULE_ID)"
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO)

# include other functional makefiles
# include $(MAKEFILE_ROOT)/xxx.mk

include $(MAKEFILE_ROOT)/makecommon.mk
