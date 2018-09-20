ccflags-y := -std=gnu99 -Wno-declaration-after-statement

# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := raven.o
# Otherwise we were called directly from the command
# line; invoke the kernel buildEventReader system.
else
	KERNELDIR ?= /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
install:
	sudo insmod raven.ko
	sudo chmod 666 /dev/raven
uninstall:
	sudo rmmod raven
reload:
	sudo rmmod raven
	sudo insmod raven.ko
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order
endif
