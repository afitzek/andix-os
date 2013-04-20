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

PERL := $(shell which perl)

DOC := $(shell which doxygen)

MKIMAGE := /home/andy/UNI/DiplArbeit/source/u-boot/tools/mkimage

#
# Only edit the following if you know why ...
#

CC := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)gcc
AS := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)gcc
AR := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)ar
NM := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)nm
OBJCOPY := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)objcopy
LD := $(HOST_TOOLCHAIN_DIR)$(HOST_PREFIX)ld


ARMCC := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)gcc
ARMAS := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)gcc
ARMAR := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)ar
ARMNM := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)nm
ARMOBJCOPY := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)objcopy
ARMLD := $(ARM_TOOLCHAIN_DIR)$(ARM_PREFIX)ld