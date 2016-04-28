# =============================================================================
# defrules.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
# Here are defined the common definitions for library and executable,
# it is included near the beginning of the modules makefiles

# Write here just for making sure the target "all" is default target
all:

# =============================================================================
DEFAULT_MAKEFILE=Makefile

OBJ_NEED_REBUILD_BY += $(MAKEFILE_ROOT)/defrules.mk     \
                     $(MAKEFILE_ROOT)/toolchains.mk   \
                     $(MAKEFILE_ROOT)/makecommon.mk \
                     $(MAKEFILE_ROOT)/makelib.mk \
                     $(MAKEFILE_ROOT)/makeapp.mk \
                     $(MAKEFILE_ROOT)/project_config.mk \
                     $(DEFAULT_MAKEFILE)

# -----------------------------------------
# Path for module exported header files
INCLUDE_DIR := include

# -----------------------------------------
# Path for the local object directory
OBJ_DIR := obj

# -----------------------------------------
# Path for the local library directory
LIB_DIR := lib

# -------------------------------------------------
# Path for the local executable bin file directory
BIN_DIR := bin

# -----------------------------------------
# Prefix of a library file
LIB_FILE_PREFIX := lib

# -----------------------------------------
# Suffix of a library file
LIB_FILE_SUFFIX := .a

# -----------------------------------------
# Suffix of an object file
C_OBJ_FILE_SUFFIX := .o
CPP_OBJ_FILE_SUFFIX := .oo

# -----------------------------------------
# Suffix of a dependency file
C_DEP_FILE_SUFFIX := .d
CPP_DEP_FILE_SUFFIX := .dd

# ========================
# Compilation Flag Set
# ========================

# -----------------------------------------
# Debug Flag compilation option
# use -g -O0 if debug without optimisation
# use -O2 if optimisation required
# use -pipe to not generate temporary file
#CFLAGS += -Wall -Wno-unknown-pragmas -ansi -pipe -c
CFLAGS += -Wall -Werror -Wno-unknown-pragmas -pipe -c


ifeq ($(RELEASE_MODE),release)
  CFLAGS += -O2
  LFLAGS +=
else
ifeq ($(RELEASE_MODE),debug)
  CFLAGS +=  -g -O0
  LFLAGS +=
else
ifeq ($(RELEASE_MODE),cutomize)
  CFLAGS += $(GENERAL_CFLAGS)
  LFLAGS += $(GENERAL_LFLAGS)
  export GENERAL_CFLAGS
  export GENERAL_LFLAGS
endif
endif
endif


# -----------------------------------------
# Makefile run as verbose mode
MAKE_VERBOSE_DEFAULT:=FALSE
MAKE_VERBOSE ?= $(MAKE_VERBOSE_DEFAULT)
ifeq ($(MAKE_VERBOSE),TRUE)
AT=
else
AT=@
endif

#=============================================================================
# Modules list needed for the compilation
#ifdef SOURCES
# because some modules don't have source file list,
# and don't need dependencises
# If goes here, SOURCES must contain some nonspace characters.

# The SOURCES should only contain c or cpp files.
SOURCES_FILTER = $(filter %.c %.cpp, $(SOURCES))
ifneq ($(strip $(SOURCES_FILTER)),$(strip $(SOURCES)))
   $(error Please check your makefile. The SOURCES should only contain c or cpp files.)
endif

OBJS = $(addprefix $(OBJ_DIR)/, $(subst .c,$(C_OBJ_FILE_SUFFIX),$(subst .cpp,$(CPP_OBJ_FILE_SUFFIX),$(strip $(SOURCES)) ) ) )
DEPS = $(addprefix $(OBJ_DIR)/, $(subst .c,$(C_DEP_FILE_SUFFIX),$(subst .cpp,$(CPP_DEP_FILE_SUFFIX),$(strip $(SOURCES)) ) ) )
#endif
#=============================================================================
# Modules list needed for the compilation
MODULES_USED=$(MODULE_DEP_$(MODULE_ID))
LIST_USED = $(filter  $(MODULES_USED), $(ALL_MODULES_LIST))

#=============================================================================
# List the missed modules
LIST_MISSED = $(addsuffix _MISSED, $(filter-out $(ALL_MODULES_LIST), $(MODULES_USED)))

#=============================================================================
# List path not defined for used modules
LIST_PATH_NOT_DEFINED_USED_MODULES = $(filter $(LIST_USED), $(foreach module, $(LIST_USED), $(MODULE_PATH_$(module))$(module)))

