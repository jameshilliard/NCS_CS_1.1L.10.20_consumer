#related directory to aei_makesys
AEI_MKSYS_ROOT ?= ../../aei_makesys

exclude_dirs = dir1 dir2 #define directories which skip the making process
include $(AEI_MKSYS_ROOT)/travel_dir.mk
