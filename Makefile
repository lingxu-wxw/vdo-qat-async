KBUILD_EXTRA_SYMBOLS += $(ICP_ROOT)/quickassist/lookaside/access_layer/src/Module.symvers
obj-y += uds/
obj-y += vdo/

.PHONY: install clean
install:
	make -C /usr/src/kernels/`uname -r` M=`pwd`
clean:
	rm -f *.o .*.o.cmd *.mod.c *.order *.symvers
	rm -f .tmp_versions/.*.mod
	rm -rf .tmp_versions/
	rm -f uds/*.o uds/*.ko uds/.*.o.cmd uds/.*.ko.cmd
	rm -f uds/util/*.o uds/util/.*.o.cmd
	rm -f uds/murmur/*.o uds/murmur/.*.o.cmd
	rm -f vdo/*.o vdo/*.ko vdo/.*.o.cmd vdo/.*.ko.cmd vdo/*.mod.c vdo/*.order
	rm -f vdo/base/*.o vdo/base/.*.o.cmd
	rm -f vdo/kernel/*.o vdo/kernel/.*.o.cmd
