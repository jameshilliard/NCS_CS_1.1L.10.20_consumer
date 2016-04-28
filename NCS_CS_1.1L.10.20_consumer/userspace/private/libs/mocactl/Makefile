LIB = libmocactl.so
default: dynamic


libmocactl.so:
	cp libmocactl.so_$(PROFILE).save libmocactl.so

dynamic: generic_private_lib_install

clean:
	rm -f libmocactl.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
