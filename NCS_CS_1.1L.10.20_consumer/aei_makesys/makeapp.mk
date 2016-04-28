# =============================================================================
# makeapp.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
# =============================================================================
# GENERIC MAKEFILE TO GENERATE A Out file which can be executed
# This file is included in the $(DEFAULT_MAKEFILE)
# =============================================================================
# it requires:
# MODULE_ID    which reflects the executable to build
# MODULES_USED which reflects the dependencies and so the includes path
# INCLUDES     which relects  some specific include needs
#
# general variables that reflect the toolset command:
# INCLUDE_PREFIX          prefix to path where to find header files
# INCLUDE_LIBRARY_PREFIX  prefix to libraries to link with
# LIBRARY_PREFIX          prefix to path where to find libraries
# MKDIR                   command to create needed directory (with no error if it already exists)
#
# optional flags used for compilation
# CFLAGS which is a compilation flag
# OPTIONS used the same way as CFLAGS but should be used for conditionnal compilation flags

include $(MAKEFILE_ROOT)/defrules.mk

# Get all the dependencies for this module which is an app module
MODULE_DEP_$(MODULE_ID)=$(sort $(MODULE_DIRECT_DEP_$(MODULE_ID)) \
                          $(foreach m,$(MODULE_DIRECT_DEP_$(MODULE_ID)),$(MODULE_DEP_$(m))))

#=============================================================================
# Get some private link flags for all depended modules
DEP_MODULES_PRIVATE_LFLAGS = $(foreach module, $(LIST_USED), $(MODULE_PRIVATE_LFLAGS_$(module)))
LFLAGS += $(DEP_MODULES_PRIVATE_LFLAGS)

# ============================================================================
# Defines paths
TARGET_APP_SUFFIX ?= .exe
TARGET_APP   := $(MODULE_ID)$(TARGET_APP_SUFFIX)
TARGET_MAP   := $(MODULE_ID).map


# =============================================================================
# Global Compilation flags

#========= Modules not used passed for the compilation

MODULES_NOT_USED := $(filter-out  $(MODULES_USED), $(ALL_MODULES_LIST))
export MODULES_NOT_USED_CFLAGS := $(addsuffix _NOT_USED,$(addprefix -DMODULE_,$(MODULES_NOT_USED)))

# ------ Modules which have source code to compile or clean
LIB_TO_BUILD := $(foreach module,$(filter-out $(MODULE_WITHOUT_SOURCE) $(MODULE_WITHOUT_LIB),$(LIST_USED)),$(module).lib)
LIB_TO_CLEAN := $(subst .lib,.clean, $(LIB_TO_BUILD))

# ============= To define the libraries path : -Lpath/xxx/$(LIB_DIR) ========
PATH_IMPORTS_LIBS += $(foreach module,$(LIST_USED), $(LIBRARY_PREFIX)$(MODULE_PATH_$(module))/$(LIB_DIR))

# ============= To define the libraries list : -llibxxx =======================
# Only link the found modules' lib
IMPORTS_LIBS += $(foreach module,$(filter-out $(MODULE_WITHOUT_LIB),$(LIST_USED)),$(INCLUDE_LIBRARY_PREFIX)$(module))

# for info define modules which have no source code
LIB_TO_LINK_WITHOUT_SRC := $(filter $(MODULE_WITHOUT_SOURCE),$(LIST_USED))


all: check $(TARGET_APP)

#=============================================================================
# Command Line to check the modules list
check: check_module_dep

.PHONY: force app appf link_appf clean showinfo

# =============================================================================
# ========= Command Line for LKU generation
app: $(TARGET_APP)

$(TARGET_APP): $(OBJS)          \
               $(LIB_TO_BUILD)  \
               showinfo         \
               link_appf

appf: $(OBJS) link_appf

showinfo:
	@$(ECHO)
	@$(ECHO) ..........
	@$(ECHO) All needed libs are built
	@$(ECHO) $(BLOCK_SEPARATE)

link_appf:
	@$(ECHO)
ifneq ($(strip $(LIB_TO_LINK_WITHOUT_SRC)),)
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) The following library modules come without source code: $(LIB_TO_LINK_WITHOUT_SRC)
	@$(ECHO) $(BLOCK_SEPARATE)
endif
	-$(AT)$(CC) $(OBJS) $(PATH_IMPORTS_LIBS) $(LFLAGS) -Xlinker -Map=$(TARGET_MAP) -u _main -Wl,--start-group $(IMPORTS_LIBS) -Wl,--end-group -o $(TARGET_APP)
	@$(ECHO) $(ECHO_NO_NEWLINE) $(PROMPT_SYMBLO_L1) Creating APP $(TARGET_APP) ...
	$(AT)if [ -f $(TARGET_APP) ];then \
                $(ECHO) " [  "$(OK)" ]";\
         else \
                $(ECHO) " [ "$(FAIL) "]";\
         fi

#========= Command Line for lib generation
#To rebuld a module: make module_name.lib.rebuild
%.lib:
	@$(ECHO) $(PROMPT_SYMBLO_L1) Building $*
	$(AT)$(MAKE) --no-print-directory -C $(MODULE_PATH_$*)

