include script/config.mk
include script/vars.mk
include script/rules.mk

.PHONY: all
all: mkdirs tz_kernel_build tz_newlib_c_build tz_tee_runtime_build

.PHONY: clean
clean: rmdirs tz_kernel_clean tz_newlib_c_clean tz_tee_runtime_clean

.PHONY: doc
doc: tz_kernel_doc tz_tee_runtime_doc

.PHONY: pdfdoc
pdfdoc: tz_kernel_pdfdoc tz_tee_runtime_pdfdoc
	
####
# Anidx TZ Kernel
####

TZ_KERNEL = ANDIX TZ KERNEL

tz_kernel_build:
	$(start-build-command) $(TZ_KERNEL)
	@$(make-command) $(TZ_KERNEL_SRC)
	$(sep-command)

tz_kernel_clean:
	$(start-clean-command) $(TZ_KERNEL)
	@$(make-command) $(TZ_KERNEL_SRC) clean
	$(sep-command)

tz_kernel_doc:
	$(start-build-doc-command) $(TZ_KERNEL)
	@$(make-command) $(TZ_KERNEL_SRC) doc
	$(sep-command)
	
tz_kernel_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_KERNEL)
	@$(make-command) $(TZ_KERNEL_SRC) pdfdoc
	$(sep-command)

####

####
# Anidx TZ TEE Runtime
####

TZ_TEE_RUNTIME = ANDIX TZ TEE RUNTIME

tz_tee_runtime_build:
	$(start-build-command) $(TZ_TEE_RUNTIME)
	@$(make-command) $(TZ_USER_TEE_SRC)
	$(sep-command)

tz_tee_runtime_clean:
	$(start-clean-command) $(TZ_TEE_RUNTIME)
	@$(make-command) $(TZ_USER_TEE_SRC) clean
	$(sep-command)

tz_tee_runtime_doc:
	$(start-build-doc-command) $(TZ_TEE_RUNTIME)
	@$(make-command) $(TZ_USER_TEE_SRC) doc
	$(sep-command)
	
tz_tee_runtime_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_TEE_RUNTIME)
	@$(make-command) $(TZ_USER_TEE_SRC) pdfdoc
	$(sep-command)

####

####
# Anidx newlib c system interface library
####

TZ_NEWLIB_C = ANDIX TZ NEWLIB C

tz_newlib_c_build:
	$(start-build-command) $(TZ_NEWLIB_C)
	@$(make-command) $(TZ_USER_C_SRC)
	$(sep-command)

tz_newlib_c_clean:
	$(start-clean-command) $(TZ_NEWLIB_C)
	@$(make-command) $(TZ_USER_C_SRC) clean
	$(sep-command)

####

.PHONY: mkdirs
mkdirs: mkdeploydir mkbuilddir

.PHONY: rmdirs
rmdirs: cleandeploydir cleanbuilddir

mkdeploydir:
	@mkdir -p $(ANDIX_DEPLOY_DIR)

cleandeploydir:
	@rm -rf $(ANDIX_DEPLOY_DIR)

mkbuilddir:
	@mkdir -p $(ANDIX_BUILD_DIR)

cleanbuilddir:
	@rm -rf $(ANDIX_BUILD_DIR)

	