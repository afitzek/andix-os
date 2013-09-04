TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = norm/app_rsa_engine_test/
OUTDIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)

C_SRCS = $(shell find -name '*.[c]' | sed 's|./||')
C_OBJS = $(addprefix $(OUTDIR), $(addsuffix .o,$(basename $(C_SRCS))))
C_DIRS = $(sort $(dir $(C_OBJS)))

A_SRCS = $(shell find -name '*.[S]' | sed 's|./||')
A_OBJS = $(addprefix $(OUTDIR), $(addsuffix .o,$(basename $(A_SRCS))))
A_DIRS = $(sort $(dir $(A_OBJS)))

OBJS = $(C_OBJS) $(A_OBJS)

DIRS = $(sort $(C_DIRS) $(A_DIRS))

PROGRAM_NAME = rsa_engine.bin

###############################################################################
ROOTFS_LIBDIR = /home/flo/workspace/masterprojekt/linaro-userland/binary/lib/arm-linux-gnueabihf
ASFLAGS = -g -mcpu=cortex-a8
CFLAGS = -g -I$(NORM_US_APP_ENC_INC) -I$(ANDIX_INC_COMMON) -mcpu=cortex-a8 
LDFLAGS = -Wl,-rpath-link=$(ROOTFS_LIBDIR)
LDLIBS = $(ROOTFS_LIBDIR)/libcrypto.so.1.0.0

all: mkbuilddirs $(OUTDIR)$(PROGRAM_NAME)

$(OUTDIR)$(PROGRAM_NAME): $(OBJS)
	@$(ARMNORMCC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@mkdir -p $(ANDIX_DEPLOY_DIR)/norm/app
	@cp $(OUTDIR)$(PROGRAM_NAME) $(ANDIX_DEPLOY_DIR)/norm/app
                
.PHONY: mkbuilddirs		
mkbuilddirs:
	@mkdir -p $(DIRS)
	
$(C_OBJS):$(OUTDIR)%.o:%.c
	$(arm-norm-cc-command)
	
$(A_OBJS):$(OUTDIR)%.o:%.S
	$(arm-norm-as-command)
	
.PHONY: clean
clean:
	@echo Removing directory $(OUTDIR)
	@rm -rf $(OUTDIR) $(ANDIX_DEPLOY_DIR)/norm/app/$(PROGRAM_NAME)