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
ARM_TOOLCHAIN_LIB_DIR := $(ANDIX_DIR)/toolchain/lib/gcc/arm-none-eabi/4.8.2/
#ARM_TOOLCHAIN_LIB_DIR := $(ANDIX_DIR)/prebuild/tz_libgcc/
#ARM_TOOLCHAIN_LIB_DIR := $(ANDIX_DIR)/prebuild/cs_libgcc/

ARM_NEWLIB_LIB_DIR := $(ANDIX_PREBUILD_DIR)/newlib2/arm-none-eabi/lib
ARM_NEWLIB_INC_DIR := $(ANDIX_PREBUILD_DIR)/newlib2/arm-none-eabi/include

ARM_TROPICSSL_LIB_DIR := $(ANDIX_PREBUILD_DIR)/tropicssl/lib
ARM_TROPICSSL_INC_DIR := $(ANDIX_PREBUILD_DIR)/tropicssl/include

#ARM_NORM_TOOLCHAIN_DIR := $(ANDIX_DIR)/toolchain_norm/bin/
ARM_NORM_TOOLCHAIN_DIR :=
#ARM_NORM_PREFIX := armel-linux-
ARM_NORM_KERN_PREFIX := arm-linux-gnueabi-
ARM_NORM_PREFIX := arm-linux-gnueabihf-
#ARM_NORM_TOOLCHAIN_DIR := 
#ARM_NORM_PREFIX := arm-linux-gnueabi-
#ARM_NORM_TOOLCHAIN_LIB_DIR := /home/andy/apps/CodeSourcery_ARM_EABI_2012/lib/gcc/arm-none-eabi/4.6.3/

LINUX_KERNEL_DIR := $(ANDIX_DIR)/../linux-linaro-tracking/
#LINUX_KERNEL_DIR := $(ANDIX_DIR)/../imx-devel/KERNEL/
ROOTFS_LIBDIR := $(ANDIX_DIR)/../linaro-userland/binary/lib/arm-linux-gnueabihf
# QEMU KERNEL
# QEMU ANDROID KERNEL 

PERL := $(shell which perl)

DOC := $(shell which doxygen)
#DOC := /home/andy/apps/doxygen_git/bin/doxygen

TZ_QEMU := $(ANDIX_DIR)/qemu/bin/qemu-system-arm

MKIMAGE := /usr/bin/mkimage
