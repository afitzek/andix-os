#
# Edit these values:
#

####
# Tool variables
####

HOST_TOOLCHAIN_DIR := 
HOST_PREFIX := 

ARM_TOOLCHAIN_DIR := $(ANDIX_DIR)/toolchain/bin/
ARM_PREFIX := arm-none-eabi-
#ARM_TOOLCHAIN_LIB_DIR := /home/andy/apps/CodeSourcery_ARM_EABI_2012/lib/gcc/arm-none-eabi/4.6.3/
ARM_TOOLCHAIN_LIB_DIR := $(ANDIX_DIR)/prebuild/tz_libgcc/

ARM_NEWLIB_LIB_DIR := $(ANDIX_PREBUILD_DIR)/newlib/lib
ARM_NEWLIB_INC_DIR := $(ANDIX_PREBUILD_DIR)/newlib/include

ARM_TROPICSSL_LIB_DIR := $(ANDIX_PREBUILD_DIR)/tropicssl/lib
ARM_TROPICSSL_INC_DIR := $(ANDIX_PREBUILD_DIR)/tropicssl/include

ARM_NORM_TOOLCHAIN_DIR := 
ARM_NORM_PREFIX := arm-linux-gnueabi-
#ARM_NORM_TOOLCHAIN_LIB_DIR := /home/andy/apps/CodeSourcery_ARM_EABI_2012/lib/gcc/arm-none-eabi/4.6.3/

LINUX_KERNEL_DIR := /home/andy/UNI/DiplArbeit/3rdParty/imx-devel/KERNEL

PERL := $(shell which perl)

DOC := $(shell which doxygen)

TZ_QEMU := $(ANDIX_DIR)/qemu/bin/qemu-system-arm

MKIMAGE := /home/andy/UNI/DiplArbeit/source/u-boot/tools/mkimage
