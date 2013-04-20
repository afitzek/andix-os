#
# Edit these values:
#

####
# Tool variables
####

HOST_TOOLCHAIN_DIR := 
HOST_PREFIX := 

ARM_TOOLCHAIN_DIR := /home/andy/UNI/DiplArbeit/source/toolchain/bin/bin/
ARM_PREFIX := arm-none-eabi-
ARM_TOOLCHAIN_LIB_DIR := /home/andy/apps/CodeSourcery_ARM_EABI_2012/lib/gcc/arm-none-eabi/4.6.3/

ARM_NEWLIB_LIB_DIR := $(ANDIX_PREBUILD_DIR)/newlib/lib
ARM_NEWLIB_INC_DIR := $(ANDIX_PREBUILD_DIR)/newlib/include

ARM_POLARSSL_LIB_DIR := $(ANDIX_PREBUILD_DIR)/polarSSL/lib
ARM_POLARSSL_INC_DIR := $(ANDIX_PREBUILD_DIR)/polarSSL/include

LINUX_KERNEL_DIR := /home/andy/UNI/DiplArbeit/3rdParty/imx-devel/KERNEL

PERL := $(shell which perl)

DOC := $(shell which doxygen)

MKIMAGE := /home/andy/UNI/DiplArbeit/source/u-boot/tools/mkimage