#=============================================================================
# Include list needed for the compilation
INCLUDES_USED_MODULE = $(foreach module, $(LIST_USED), $(INCLUDE_PREFIX)$(MODULE_PATH_$(module))/$(INCLUDE_DIR))

INCLUDES += \
            $(INCLUDE_PREFIX)./$(INCLUDE_DIR)  \
            $(INCLUDES_USED_MODULE)

#=============================================================================
# All used modules' path list
#=============================================================================
MODULE_PATHS_LIST = $(foreach module, $(LIST_USED), $(MODULE_PATH_$(module)))

#========================================================
# For display look and feel
#========================================================
MAKE_COLOR_DEFAULT=TRUE
MAKE_COLOR := $(MAKE_COLOR_DEFAULT)
ifeq ($(MAKE_COLOR),TRUE)
ECHO = echo -e

ECHO_NORMAL      = "\033[0m"
ECHO_BOLD        = "\033[1m"
ECHO_ULINE       = "\033[4m"
ECHO_FLASH       = "\033[5m"

ECHO_FG_BLACK    = "\033[30m"
ECHO_FG_RED      = "\033[31m"
ECHO_FG_GREEN    = "\033[32m"
ECHO_FG_YELLOW   = "\033[33m"
ECHO_FG_BLUE     = "\033[34m"
ECHO_FG_PINK     = "\033[35m"
ECHO_FG_SKY_BLUE = "\033[36m"

ECHO_BG_RED      = "\033[41m"
ECHO_BG_GREEN    = "\033[42m"
ECHO_BG_YELLOW   = "\033[43m"
ECHO_BG_BLUE     = "\033[44m"
ECHO_BG_PINK     = "\033[45m"
ECHO_BG_SKY_BLUE = "\033[46m"
ECHO_BG_WHITE    = "\033[47m"
else
ECHO    = echo

ECHO_NORMAL      = ""
ECHO_BOLD        = ""
ECHO_ULINE       = ""
ECHO_FLASH       = ""

ECHO_FG_BLACK    = ""
ECHO_FG_RED      = ""
ECHO_FG_GREEN    = ""
ECHO_FG_YELLOW   = ""
ECHO_FG_BLUE     = ""
ECHO_FG_PINK     = ""
ECHO_FG_SKY_BLUE = ""

ECHO_BG_RED      = ""
ECHO_BG_GREEN    = ""
ECHO_BG_YELLOW   = ""
ECHO_BG_BLUE     = ""
ECHO_BG_PINK     = ""
ECHO_BG_SKY_BLUE = ""
ECHO_BG_WHITE    = ""
endif

ECHO_NO_NEWLINE = -n  # Linux echo flag

CENTER_SCREEN ="                                   "
LINE_SEPARATE ="-------------------------------------------------------"
BLOCK_SEPARATE="======================================================="

PROMPT_SYMBLO_L1=$(ECHO_BOLD)$(ECHO_FG_GREEN)">>>>>"$(ECHO_NORMAL)
CLOSE_SYMBLO_L1 =$(ECHO_BOLD)$(ECHO_FG_GREEN)"<<<<<"$(ECHO_NORMAL)

PROMPT_SYMBLO_L2=$(ECHO_BOLD)$(ECHO_FG_GREEN)">>>>>>>"$(ECHO_NORMAL)
CLOSE_SYMBLO_L2 =$(ECHO_BOLD)$(ECHO_FG_GREEN)"<<<<<<<"$(ECHO_NORMAL)

PROMPT_SYMBLO_L3=$(ECHO_BOLD)$(ECHO_FG_GREEN)">>>>>>>>>>"$(ECHO_NORMAL)
CLOSE_SYMBLO_L3 =$(ECHO_BOLD)$(ECHO_FG_GREEN)"<<<<<<<<<<"$(ECHO_NORMAL)

WARNING_INFO= $(ECHO_FG_YELLOW)$(ECHO_FLASH)$(ECHO_BOLD)"[WARNING]:"$(ECHO_NORMAL)
ERROR_INFO= $(ECHO_FG_RED)$(ECHO_FLASH)$(ECHO_BOLD)"[ERROR]:"$(ECHO_NORMAL)
OK= $(ECHO_FG_GREEN)$(ECHO_BOLD)"OK"$(ECHO_NORMAL)
FAIL= $(ECHO_FG_RED)$(ECHO_FLASH)$(ECHO_BOLD)"FAIL"$(ECHO_NORMAL)
