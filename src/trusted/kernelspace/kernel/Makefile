TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

###############################################################################

####
# Configure sources
####

PROGRAM = tz/andix/kernel/
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

LIBGCC_FILE = $(ARM_TOOLCHAIN_LIB_DIR)libgcc.a

OPT_FLAGS = -Wall -std=gnu99 -O1 -I$(TZ_KERNEL_INC) -I$(ANDIX_INC_COMMON) -mapcs-frame -mapcs-stack-check

ASFLAGS = $(OPT_FLAGS) -g -mcpu=cortex-a8
CFLAGS = $(OPT_FLAGS) -DBUILD_DATE=\"$(BUILD_DATE)\" -DBUILD_NUM=\"$(BUILD_NUM)\" -DVERSION=\"$(VERSION)\" -DUSER=\"$(USER)\" -DINFO -DDEBUG -DGITHASH=\"$(GITHASH)\" -DIMX53_LOCO -DAKERNEL -g -mcpu=cortex-a8 -nostdlib -nostdinc
LDFLAGS = -T link.ld -nostdlib $(BUILD_NUMBER_LDFLAGS)
MKIMAGEARGS = -A arm -O linux -T kernel -a 0xB0000000 -e 0xB0008000 -n "ANDIX" -C none -d $(OUTDIR)andix.elfagain

.PHONY: all

all:	getpayloads mkbuilddirs $(OUTDIR)uImage.bin

$(OUTDIR)uImage.bin: $(OUTDIR)andix.elfagain $(OUTDIR)andix.symagain
	@echo "GITHASH: $(GITHASH)"
	@$(MKIMAGE) $(MKIMAGEARGS) $(OUTDIR)uImage.bin
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/kernel
	@cp $(OUTDIR)uImage.bin $(ANDIX_DEPLOY_DIR)/tz/kernel/andix.img

$(OUTDIR)%.symagain: $(OUTDIR)%.elfagain
	$(arm-sym-command)

$(OUTDIR)%.elfagain:  $(OBJS) $(OUTDIR)symbol_table.o
	$(arm-ld-command) $(LIBGCC_FILE)

$(OUTDIR)symbol_table.o : $(OUTDIR)andix.mys
	@echo 
	@echo Converting Symbol table: $@
	@cp $(<) $(*).tmp
	@$(ARMOBJCOPY) -I binary -O elf32-littlearm -B arm \
	--rename-section .data=.symbol_table,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$*_tmp_start=symbol_table \
	--redefine-sym _binary_$*_tmp_end=symbol_table_end \
	--redefine-sym _binary_$*_tmp_size=symbol_table_size_sym \
	$(*).tmp $(@)
	@rm $(*).tmp

%.mys: %.sym
	$(mys-command)

%.sym: %.elf
	$(arm-sym-command)

$(OUTDIR)andix.elf:  $(OBJS) $(BUILD_NUMBER_FILE)
	@$(ARMLD) $(LDFLAGS) -o $@  $^ $(LIBGCC_FILE)


.PHONY: doc
doc:
	@echo
	@mkdir -p $(D_DIR) $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	@cp doc/andix.doxyfile doc/andix.doxfile.tmp
	@sed -i -e "s/{{VERSION}}/$(VERSION)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{GITHASH}}/$(GITHASH)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{LOGO}}/$(E_DOC_LOGO)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{OUTDIR}}/$(OUTDIR_DOC)/" doc/andix.doxfile.tmp
	@$(DOC) doc/andix.doxfile.tmp
	@rm -rf doc/andix.doxfile.tmp
	@cp -R $(OUTDIR_DOC)/html $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)

pdfdoc: doc
	@make -C $(OUTDIR_DOC)/latex/
	@cp $(OUTDIR_DOC)/latex/refman.pdf $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)

.PHONY: getpayloads
getpayloads:
	cp $(ANDIX_DEPLOY_DIR)/tz/trustlets/manager.bin $(realpath $(TOP))/payload/manager.bin
	touch $(realpath $(TOP))/payload/userpayload.S
	touch $(realpath $(TOP))/kernel/main.c

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
	@rm -rf $(OUTDIR) $(ANDIX_DEPLOY_DIR)/tz/kernel/andix.img
