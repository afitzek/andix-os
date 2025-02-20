TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/libtrustlet/
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

ASFLAGS = -mcpu=cortex-a8 -mfloat-abi=soft
CFLAGS = -I $(ARM_NEWLIB_INC_DIR) -I $(ARM_TROPICSSL_INC_DIR) -I $(TZ_USER_C_INC) -mcpu=cortex-a8 -nostdlib -mfloat-abi=soft
LDFLAGS = -nostdlib

all: mkbuilddirs $(OUTDIR)libtrustlet.a

$(OUTDIR)libtrustlet.a: $(OBJS)
	@$(ARMAR) rcs $@ $^ 
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/lib
	@cp $@ $(ANDIX_DEPLOY_DIR)/tz/lib
                
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