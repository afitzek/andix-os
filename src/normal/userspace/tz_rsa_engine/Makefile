TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = norm/opennssl_tz_rsa_engine/
LIBNAME = libtz_rsa_engine.so
OUTDIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)

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
CFLAGS = -g -fPIC -I$(NORM_US_APP_LIB_INC) -I$(ANDIX_INC_COMMON) -mcpu=cortex-a8 
LDFLAGS = -shared -L$(ANDIX_DEPLOY_DIR)/norm/lib
LDLIBS = -landixtee

all: mkbuilddirs $(OUTDIR)$(LIBNAME)

$(OUTDIR)$(LIBNAME): $(OBJS)
	$(ARMNORMCC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@mkdir -p $(ANDIX_DEPLOY_DIR)/norm/lib
	@cp $(OUTDIR)$(LIBNAME) $(ANDIX_DEPLOY_DIR)/norm/lib

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