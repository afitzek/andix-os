TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/newlib_c/
OUTDIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)
D_DIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)doc
OUTDIR_DOC = $(subst /,\/,$(D_DIR))

DOC_LOGO = $(realpath $(TOP))/doc/logo.png
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


LIBC_NAME = libc.a
LIBGCC_NAME = libgcc.a
LIBC_FILE = $(ARM_NEWLIB_LIB_DIR)/$(LIBC_NAME)
LIBGCC_FILE = $(ARM_TOOLCHAIN_LIB_DIR)/$(LIBGCC_NAME)

ASFLAGS = -mcpu=cortex-a8
CFLAGS = -I $(ARM_NEWLIB_INC_DIR) -I $(TZ_USER_C_INC) -mcpu=cortex-a8 -nostdlib
LDFLAGS = -nostdlib

all: mkbuilddirs $(OUTDIR)libandixC.a

$(OUTDIR)libandixC.a: $(OBJS)
	@mkdir -p $(OUTDIR)tmp/libc
	@mkdir -p $(OUTDIR)tmp/libgcc
	@cp $(LIBC_FILE) $(OUTDIR)tmp/libc/
	@cp $(LIBGCC_FILE) $(OUTDIR)tmp/libgcc/
	@echo Extracting $(LIBC_NAME)
	@cd $(OUTDIR)tmp/libc/; $(ARMAR) xo $(LIBC_NAME)
	@echo Extracting $(LIBGCC_NAME)
	@cd $(OUTDIR)tmp/libgcc/; $(ARMAR) xo $(LIBGCC_NAME)
	@echo Packing $@
	@$(ARMAR) rcs $@ $^ $(OUTDIR)tmp/libc/*.o $(OUTDIR)tmp/libgcc/*.o
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/lib
	@cp $(OUTDIR)libandixC.a $(ANDIX_DEPLOY_DIR)/tz/lib
	@rm -rf $(OUTDIR)tmp
                
.PHONY: mkbuilddirs		
mkbuilddirs:
	@mkdir -p $(DIRS)
	
$(C_OBJS):$(OUTDIR)%.o:%.c
	$(arm-cc-command)
	
$(A_OBJS):$(OUTDIR)%.o:%.S
	$(arm-as-command)
	
.PHONY: clean
clean: 
	@echo Removing directory $(OUTDIR)
	@rm -rf $(OUTDIR)