%.lib.rebuild:
	@$(ECHO) $(PROMPT_SYMBLO_L1) Rebuilding $*
	$(AT)$(MAKE) --no-print-directory -C $(MODULE_PATH_$*) clean
	$(AT)$(MAKE) --no-print-directory -C $(MODULE_PATH_$*)

%$(LIB_FILE_SUFFIX): force
	@$(ECHO)
#	@$(ECHO) $(PROMPT_SYMBLO_L1) Making \$(*F) $(@D)
	$(AT)$(MAKE) --no-print-directory -C $(@D)/..

%.clean:
	@$(ECHO) $(LINE_SEPARATE)
	@$(ECHO) $(PROMPT_SYMBLO_L1) Cleaning ...... $*
	$(AT)$(MAKE) --no-print-directory -C $(MODULE_PATH_$*) clean

# =============================================================================
# Clean Macro
clean:
	@$(ECHO) $(PROMPT_SYMBLO_L1) Cleaning output files for $(MODULE_ID)
	-$(AT)$(RM) -r $(subst $(BAD_SLASH),$(GOOD_SLASH),$(OBJ_DIR)/*)
	-$(AT)$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TARGET_MAP))
	-$(AT)$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(TARGET_APP))

# =============================================================================
# CleanAll Macro
cleanall: clean $(LIB_TO_CLEAN)

# =============================================================================
# Run target
run: $(TARGET_APP) runf

# =============================================================================
# Fast Run target
runf:
	@$(ECHO) $(WARNING_INFO) $@ is under implementation!

# =============================================================================
# Run target with the debugger
dbg: $(TARGET_APP) dbgf

# =============================================================================
# Fast Run target with the debugger
dbgf:
	@$(ECHO) $(WARNING_INFO) $@ is under implementation!


# =============================================================================
# Configure each modules
#
config:
	$(AT)while [ 1=1 ]; do\
    $(ECHO) $(CENTER_SCREEN) "+++++++++++++++++++++++++++++++++++++++++++++++" ;\
    $(ECHO) $(CENTER_SCREEN) "++  $(MODULE_ID) Configure Modules Menu  +++" ;\
    $(ECHO) $(CENTER_SCREEN) "+++++++++++++++++++++++++++++++++++++++++++++++" ;\
    $(ECHO) ;\
    i=0; \
    for module in $(LIST_USED); do\
        i=$$[$$i+1]; \
        $(ECHO) $(CENTER_SCREEN) [$$i] $$module  $(PROMPT_SYMBLO_L1);\
    done; \
    $(ECHO) $(CENTER_SCREEN) all use [D]efault or  [Q]uit!;\
    $(ECHO) -n $(CENTER_SCREEN) "Choose the module you want to configure:";\
    read moduleIndex; \
    \
    case $$moduleIndex in \
    Q): ;\
        exit 0;\
    ;;\
    D): ;\
        $(ECHO) -n Do you really want to use default configuration for all the above modules [y/n]:; \
        read useDefConf; \
         if [ "$$useDefConf" = "y" ]; then\
            i=0 ;\
            for module in $(LIST_USED); do\
                i=$$[$$i+1]; \
                $(ECHO) $(PROMPT_SYMBLO_L1) Set defalut configuration for $. $$module ...;\
            done; \
         fi;\
    ;; \
    *): ;\
    if [ $$moduleIndex -gt $$i -o $$moduleIndex -lt 1 ]; then \
            $(ECHO) wrong input;\
    else \
         config_script="`$(ECHO) $(MODULE_PATHS_LIST)|cut -d' ' -f$$moduleIndex`/configure"; \
         if [ -f "$$config_script" ]; then\
              . $$config_script;\
         else \
              $(ECHO) $$config_script NOT FOUND;\
         fi;\
    fi;\
    esac;\
    done

help: makeapp_help common_help

makeapp_help:
	@$(ECHO) $(BLOCK_SEPARATE)
	@$(ECHO) Make Out Targets:
	@$(ECHO)
	@$(ECHO)   "- make            : to create the exec $(TARGET_APP)"
	@$(ECHO)   "- make config     : to configure all dependent modules"
	@$(ECHO)   "- make appf       : to create the exec $(TARGET_APP) but do not recreate nor check libraries"
	@$(ECHO)   "- make link_appf  : same as appf but do not test $(OBJS) age"
	@$(ECHO)   "- make clean      : to clean obj, lib, and app files of this module"
	@$(ECHO)   "- make cleanall   : to clean as clean  + all dependancies"
	@$(ECHO)   "- make run        : to generate and run the exec $(TARGET_APP)"
	@$(ECHO)   "- make runf       : to run directly the exec $(TARGET_APP)"
	@$(ECHO)   "- make dbg        : to generate and to run the exec $(TARGET_APP) with debugger"
	@$(ECHO)   "- make dbgf       : to run the exec $(TARGET_APP) with debugger"
	@$(ECHO) $(LINE_SEPARATE)
	@$(ECHO) Special Flags :
	@$(ECHO)
	@$(ECHO) $(LINE_SEPARATE)

include $(MAKEFILE_ROOT)/makecommon.mk
