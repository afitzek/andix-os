TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/manager/
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

ASFLAGS = -g -mcpu=cortex-a8
CFLAGS = -I $(ANDIX_INC_COMMON) -I $(ARM_NEWLIB_INC_DIR) -I $(TZ_USER_TEE_INC) -I $(ARM_POLARSSL_INC_DIR) -I $(TZ_USER_C_INC) -I $(TZ_USER_MANAGER_INC) -g -mcpu=cortex-a8
LDFLAGS = -static -L$(ANDIX_DEPLOY_DIR)

all: mkbuilddirs $(OUTDIR)manager.bin

$(OUTDIR)manager.bin: $(OBJS)
	@echo Linking $@
	@$(ARMLD) $(LDFLAGS) -o $@ $^ -landix 
	@cp $(OUTDIR)manager.bin $(ANDIX_DEPLOY_DIR)/manager.bin
	
.PHONY: mkbuilddirs		
mkbuilddirs:
	@mkdir -p $(DIRS)
	
$(C_OBJS):$(OUTDIR)%.o:%.c
	$(arm-cc-command)
	
$(A_OBJS):$(OUTDIR)%.o:%.S
	$(arm-as-command)
                                                                
.PHONY: clean
clean:
	@echo 
	@echo Removing directory $(OUTDIR)
	rm -rf $(OUTDIR)
                                                                                