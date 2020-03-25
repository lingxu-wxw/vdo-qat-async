cmd_/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o := gcc -Wp,-MD,/home/hlguo/testspace/vdo-qat-async/vdo/base/.compressionState.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include -I./arch/x86/include -Iarch/x86/include/generated  -Iinclude -I./arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -m64 -mno-mmx -mno-sse -mpreferred-stack-boundary=3 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -Wframe-larger-than=2048 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -DCONFIG_AS_AVX512=1 -DCONFIG_AS_SHA1_NI=1 -DCONFIG_AS_SHA256_NI=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -mindirect-branch=thunk-extern -mindirect-branch-register -DRETPOLINE -Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -pg -mfentry -DCC_USING_FENTRY -fno-inline-functions-called-once -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -std=gnu99 -fno-builtin-memset -Wframe-larger-than=400 -Wno-declaration-after-statement -DVDO_VERSION_MAJOR=6 -DVDO_VERSION_MINOR=2 -DVDO_VERSION_MICRO=0 -DCURRENT_VERSION=\"6.2.0.293\" -I/home/hlguo/testspace/vdo-qat-async/vdo/base -I/home/hlguo/testspace/vdo-qat-async/vdo/kernel -I/home/hlguo/testspace/vdo-qat-async/vdo/../uds -I/QAT//quickassist/include  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(compressionState)"  -D"KBUILD_MODNAME=KBUILD_STR(kvdo)" -c -o /home/hlguo/testspace/vdo-qat-async/vdo/base/.tmp_compressionState.o /home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.c

source_/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o := /home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.c

