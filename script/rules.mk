
define sep-command
@echo =====================================================================
endef

define start-command
@echo
$(sep-command)
endef

define build-msg
@echo Building
endef

define clean-msg
@echo Cleaning
endef

define build-doc-msg
@echo Building Documentation
endef

define build-pdfdoc-msg
@echo Building PDF Documentation
endef


define start-build-command
$(start-command)
$(build-msg)
endef

define start-build-doc-command
$(start-command)
$(build-doc-msg)
endef

define start-build-pdfdoc-command
$(start-command)
$(build-pdfdoc-msg)
endef

define start-clean-command
$(start-command)
$(clean-msg)
endef

define make-command
@$(MAKE) -I $(MAKE_INC) -C
endef

define ld-command
@echo Linking: $@
@$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
endef

define cc-command
@echo Building: $@
@$(CC) $(CFLAGS) -c $< -o $@
endef

define as-command
@echo Building: $@
@$(AS) $(ASFLAGS) -c $< -o $@
endef

define arm-ld-command
@echo [ARM-TZ] Linking: $@
@$(ARMLD) $(LDFLAGS) -o $@ $^ $(LIBS)
endef

define arm-objcpy-command
@echo [ARM-TZ] Make raw image: $@
@$(ARMOBJCOPY) -O binary $^ $@
endef

define arm-cc-command
@echo [ARM-TZ] Building: $@
@$(ARMCC) $(CFLAGS) -c $< -S -o $<.OS
@$(ARMRAS) $(CFLAGS) -c $<.OS -o $@
endef

define arm-as-command
@echo [ARM-TZ] Building: $@
@$(ARMRAS) $(ASFLAGS) -c $< -o $@
endef

define arm-norm-ld-command
@echo [ARM-NORM] Linking: $@
@$(ARMNORMLD) $(LDFLAGS) -o $@ $^ $(LIBS)
endef

define arm-norm-cc-command
@echo [ARM-NORM] Building: $@
@$(ARMNORMCC) $(CFLAGS) -c $< -o $@
endef

define arm-norm-as-command
@echo [ARM-NORM] Building: $@
@$(ARMNORMAS) $(ASFLAGS) -c $< -o $@
endef

define arm-sym-command
@echo 
@echo [ARM] Generating symboltable: $@
@$(ARMNM) -n $< | grep " T " > $@
endef

define sym-command
@echo 
@echo Generating symboltable: $@
@$(NM) -n $< | grep " T " > $@
endef

define mys-command
@echo
@echo Rewriting symbol table: $@
@$(PERL) -p -e 's/\x0a/\x00/g' $^ > $@
endef

define build-num
@echo Incrementing build number
@if ! test -f $(PROG_BUILD_NUM); then echo 0 > $(PROG_BUILD_NUM); fi
@echo $$(($$(cat $(PROG_BUILD_NUM)) + 1)) > $(PROG_BUILD_NUM)
endef