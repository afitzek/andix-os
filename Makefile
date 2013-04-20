include script/config.mk
include script/vars.mk
include script/rules.mk

.PHONY: all
all: mkdirs tool_builds tz_builds linux_builds
	@cat src/instruction.txt
	@tree -h deploy

.PHONY: clean
clean: rmdirs tool_clean tz_clean linux_clean

.PHONY: doc
doc: tz_kernel_doc tz_tee_runtime_doc

.PHONY: pdfdoc
pdfdoc: tz_kernel_pdfdoc tz_tee_runtime_pdfdoc

####
# Tool Build targets
####

tool_builds: tool_packer_build tool_scripts_build

tool_clean: tool_packer_clean tool_scripts_clean

####

####
# Linux Build targets
####

linux_builds: linux_modules_build linux_serv_daemon_build

linux_clean: linux_modules_clean linux_serv_daemon_clean

####

####
# TZ Build targets
####

tz_builds: tz_newlib_c_build tz_tee_runtime_build tz_manager_build tz_kernel_build
	
tz_clean: tz_newlib_c_clean tz_tee_runtime_clean tz_manager_clean tz_kernel_clean	
	
####	
	
####
# Andix TZ Kernel
####

TZ_KERNEL = ANDIX TZ KERNEL

tz_kernel_build:
	$(start-build-command) $(TZ_KERNEL)
	$(sep-command)
	@$(make-command) $(TZ_KERNEL_SRC)
	$(sep-command)

tz_kernel_clean:
	$(start-clean-command) $(TZ_KERNEL)
	$(sep-command)
	@$(make-command) $(TZ_KERNEL_SRC) clean
	$(sep-command)

tz_kernel_doc:
	$(start-build-doc-command) $(TZ_KERNEL)
	$(sep-command)
	@$(make-command) $(TZ_KERNEL_SRC) doc
	$(sep-command)
	
tz_kernel_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_KERNEL)
	$(sep-command)
	@$(make-command) $(TZ_KERNEL_SRC) pdfdoc
	$(sep-command)

####

####
# Andix TZ TEE Runtime
####

TZ_TEE_RUNTIME = ANDIX TZ TEE RUNTIME

tz_tee_runtime_build:
	$(start-build-command) $(TZ_TEE_RUNTIME)
	$(sep-command)
	@$(make-command) $(TZ_USER_TEE_SRC)
	$(sep-command)

tz_tee_runtime_clean:
	$(start-clean-command) $(TZ_TEE_RUNTIME)
	$(sep-command)
	@$(make-command) $(TZ_USER_TEE_SRC) clean
	$(sep-command)

tz_tee_runtime_doc:
	$(start-build-doc-command) $(TZ_TEE_RUNTIME)
	$(sep-command)
	@$(make-command) $(TZ_USER_TEE_SRC) doc
	$(sep-command)
	
tz_tee_runtime_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_TEE_RUNTIME)
	$(sep-command)
	@$(make-command) $(TZ_USER_TEE_SRC) pdfdoc
	$(sep-command)

####

####
# Andix TZ Manager
####

TZ_TEE_MANAGER = ANDIX TZ MANAGER

tz_manager_build:
	$(start-build-command) $(TZ_TEE_MANAGER)
	$(sep-command)
	@$(make-command) $(TZ_USER_MANAGER_SRC)
	$(sep-command)

tz_manager_clean:
	$(start-clean-command) $(TZ_TEE_MANAGER)
	$(sep-command)
	@$(make-command) $(TZ_USER_MANAGER_SRC) clean
	$(sep-command)

tz_manager_doc:
	$(start-build-doc-command) $(TZ_TEE_MANAGER)
	$(sep-command)
	@$(make-command) $(TZ_USER_MANAGER_SRC) doc
	$(sep-command)
	
tz_manager_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_TEE_MANAGER)
	$(sep-command)
	@$(make-command) $(TZ_USER_MANAGER_SRC) pdfdoc
	$(sep-command)

####

####
# Andix newlib c system interface library
####

TZ_NEWLIB_C = ANDIX TZ NEWLIB C

tz_newlib_c_build:
	$(start-build-command) $(TZ_NEWLIB_C)
	$(sep-command)
	@$(make-command) $(TZ_USER_C_SRC)
	$(sep-command)

tz_newlib_c_clean:
	$(start-clean-command) $(TZ_NEWLIB_C)
	$(sep-command)
	@$(make-command) $(TZ_USER_C_SRC) clean
	$(sep-command)

####

####
# Andix packer tool
####

PACKER_TOOLS = ANDIX PACKER TOOL

tool_packer_build:
	$(start-build-command) $(PACKER_TOOLS)
	$(sep-command)
	@$(make-command) $(TOOL_PACKER_SRC)
	$(sep-command)

tool_packer_clean:
	$(start-clean-command) $(PACKER_TOOLS)
	$(sep-command)
	@$(make-command) $(TOOL_PACKER_SRC) clean
	$(sep-command)

####

####
# Andix tools scripts
####

TOOL_SCRIPTS = ANDIX LINUX SCRIPTS

tool_scripts_build:
	$(start-build-command) $(TOOL_SCRIPTS)
	$(sep-command)
	@$(make-command) $(TOOL_SCRIPTS_SRC)
	$(sep-command)

tool_scripts_clean:
	$(start-clean-command) $(TOOL_SCRIPTS)
	$(sep-command)
	@$(make-command) $(TOOL_SCRIPTS_SRC) clean
	$(sep-command)

####

####
# Andix linux service modules
####

LINUX_MOD = ANDIX LINUX MODULES

linux_modules_build:
	$(start-build-command) $(LINUX_MOD)
	$(sep-command)
	@$(make-command) $(NORM_K_SRC)
	$(sep-command)

linux_modules_clean:
	$(start-clean-command) $(LINUX_MOD)
	$(sep-command)
	@$(make-command) $(NORM_K_SRC) clean
	$(sep-command)

####

####
# Andix linux service daemon
####

LINUX_SERV_DAEMON = ANDIX LINUX SERVICE DAEMON

linux_serv_daemon_build:
	$(start-build-command) $(LINUX_SERV_DAEMON)
	$(sep-command)
	@$(make-command) $(NORM_US_SERVICE_DAEMON_SRC)
	$(sep-command)

linux_serv_daemon_clean:
	$(start-clean-command) $(LINUX_SERV_DAEMON)
	$(sep-command)
	@$(make-command) $(NORM_US_SERVICE_DAEMON_SRC) clean
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

	