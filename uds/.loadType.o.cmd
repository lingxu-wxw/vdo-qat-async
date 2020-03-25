cmd_/home/hlguo/testspace/vdo-qat-async/uds/loadType.o := gcc -Wp,-MD,/home/hlguo/testspace/vdo-qat-async/uds/.loadType.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include -I./arch/x86/include -Iarch/x86/include/generated  -Iinclude -I./arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -m64 -mno-mmx -mno-sse -mpreferred-stack-boundary=3 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -Wframe-larger-than=2048 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_AVX512=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mindirect-branch=thunk-extern -mindirect-branch-register -DRETPOLINE -Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -pg -mfentry -DCC_USING_FENTRY -fno-inline-functions-called-once -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -std=gnu99 -fno-builtin-memset -Wframe-larger-than=400 -Wno-declaration-after-statement -DUDS_VERSION=\"6.2.0.77\" -I/home/hlguo/testspace/vdo-qat-async/uds  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(loadType)"  -D"KBUILD_MODNAME=KBUILD_STR(uds)" -c -o /home/hlguo/testspace/vdo-qat-async/uds/.tmp_loadType.o /home/hlguo/testspace/vdo-qat-async/uds/loadType.c

source_/home/hlguo/testspace/vdo-qat-async/uds/loadType.o := /home/hlguo/testspace/vdo-qat-async/uds/loadType.c

deps_/home/hlguo/testspace/vdo-qat-async/uds/loadType.o := \
  /home/hlguo/testspace/vdo-qat-async/uds/loadType.h \
  /home/hlguo/testspace/vdo-qat-async/uds/logger.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h \
  /home/hlguo/testspace/vdo-qat-async/uds/loggerDefs.h \
  include/generated/uapi/linux/version.h \

/home/hlguo/testspace/vdo-qat-async/uds/loadType.o: $(deps_/home/hlguo/testspace/vdo-qat-async/uds/loadType.o)

$(deps_/home/hlguo/testspace/vdo-qat-async/uds/loadType.o):
