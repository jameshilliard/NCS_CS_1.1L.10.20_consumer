PWD = $(shell pwd)
KERNELRELEASE = $(shell cat $(KERNEL_DIR)/include/config/kernel.release 2> /dev/null)

obj-m := dpi.o

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
	install dpi.ko $(PROFILE_DIR)/modules/lib/modules/$(KERNELRELEASE)/extra

clean:
	rm *.o
	rm *.ko
	rm modules.order
	rm Module.symvers
	rm *.mod.c
