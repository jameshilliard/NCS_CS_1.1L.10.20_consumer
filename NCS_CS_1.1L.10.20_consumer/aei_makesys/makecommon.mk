# =============================================================================
# makecommon.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
#=============================================================================
# GENERIC MAKEFILE contains common useful targets.
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
# SRCPATH ????
#
# general variables that reflect the toolset command:
# INCLUDE_PREFIX
# MKDIR
# OPTIONAL Flags used for compilation
# CFLAGS which is a compilation flag
# GENERAL_CFLAGS optionaly, it is added to CFLAGS
# OPTIONS used the same way as CFLAGS but should be used for conditionnal compilation flags

#=============================================================================

#ifneq ($(OBJS),)
# because some modules don't have source file list,
# and don't need dependencises
#-include $(subst .o,.d,$(OBJS))
#endif

ifneq ($(DEPS),)
# because some modules don't have source file list,
# and don't need dependencises
-include $(DEPS)
endif

check_module_dep:
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) $(PROMPT_SYMBLO_L1) Building module $(MODULE_ID)-$(MODULE_VERSION)
	@$(ECHO) "MODULES USED    : $(MODULES_USED)"
ifneq ($(LIST_PATH_NOT_DEFINED_USED_MODULES),)
	@$(ECHO) $(ERROR_INFO) Module dependency check fail.
	@$(ECHO) These modules pathes are NOT DEFINED in $(PROJECT_CONFIG_FILE) : $(ECHO_BOLD) $(LIST_PATH_NOT_DEFINED_USED_MODULES) $(ECHO_NORMAL);
	@$(ECHO) 1. Check project configuration file $(PROJECT_CONFIG_FILE) path definitation for $(LIST_PATH_NOT_DEFINED_USED_MODULES).
	@$(ECHO) 2. Contact makefile system maintainer.
endif
ifneq ($(LIST_MISSED),)
	@$(ECHO) $(WARNING_INFO) Module dependency check fail.
	@$(ECHO) These modules are NOT FOUND : $(ECHO_BOLD) $(subst _MISSED,, $(LIST_MISSED)) $(ECHO_NORMAL);
	@$(ECHO) 1. Check $(DEFAULT_MAKEFILE) for module $(MODULE_ID).
	@$(ECHO) 2. Check project configuration file $(PROJECT_CONFIG_FILE).
	@$(ECHO) 3. Contact makefile system maintainer.
endif
	@$(ECHO) $(BLOCK_SEPARATE)

test:
	echo $(OBJS)
	echo OBJ_DIR=$(OBJ_DIR)
	echo OBJ_NEED_REBUILD_BY=$(OBJ_NEED_REBUILD_BY)

#=============================================================================
# Command Line for compilation of "C" files
$(OBJ_DIR)/%$(C_OBJ_FILE_SUFFIX): $(SRC_DIR)/%.c \
                                $(OBJ_NEED_REBUILD_BY)
	@$(ECHO) $(PROMPT_SYMBLO_L2) Compiling $<
	@$(MKDIR) $(dir $@)
	$(AT)$(CC) $< -MMD -MF $(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@) $(CFLAGS) $(INCLUDES) $(OPTIONS) -o $@
	$(AT)sed -e "s/\\\ //g" $(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@) >$(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@).tmp && rm $(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@) && mv $(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@).tmp $(subst $(C_OBJ_FILE_SUFFIX),$(C_DEP_FILE_SUFFIX),$@)


#=============================================================================
# Command Line for compilation of "C++" files
$(OBJ_DIR)/%$(CPP_OBJ_FILE_SUFFIX): $(SRC_DIR)/%.cpp \
                                $(OBJ_NEED_REBUILD_BY)
	@$(ECHO) $(PROMPT_SYMBLO_L2) C++ Compiling $<
	@$(MKDIR) $(dir $@)
	$(AT)$(CC) $< -MMD -MF $(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@) $(CFLAGS) $(INCLUDES) $(OPTIONS) -o $@
	$(AT)sed -e "s/\\\ //g" $(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@) >$(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@).tmp && rm $(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@) && mv $(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@).tmp $(subst $(CPP_OBJ_FILE_SUFFIX),$(CPP_DEP_FILE_SUFFIX),$@)


#=============================================================================
# Command Line .lib generation for LIBS
$(LIB_DIR)/$(LIBS): $(OBJS) $(IMPORTS_IN_LIB) $(MAKEFILE_ROOT)/makelib.mk
	-$(AT)@$(MKDIR) $(LIB_DIR)
	-$(AT)$(AR) $@ $(OBJS) $(IMPORTS_IN_LIB)
	@$(ECHO) $(ECHO_NO_NEWLINE) $(PROMPT_SYMBLO_L1) Generate $@ ...
	$(AT)if [ -f $@ ];then \
                $(ECHO) " [  "$(OK)" ]";\
         else \
                $(ECHO) " [ "$(FAIL) "]";\
         fi

common_help:
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) Common Make Targets:
	@$(ECHO)   "- make help       : show this help info"
	@$(ECHO)   "- make check      : check all the MODULES_USED in $(DEFAULT_MAKEFILE)"
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO)
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) Common Make Option Flags :
	@$(ECHO)
	@$(ECHO)   "- MAKE_VERBOSE = TRUE  : makefile run in verbose mode"
	@$(ECHO)   "               | FALSE : makefile run not in verbose mode"
	@$(ECHO)   "               Default value : $(MAKE_VERBOSE_DEFAULT)"
	@$(ECHO)
	@$(ECHO)   "- RELEASE_MODE  = release : to use toolchain optimisation (-O2)"
	@$(ECHO)   "               | debug   : to compile with -g -O0"
	@$(ECHO)   "               | cutomize: user set option by GENERAL_CFLAGS"
	@$(ECHO)   "               Default value : $(RELEASE_MODE_DEFAULT)"
	@$(ECHO)
	@$(ECHO)   "- MAKE_COLOR   = TRUE  : makefile run with colorful output information."
	@$(ECHO)   "               | FALSE : makefile run without colorful output information."
	@$(ECHO)   "               Default value : $(MAKE_COLOR_DEFAULT)"
	@$(ECHO)
	@$(ECHO)   "- PROJECT_CONFIG_FILE = "$(ECHO_BOLD)" fileName "$(ECHO_NORMAL)": file name of your project configuration."
	@$(ECHO)   "               Default value : $(PROJECT_CONFIG_FILE_DEFAULT)"
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO)
