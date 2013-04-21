TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = norm/app_lib/
OUTDIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)
D_DIR = $(ANDIX_BUILD_DIR)/$(PROGRAM)doc
OUTDIR_DOC = $(subst /,\/,$(D_DIR))
OUTDIR_DOC_API = $(subst /,\/,$(D_DIR)api)
INC_DIR_DOC = $(subst /,\/,$(ANDIX_INC_COMMON)/tee_client_api.h)

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
CFLAGS = -g -fPIC -I$(NORM_US_APP_LIB_INC) -I$(ANDIX_INC_COMMON) -mcpu=cortex-a8 
LDFLAGS = -L$(ARM_TOOLCHAIN_LIB_DIR) -lgcc

all: mkbuilddirs $(OUTDIR)libandixtee.so

$(OUTDIR)libandixtee.so: $(OBJS)
	@$(ARMNORMCC) -shared $(LDFLAGS) -o $@ $^
	@mkdir -p $(ANDIX_DEPLOY_DIR)/norm/lib
	@cp $(OUTDIR)libandixtee.so $(ANDIX_DEPLOY_DIR)/norm/lib
              
.PHONY: doc
doc: apidoc
	@echo
	@mkdir -p $(D_DIR) $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	@cp doc/andix.doxyfile doc/andix.doxfile.tmp
	@sed -i -e "s/{{VERSION}}/$(VERSION)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{GITHASH}}/$(GITHASH)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{LOGO}}/$(E_DOC_LOGO)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{OUTDIR}}/$(OUTDIR_DOC)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{INC_DIR}}/$(INC_DIR_DOC)/" doc/andix.doxfile.tmp
	@$(DOC) doc/andix.doxfile.tmp
	@rm -rf doc/andix.doxfile.tmp
	@cp -R $(OUTDIR_DOC)/html $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	
.PHONY: apidoc
apidoc:
	@echo
	@mkdir -p $(D_DIR) $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)api/
	@cp doc/andix_api.doxyfile doc/andix_api.doxfile.tmp
	@sed -i -e "s/{{VERSION}}/$(VERSION)/" doc/andix_api.doxfile.tmp
	@sed -i -e "s/{{GITHASH}}/$(GITHASH)/" doc/andix_api.doxfile.tmp
	@sed -i -e "s/{{LOGO}}/$(E_DOC_LOGO)/" doc/andix_api.doxfile.tmp
	@sed -i -e "s/{{OUTDIR}}/$(OUTDIR_DOC_API)/" doc/andix_api.doxfile.tmp
	@sed -i -e "s/{{INC_DIR}}/$(INC_DIR_DOC)/" doc/andix_api.doxfile.tmp
	@$(DOC) doc/andix_api.doxfile.tmp
	@rm -rf doc/andix_api.doxfile.tmp
	@cp -R $(OUTDIR_DOC_API)/html $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)api/

pdfdoc: doc
	@make -C $(OUTDIR_DOC)/latex/
	@cp $(OUTDIR_DOC)/latex/refman.pdf $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	@make -C $(OUTDIR_DOC_API)/latex/
	@cp $(OUTDIR_DOC_API)/latex/refman.pdf $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)api/
                
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