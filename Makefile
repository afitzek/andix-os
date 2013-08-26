include script/config.mk
include script/vars.mk
include script/rules.mk

.PHONY: all
all: mkdirs tool_builds tz_builds linux_builds 
	@cd $(ANDIX_DEPLOY_DIR); chmod +x buildload.sh; ./buildload.sh
	@cat src/instruction.txt
	@tree -h deploy

.PHONY: clean
clean: rmdirs tool_clean tz_clean linux_clean

.PHONY: doc
doc: tz_kernel_doc tz_tee_runtime_doc

.PHONY: pdfdoc
pdfdoc: tz_kernel_pdfdoc tz_tee_runtime_pdfdoc

qemu: tz_kernel_qemu

####
# Tool Build targets
####

tool_builds: tool_packer_build tool_scripts_build

tool_clean: tool_packer_clean tool_scripts_clean

####

####
# Linux Build targets
####

linux_builds: linux_modules_build linux_serv_daemon_build linux_app_lib_build linux_app_daemon_build linux_app_build linux_enc_app_build linux_tz_test_app_build linux_rsa_app_build

linux_clean: linux_modules_clean linux_serv_daemon_clean linux_app_lib_clean linux_app_daemon_clean linux_app_clean linux_enc_app_clean linux_tz_test_app_clean linux_rsa_app_clean

####

####
# TZ Build targets
####

tz_builds: tz_newlib_c_build tz_tee_runtime_build tz_sample_build tz_manager_build tz_rsa_build tz_kernel_build 
	
tz_clean: tz_newlib_c_clean tz_tee_runtime_clean tz_sample_clean tz_manager_clean tz_rsa_clean tz_kernel_clean 
	
####	
	
####
# Andix TZ Kernel
####

TZ_KERNEL = ANDIX TZ KERNEL

tz_kernel_build:
	$(build-num)
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

tz_kernel_qemu:
	$(start-build-command) $(TZ_KERNEL)
	$(sep-command)
	@$(make-command) $(TZ_KERNEL_SRC) run_qemu
	$(sep-command)

####

####
# Andix TZ TEE Runtime
####

TZ_TEE_RUNTIME = ANDIX TZ TEE RUNTIME

tz_tee_runtime_build:
	$(build-num)
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

####
# Andix TZ Manager
####

TZ_TEE_MANAGER = ANDIX TZ MANAGER

tz_manager_build:
	$(build-num)
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
# Andix TZ Sample Trustlet
####

TZ_TEE_SAMPLE = ANDIX TZ SAMPLE

tz_sample_build:
	$(build-num)
	$(start-build-command) $(TZ_TEE_SAMPLE)
	$(sep-command)
	@$(make-command) $(TZ_USER_SAMPLE_SRC)
	$(sep-command)

tz_sample_clean:
	$(start-clean-command) $(TZ_TEE_SAMPLE)
	$(sep-command)
	@$(make-command) $(TZ_USER_SAMPLE_SRC) clean
	$(sep-command)

tz_sample_doc:
	$(start-build-doc-command) $(TZ_TEE_SAMPLE)
	$(sep-command)
	@$(make-command) $(TZ_USER_SAMPLE_SRC) doc
	$(sep-command)
	
tz_sample_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_TEE_SAMPLE)
	$(sep-command)
	@$(make-command) $(TZ_USER_SAMPLE_SRC) pdfdoc
	$(sep-command)

####

####
# Andix TZ RSA Trustlet
####

TZ_TEE_RSA = ANDIX TZ RSA

tz_rsa_build:
	$(build-num)
	$(start-build-command) $(TZ_TEE_RSA)
	$(sep-command)
	@$(make-command) $(TZ_USER_RSA_SRC)
	$(sep-command)

tz_rsa_clean:
	$(start-clean-command) $(TZ_TEE_RSA)
	$(sep-command)
	@$(make-command) $(TZ_USER_RSA_SRC) clean
	$(sep-command)

tz_rsa_doc:
	$(start-build-doc-command) $(TZ_TEE_RSA)
	$(sep-command)
	@$(make-command) $(TZ_USER_RSA_SRC) doc
	$(sep-command)
	
tz_RSA_pdfdoc:
	$(start-build-pdfdoc-command) $(TZ_TEE_RSA)
	$(sep-command)
	@$(make-command) $(TZ_USER__SRC) pdfdoc
	$(sep-command)

####

####
# Andix newlib c system interface library
####

TZ_NEWLIB_C = ANDIX TZ NEWLIB C

tz_newlib_c_build:
	$(build-num)
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
	$(build-num)
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
	$(build-num)
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
	$(build-num)
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
	$(build-num)
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

####
# Andix linux application daemon
####

LINUX_APP_DAEMON = ANDIX LINUX APPLICATION DAEMON

linux_app_daemon_build:
	$(build-num)
	$(start-build-command) $(LINUX_APP_DAEMON)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_DAEMON_SRC)
	$(sep-command)

linux_app_daemon_clean:
	$(start-clean-command) $(LINUX_APP_DAEMON)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_DAEMON_SRC) clean
	$(sep-command)

####

####
# Andix linux application
####

LINUX_TZ_TEST_APP = ANDIX LINUX TZ TEST APPLICATION

linux_tz_test_app_build: linux_app_lib_build
	$(build-num)
	$(start-build-command) $(LINUX_TZ_TEST_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_TZ_TEST_SRC)
	$(sep-command)

linux_tz_test_app_clean:
	$(start-clean-command) $(LINUX_TZ_TEST_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_TZ_TEST_SRC) clean
	$(sep-command)

####
# Andix linux application
####

LINUX_APP = ANDIX LINUX APPLICATION

linux_app_build: linux_app_lib_build
	$(build-num)
	$(start-build-command) $(LINUX_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_SRC)
	$(sep-command)

linux_app_clean:
	$(start-clean-command) $(LINUX_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_SRC) clean
	$(sep-command)
	
####
# Andix linux application
####

LINUX_ENC_APP = ANDIX LINUX ENCRPYTION APPLICATION

linux_enc_app_build: linux_app_lib_build
	$(build-num)
	$(start-build-command) $(LINUX_ENC_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_ENC_SRC)
	$(sep-command)

linux_enc_app_clean:
	$(start-clean-command) $(LINUX_ENC_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_ENC_SRC) clean
	$(sep-command)

LINUX_RSA_APP = ANDIX LINUX RSA APPLICATION

linux_rsa_app_build: linux_app_lib_build
	$(build-num)
	$(start-build-command) $(LINUX_RSA_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_RSA_SRC)
	$(sep-command)

linux_rsa_app_clean:
	$(start-clean-command) $(LINUX_RSA_APP)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_RSA_SRC) clean
	$(sep-command)

####

####
# Andix linux application library
####

LINUX_APP_LIB = ANDIX LINUX APPLICATION LIBRARY

linux_app_lib_build:
	$(build-num)
	$(start-build-command) $(LINUX_APP_LIB)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_LIB_SRC)
	$(sep-command)

linux_app_lib_clean:
	$(start-clean-command) $(LINUX_APP_LIB)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_LIB_SRC) clean
	$(sep-command)
	
linux_app_lib_doc:
	$(start-build-doc-command) $(LINUX_APP_LIB)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_LIB_SRC) doc
	$(sep-command)
	
linux_app_lib_pdfdoc:
	$(start-build-pdfdoc-command) $(LINUX_APP_LIB)
	$(sep-command)
	@$(make-command) $(NORM_US_APP_LIB_SRC) pdfdoc
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

	
