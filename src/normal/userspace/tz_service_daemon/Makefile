TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = norm/service_daemon/
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

ASFLAGS = -g -mcpu=cortex-a8
CFLAGS = -g -I$(NORM_US_SERVICE_DAEMON_INC) -I$(ANDIX_INC_COMMON) -mcpu=cortex-a8 
LDFLAGS = -L$(ARM_TOOLCHAIN_LIB_DIR) -lgcc

all: mkbuilddirs $(OUTDIR)tzsd.bin

$(OUTDIR)tzsd.bin: $(OBJS)
	@$(ARMNORMCC) $(LDFLAGS) -o $@ $^
	@mkdir -p $(ANDIX_DEPLOY_DIR)/norm/daemons
	@cp $(OUTDIR)tzsd.bin $(ANDIX_DEPLOY_DIR)/norm/daemons
                
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
	@rm -rf $(OUTDIR)