deps_/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o := \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/compressionStateInternals.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/atomic.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/atomicDefs.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  arch/x86/include/asm/atomic.h \
    $(wildcard include/config/x86/32.h) \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/uapi/linux/types.h \
  arch/x86/include/uapi/asm/types.h \
  include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/x86/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  include/uapi/asm-generic/bitsperlong.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  arch/x86/include/asm/posix_types.h \
  arch/x86/include/uapi/asm/posix_types_64.h \
  include/uapi/asm-generic/posix_types.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  arch/x86/include/asm/alternative.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/paravirt.h) \
  include/linux/stringify.h \
  arch/x86/include/asm/asm.h \
  arch/x86/include/asm/ptrace.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
  arch/x86/include/asm/segment.h \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
  include/uapi/linux/const.h \
  arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
    $(wildcard include/config/x86/internode/cache/shift.h) \
    $(wildcard include/config/x86/vsmp.h) \
  include/linux/linkage.h \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/alignment/16.h) \
  arch/x86/include/asm/page_types.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
  include/linux/mem_encrypt.h \
    $(wildcard include/config/arch/has/mem/encrypt.h) \
    $(wildcard include/config/amd/mem/encrypt.h) \
  arch/x86/include/asm/mem_encrypt.h \
  include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
  arch/x86/include/uapi/asm/bootparam.h \
  include/linux/screen_info.h \
  include/uapi/linux/screen_info.h \
  include/linux/apm_bios.h \
  include/uapi/linux/apm_bios.h \
  include/uapi/linux/ioctl.h \
  arch/x86/include/uapi/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/uapi/asm-generic/ioctl.h \
  include/linux/edd.h \
  include/uapi/linux/edd.h \
  arch/x86/include/asm/e820.h \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memtest.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  arch/x86/include/uapi/asm/e820.h \
    $(wildcard include/config/x86/pmem/legacy.h) \
    $(wildcard include/config/intel/txt.h) \
  include/linux/ioport.h \
    $(wildcard include/config/memory/hotremove.h) \
  arch/x86/include/asm/ist.h \
  arch/x86/include/uapi/asm/ist.h \
  include/video/edid.h \
    $(wildcard include/config/x86.h) \
  include/uapi/video/edid.h \
  arch/x86/include/asm/page_64_types.h \
    $(wildcard include/config/randomize/memory.h) \
    $(wildcard include/config/randomize/base.h) \
  arch/x86/include/asm/kaslr.h \
  arch/x86/include/uapi/asm/ptrace.h \
  arch/x86/include/uapi/asm/ptrace-abi.h \
  arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  arch/x86/include/uapi/asm/processor-flags.h \
  arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/queued/spinlocks.h) \
    $(wildcard include/config/paravirt/debug.h) \
  arch/x86/include/asm/desc_defs.h \
  arch/x86/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  include/asm-generic/kmap_types.h \
  arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/x86/intel/memory/protection/keys.h) \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/page/table/isolation.h) \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  arch/x86/include/asm/pgtable_64_types.h \
  arch/x86/include/asm/sparsemem.h \
    $(wildcard include/config/sparsemem.h) \
  include/linux/rh_kabi.h \
    $(wildcard include/config/rh/kabi/size/align/checks.h) \
  arch/x86/include/asm/spinlock_types.h \
    $(wildcard include/config/paravirt/spinlocks.h) \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/queue/rwlock.h) \
  include/asm-generic/qspinlock_types.h \
  arch/x86/include/asm/rwlock.h \
  include/asm-generic/ptrace.h \
  arch/x86/include/asm/cmpxchg.h \
  arch/x86/include/asm/cpufeatures.h \
  arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/cmov.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
    $(wildcard include/config/matom.h) \
  arch/x86/include/asm/disabled-features.h \
    $(wildcard include/config/x86/intel/mpx.h) \
    $(wildcard include/config/x86/intel/umip.h) \
  arch/x86/include/asm/cmpxchg_64.h \
  arch/x86/include/asm/barrier.h \
    $(wildcard include/config/x86/ppro/fence.h) \
  arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  arch/x86/include/asm/atomic64_64.h \
  include/asm-generic/atomic-long.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/compiler.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/compilerDefs.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/typeDefs.h \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stdarg.h \
  include/linux/bitops.h \
  arch/x86/include/asm/bitops.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  arch/x86/include/asm/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/le.h \
  arch/x86/include/uapi/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/uapi/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  include/uapi/linux/swab.h \
  arch/x86/include/uapi/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/kern_levels.h \
  include/linux/dynamic_debug.h \
  include/uapi/linux/kernel.h \
  include/uapi/linux/sysinfo.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/types.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/blockMappingState.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/common.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/stringUtils.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/compiler.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/stringDefs.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/uapi/linux/string.h \
  arch/x86/include/asm/string.h \
  arch/x86/include/asm/string_64.h \
    $(wildcard include/config/x86/mce.h) \
    $(wildcard include/config/arch/has/uaccess/flushcache.h) \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  arch/x86/include/asm/jump_label.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/typeDefs.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds.h \
    $(wildcard include/config/256mb.h) \
    $(wildcard include/config/512mb.h) \
    $(wildcard include/config/768mb.h) \
    $(wildcard include/config/max.h) \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds-platform.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds-block.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/statusCodes.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/kernel/errors.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds-error.h \
    $(wildcard include/config/inconsistent.h) \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/dataVIO.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/allocatingVIO.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/pbnLock.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/physicalZone.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/vio.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/completion.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/permassert.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/errors.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds-error.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/physicalLayer.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/ringNode.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/trace.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/kernel/threads.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/vdo.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/waitQueue.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/blockMapEntry.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/constants.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/numeric.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/numericDefs.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/hashZone.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/uds.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/statistics.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/header.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/buffer.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/../uds/common.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/journalPoint.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/logicalZone.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/intMap.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/referenceOperation.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/threadConfig.h \
    $(wildcard include/config/h.h) \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/vdoPageCache.h \
  /home/hlguo/testspace/vdo-qat-async/vdo/base/packer.h \

/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o: $(deps_/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o)

$(deps_/home/hlguo/testspace/vdo-qat-async/vdo/base/compressionState.o):
