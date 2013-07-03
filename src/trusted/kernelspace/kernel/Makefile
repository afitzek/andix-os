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

INCDIR_DOC = $(subst /,\/,$(TZ_KERNEL_INC))

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

#LIBGCC_FILE = $(ARM_TOOLCHAIN_LIB_DIR)libgcc.a
LIBGCC_FILE = 
OPT_FLAGS = -Wall -std=gnu99 -O1 -I$(TZ_KERNEL_INC) -I$(ANDIX_INC_COMMON) -mapcs-frame -mapcs-stack-check

ASFLAGS = -g -mcpu=cortex-a8
CFLAGS = $(OPT_FLAGS) -DBUILD_DATE=\"$(BUILD_DATE)\" -DSHOW_MEM_LAYOUT -DBUILD_NUM=\"$(BUILD_NUM)\" -DVERSION=\"$(VERSION)\" -DUSER=\"$(USER)\" -DINFO -DDEBUG -DGITHASH=\"$(GITHASH)\" -DIMX53_LOCO -DAKERNEL -g -mcpu=cortex-a8 -nostdlib -nostdinc
LDFLAGS = -nostdlib $(BUILD_NUMBER_LDFLAGS)
MKIMAGEARGS = -A arm -O linux -T kernel -a 0x70000000 -e 0x70008000 -n "ANDIX" -C none -d $(OUTDIR)andix.elfagain
LDQEMUFLAGS = -DQEMU=1
LDQEMUFALGS_FILE = -T link.qld
LDREALFALGS_FILE = -T link.rld
QEMUARGS = -M vexpress-a9 -kernel $(ANDIX_DEPLOY_DIR)/tz/kernel/andix_qemu.bin_qemu -nographic -s -S -m 1024

.PHONY: all

all:	getpayloads mkbuilddirs $(OUTDIR)uImage.bin

run_qemu: $(OUTDIR)andix_qemu.bin_qemu
	$(TZ_QEMU) $(QEMUARGS)

$(OUTDIR)uImage.bin: $(OUTDIR)andix_qemu.bin_qemu $(OUTDIR)andix.elfagain $(OUTDIR)andix.symagain
	@echo "GITHASH: $(GITHASH)"
	@$(MKIMAGE) $(MKIMAGEARGS) $(OUTDIR)uImage.bin
	@mkdir -p $(ANDIX_DEPLOY_DIR)/tz/kernel
	@cp $(OUTDIR)uImage.bin $(ANDIX_DEPLOY_DIR)/tz/kernel/andix.img
	@cp $(OUTDIR)andix_qemu.bin_qemu $(ANDIX_DEPLOY_DIR)/tz/kernel/andix_qemu.bin_qemu

$(OUTDIR)%.symagain: $(OUTDIR)%.elfagain
	$(arm-sym-command)

$(OUTDIR)%.bin: $(OUTDIR)%.elfagain
	$(arm-objcpy-command)

$(OUTDIR)%.bin_qemu: $(OUTDIR)%.elfagain_qemu
	$(arm-objcpy-command)

$(OUTDIR)%.elfagain_qemu:  $(OBJS) $(OUTDIR)%.st
	@$(ARMLD) $(LDQEMUFALGS_FILE) -o $@  $^ $(LIBGCC_FILE)
	
$(OUTDIR)%.elfagain:  $(OBJS) $(OUTDIR)%.st
	@$(ARMLD) $(LDREALFALGS_FILE) -o $@  $^ $(LIBGCC_FILE)

$(OUTDIR)%.st : $(OUTDIR)%.mys
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

$(OUTDIR)andix_qemu.elf:  $(OBJS) $(BUILD_NUMBER_FILE)
	@$(ARMCC) -E -P -x c  $(LDQEMUFLAGS) -o link.qld link.ld
	@$(ARMLD) $(LDQEMUFALGS_FILE) -o $@  $^ $(LIBGCC_FILE)

$(OUTDIR)andix.elf:  $(OBJS) $(BUILD_NUMBER_FILE)
	@$(ARMCC) -E -P -x c  -o link.rld link.ld
	@$(ARMLD) $(LDREALFALGS_FILE) -o $@  $^ $(LIBGCC_FILE)


.PHONY: doc
doc:
	@echo
	@mkdir -p $(D_DIR) $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)
	@cp doc/andix.doxyfile doc/andix.doxfile.tmp
	@sed -i -e "s/{{VERSION}}/$(VERSION)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{GITHASH}}/$(GITHASH)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{LOGO}}/$(E_DOC_LOGO)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{OUTDIR}}/$(OUTDIR_DOC)/" doc/andix.doxfile.tmp
	@sed -i -e "s/{{INC_PATH}}/$(INCDIR_DOC)/" doc/andix.doxfile.tmp
	@$(DOC) doc/andix.doxfile.tmp
	@rm -rf doc/andix.doxfile.tmp
	@cp -R $(OUTDIR_DOC)/html $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)

pdfdoc: doc
	@make -C $(OUTDIR_DOC)/latex/
	@cp $(OUTDIR_DOC)/latex/refman.pdf $(ANDIX_DOC_GEN_DIR)/$(PROGRAM)

.PHONY: getpayloads
getpayloads:
	cp $(ANDIX_DEPLOY_DIR)/tz/trustlets/manager.bin $(realpath $(TOP))/payload/manager.bin
	cp $(ANDIX_DEPLOY_DIR)/tz/trustlets/sample.bin $(realpath $(TOP))/payload/sample.bin
	touch $(realpath $(TOP))/payload/userpayload.S
	touch $(realpath $(TOP))/payload/samplepayload.S
	touch $(realpath $(TOP))/payload/payload.S
	touch $(realpath $(TOP))/payload/android_payload.S
	touch $(realpath $(TOP))/payload/android_payload_rd.S
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
