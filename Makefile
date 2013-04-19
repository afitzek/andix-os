include script/config.mk
include script/vars.mk
include script/rules.mk

SRCS = $(shell find -name '*.[cS]')
OBJS = $(addprefix $(ANDIX_BUILD_DIR),$(addsuffix .o,$(basename $(SRCS))))

.PHONY: all
all:
	@echo Building TZ KERNEL
	@$(make-command) $(TZ_KERNEL_SRC)

.PHONY: doc
doc:
	@echo Building Documentation TZ KERNEL
	@$(make-command) $(TZ_KERNEL_SRC) doc
	
.PHONY: pdfdoc
pdfdoc:
	@echo Building PDF Documentation TZ KERNEL
	@$(make-command) $(TZ_KERNEL_SRC) pdfdoc

.PHONY: clean
clean:
	@echo Cleaning TZ KERNEL
	@$(make-command) $(TZ_KERNEL_SRC) clean