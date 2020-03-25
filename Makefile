KBUILD_EXTRA_SYMBOLS += $(ICP_ROOT)/quickassist/lookaside/access_layer/src/Module.symvers
obj-y += uds/
obj-y += vdo/

.PHONY: install clean
install:
	make -C /usr/src/kernels/`uname -r` M=`pwd`
clean:
	rm -f *.o
	rm -f *.mod.c
	rm -f *.order
	rm -f *.symvers
	rm -f uds/*.o
	rm -f uds/*.ko
	rm -f uds/util/*.o
	rm -f uds/murmur/*.o
	rm -f vdo/*.o
	rm -f vdo/*.ko
	rm -f vdo/base/*.o
	rm -f vdo/kernel/*.o
