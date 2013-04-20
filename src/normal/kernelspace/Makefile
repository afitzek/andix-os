TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include vars.mk
include config.mk
include rules.mk

###############################################################################

####
# Configure sources
####

PROGRAM = norm/linux/service_mod/
OUTDIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)
D_DIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)doc
OUTDIR_DOC = $(subst /,\/,$(D_DIR))

DOC_LOGO = $(ANDIX_DOC_DIR)/logo.png
E_DOC_LOGO = $(subst /,\/,$(DOC_LOGO))

C_SRCS = $(shell find -name '*.[c]' | sed 's|./||')
C_OBJS = $(addprefix $(OUTDIR), $(addsuffix .o,$(basename $(C_SRCS))))
C_DIRS = $(sort $(dir $(C_OBJS)))

A_SRCS = $(shell find -name '*.[S]' | sed 's|./||')
A_OBJS = $(addprefix $(OUTDIR), $(addsuffix .o,$(basename $(A_SRCS))))
A_DIRS = $(sort $(dir $(A_OBJS)))

OBJS = $(C_OBJS) $(A_OBJS)

DIRS = $(sort $(C_DIRS) $(A_DIRS))

###############################################################################

obj-m := andixtz.o andixtee.o
andixtz-objs := tz_service_mod/utils.o tz_service_mod/module_main.o tz_service_mod/andixservice.o tz_service_mod/tz_file_ops.o
andixtee-objs := tz_application_mod/utils.o tz_application_mod/module_main.o tz_application_mod/andixservice.o tz_application_mod/tz_file_ops.o
ccflags-y := -mcpu=cortex-a8 -I $(NORM_K_INC) -I $(ANDIX_INC_COMMON)

KBUILD_OUTPUT := $(OUTDIR)

all:
	@echo $(INC_DIR)
	@mkdir -p $(OUTDIR)
	@mkdir -p $(ANDIX_DEPLOY_DIR)
	@make -C $(LINUX_KERNEL_DIR) EXTRA_CFLAGS=$(EXTRACFLAGS) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- M=`pwd` modules
	@cp *.ko $(OUTDIR)
	@cp *.ko $(ANDIX_DEPLOY_DIR)/
	@make -C $(LINUX_KERNEL_DIR) EXTRA_CFLAGS=$(EXTRACFLAGS) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- M=`pwd` clean
	
clean:
	make -C $(LINUX_KERNEL_DIR) EXTRA_CFLAGS=$(EXTRACFLAGS) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- M=`pwd` clean