
define make-command
@$(MAKE) -I $(MAKE_INC) -C
endef

define ld-command
@echo Linking: $@
@$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
endef

define arm-ld-command
@echo [ARM]Linking: $@
@$(ARMLD) $(LDFLAGS) -o $@ $^ $(LIBS)
endef

define arm-cc-command
@echo [ARM]Building: $@
@$(ARMCC) $(CFLAGS) -c $< -o $@
endef

define cc-command
@echo Building: $@
@$(CC) $(CFLAGS) -c $< -o $@
endef

define as-command
@echo Building: $@
@$(AS) $(ASFLAGS) -c $< -o $@
endef

define arm-as-command
@echo [ARM]Building: $@
@$(ARMAS) $(ASFLAGS) -c $< -o $@
endef

define arm-sym-command
@echo 
@echo [ARM]Generating symboltable: $@
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