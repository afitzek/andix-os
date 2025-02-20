TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/sample/
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

ANDIXRUNTIME = $(ANDIX_DEPLOY_DIR)/libandix.a

ASFLAGS = -g -mcpu=cortex-a8  -mfloat-abi=soft
CFLAGS = -I $(ANDIX_INC_COMMON) -I $(ARM_NEWLIB_INC_DIR) -I $(TZ_USER_TEE_INC) -I $(ARM_TROPICSSL_INC_DIR) -I $(TZ_USER_C_INC) -I $(TZ_USER_SAMPLE_INC) -g -mcpu=cortex-a8  -mfloat-abi=soft
LDFLAGS = -L$(ANDIX_DEPLOY_DIR)/tz/lib --nostdlib

all: mkbuilddirs $(OUTDIR)sample.bin

$(OUTDIR)sample.bin: $(OBJS)
	@echo Linking $@
	@$(ARMLD) $(LDFLAGS) -o $@ $^ -ltrustlet -landix -ltrustlet
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/trustlets
	@cp $(OUTDIR)sample.bin $(ANDIX_DEPLOY_DIR)/tz/trustlets/sample.bin
	
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
                                                                                
