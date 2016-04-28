PROJECT_ROOT ?= ../../../
AEI_MKSYS_ROOT ?= $(PROJECT_ROOT)/aei_makesys
PROJECT_CONFIG_FILE = aei_wxx6000_host.prj
MAKEFILE_ROOT=$(AEI_MKSYS_ROOT)
#========= Include project configuration
include $(MAKEFILE_ROOT)/project_config.mk

MODULE_ID:=aei_img_sign
MODULE_VERSION:=1.0

#The direct depended modules of an application just specify here is ok,
#no need to specify in $(MAKEFILE_ROOT)/projectname.prj
MODULE_DIRECT_DEP_$(MODULE_ID):=

# SRC_DIR specify a relative directory to this makefile,
# all the source files specified in SOURCES starts from this directory.
# Note: to avoid multi-adjacent slash in source code file location, do not add the last slash, makefile system will add it;
#
# We found that duplicate slash ("//") in source code file location will raise issue when use certain gdb, like "sh4gdb -w", to debug program.
SRC_DIR = src
SOURCES = main.c \
          ais_block.c \
          ais_block_show.c \
          crc32.c \

include $(MAKEFILE_ROOT)/makeapp.mk

#Extend some default targets
help: $(MODULE_ID)_help

$(MODULE_ID)_help:
	@echo "No more info for $(MODULE_ID) help"
