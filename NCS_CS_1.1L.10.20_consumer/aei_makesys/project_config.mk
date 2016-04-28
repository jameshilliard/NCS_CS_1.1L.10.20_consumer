# =============================================================================
# project_config.mk
# Copyright (C) 2006-2015 Ken HU
# =============================================================================
# Write here just for making sure the target "all" is default target
all:

PROJECT_CONFIG_FILE_DEFAULT := aei_wxx6000.prj
PROJECT_CONFIG_FILE ?= $(PROJECT_CONFIG_FILE_DEFAULT)

include $(MAKEFILE_ROOT)/$(PROJECT_CONFIG_FILE)
