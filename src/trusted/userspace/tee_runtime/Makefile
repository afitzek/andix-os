TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/tee_runtime/
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

ANDIXC_NAME = libandixC.a
POLAR_NAME = libpolarssl.a
ANDIXC_FILE = $(ANDIX_DEPLOY_DIR)/tz/lib/$(ANDIXC_NAME)
POLAR_FILE = $(ARM_POLARSSL_LIB_DIR)/$(POLAR_NAME)

ASFLAGS = -g -mcpu=cortex-a8
CFLAGS = -I $(TZ_USER_C_INC) -I $(TZ_USER_TEE_INC) -I $(ARM_NEWLIB_INC_DIR) -I $(ARM_POLARSSL_INC_DIR) -g -mcpu=cortex-a8
LDFLAGS = -nostdlib

all: mkbuilddirs $(OUTDIR)libandix.a

$(OUTDIR)libandix.a: $(OBJS)
	@mkdir -p $(OUTDIR)tmp/mylib
	@mkdir -p $(OUTDIR)tmp/polar
	@cp $(ANDIXC_FILE) $(OUTDIR)tmp/mylib/
	@cp $(POLAR_FILE) $(OUTDIR)tmp/polar/
	@echo Extracting AndixC: $(ANDIXC_NAME)
	@cd $(OUTDIR)tmp/mylib/; $(ARMAR) xo $(ANDIXC_NAME)
	@echo Extrating POLAR SSL: $(POLAR_NAME)
	@cd $(OUTDIR)tmp/polar/; $(ARMAR) xo $(POLAR_NAME)
	@echo Building Runtime archiv
	@$(ARMAR) rcs $@ $^ $(OUTDIR)tmp/mylib/*.o $(OUTDIR)tmp/polar/*.o
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/lib
	@cp $(OUTDIR)libandix.a $(ANDIX_DEPLOY_DIR)/tz/lib
	@rm -rf tmp
	
.PHONY: mkbuilddirs		
mkbuilddirs:
	@mkdir -p $(DIRS)
	
$(C_OBJS):$(OUTDIR)%.o:%.c
	$(arm-cc-command)
	
$(A_OBJS):$(OUTDIR)%.o:%.S
	$(arm-as-command)
                 
.PHONY: doc
doc:
	@echo
	@mkdir -p $(D_DIR) $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	@cp doc/andix.doxyfile doc/andix.doxfile.tmp
	@sed -i -e "s/{{VERSION}}/$(VERSION)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{GITHASH}}/$(GITHASH)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{OUTDIR}}/$(OUTDIR_DOC)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{LOGO}}/$(E_DOC_LOGO)/" doc/andix.doxfile.tmp
	@$(DOC) doc/andix.doxfile.tmp
	@rm -rf doc/andix.doxfile.tmp
	@cp -R $(OUTDIR_DOC)/html $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
               
pdfdoc: doc
	@make -C $(OUTDIR_DOC)/latex/
	@cp $(OUTDIR_DOC)/latex/refman.pdf $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	                                                   
.PHONY: clean
clean: 
	@echo Removing directory $(OUTDIR)
	@rm -rf $(OUTDIR)
                  
                                                                               