#
# DO NOT EDIT THIS FILE USE config.mk
#

MAJOR_VERSION := 0
MINOR_VERSION := 1

USER := $(shell whoami)
GITHASH := $(shell git log --pretty=format:%H -1)

####
# Andix base directories
####

ANDIX_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))../)

ANDIX_SRC_DIR := $(ANDIX_DIR)/src
ANDIX_INC_DIR := $(ANDIX_DIR)/include
ANDIX_BUILD_DIR := $(ANDIX_DIR)/build
ANDIX_DEPLOY_DIR := $(ANDIX_DIR)/deploy
ANDIX_PREBUILD_DIR := $(ANDIX_DIR)/prebuild
ANDIX_DOC_DIR := $(ANDIX_DIR)/doc
ANDIX_DOC_GEN_DIR := $(ANDIX_DOC_DIR)/generated
MAKE_INC := $(ANDIX_DIR)/script

PROG_BUILD_NUM:=$(MAKE_INC)/build-number.txt
BUILD_DATE = $(shell date +'%Y.%m.%d_%H:%M:%S')
BUILD_NUM = $(shell cat $(PROG_BUILD_NUM))

VERSION := $(MAJOR_VERSION).$(MINOR_VERSION).$(BUILD_NUM)

####
# FOLDER CONSTANTS
####

KS := kernelspace
US := userspace

TZ := trusted
NORM := normal

DIR_SEP := /

####
# Abstract source directories
####

TZ_SRC := $(ANDIX_SRC_DIR)$(DIR_SEP)$(TZ)
TZ_K_SRC := $(TZ_SRC)$(DIR_SEP)$(KS)
TZ_U_SRC := $(TZ_SRC)$(DIR_SEP)$(US)

NORM_SRC := $(ANDIX_SRC_DIR)$(DIR_SEP)$(NORM)
NORM_K_SRC := $(NORM_SRC)$(DIR_SEP)$(KS)
NORM_U_SRC := $(NORM_SRC)$(DIR_SEP)$(US)

####
# Abstract includes
####

TZ_INC := $(ANDIX_INC_DIR)$(DIR_SEP)$(TZ)
TZ_K_INC := $(TZ_INC)$(DIR_SEP)$(KS)
TZ_U_INC := $(TZ_INC)$(DIR_SEP)$(US)

NORM_INC := $(ANDIX_INC_DIR)$(DIR_SEP)$(NORM)
NORM_K_INC := $(NORM_INC)$(DIR_SEP)$(KS)
NORM_U_INC := $(NORM_INC)$(DIR_SEP)$(US)

####
# Include Directory Variables
####

ANDIX_INC_COMMON := $(ANDIX_INC_DIR)/common
TOOLS_INC := $(ANDIX_INC_DIR)/tools

####
# Trusted World includes
####

TZ_KERNEL_INC := $(TZ_K_INC)/kernel


TZ_USER_C_INC := $(TZ_U_INC)/newlib_c
TZ_USER_TEE_INC := $(TZ_U_INC)/tee_runtime
TZ_USER_MANAGER_INC := $(TZ_U_INC)/andix_manager
TZ_USER_SAMPLE_INC := $(TZ_U_INC)/sample_trustlet
TZ_USER_RSA_INC := $(TZ_U_INC)/rsa_trustlet

####
# Normal World includes
####

NORM_KS_APP_INC := $(NORM_K_INC)/tz_application_mod
NORM_KS_SERV_INC := $(NORM_K_INC)/tz_service_mod

NORM_US_APP_DAEMON_INC := $(NORM_U_INC)/tz_application_daemon
NORM_US_APP_LIB_INC := $(NORM_U_INC)/tz_application_library
NORM_US_APP_TZ_TEST_INC := $(NORM_U_INC)/tz_mem_test
NORM_US_APP_INC := $(NORM_U_INC)/tz_application
NORM_US_APP_ENC_INC := $(NORM_U_INC)/tz_encrypter
NORM_US_APP_RSA_INC := $(NORM_U_INC)/tz_rsa
NORM_US_SERVICE_DAEMON_INC := $(NORM_U_INC)/tz_service_daemon

####
# Tool includes
####

TOOL_PACKER_INC := $(TOOLS_INC)/andix_packer

####
# Source Directory Variables
####

ANDIX_SRC_COMMON := $(ANDIX_SRC_DIR)/common
TOOLS_SRC := $(ANDIX_SRC_DIR)/tools

####
# Trusted World sources
####

TZ_KERNEL_SRC := $(TZ_K_SRC)/kernel

TZ_USER_C_SRC := $(TZ_U_SRC)/newlib_c
TZ_USER_TEE_SRC := $(TZ_U_SRC)/tee_runtime
TZ_USER_MANAGER_SRC := $(TZ_U_SRC)/andix_manager
TZ_USER_SAMPLE_SRC := $(TZ_U_SRC)/sample_trustlet
TZ_USER_RSA_SRC := $(TZ_U_SRC)/rsa_trustlet

####
# Normal World sources
####

NORM_KS_APP_SRC := $(NORM_K_SRC)/tz_application_mod
NORM_KS_SERV_SRC := $(NORM_K_SRC)/tz_service_mod

NORM_US_APP_DAEMON_SRC := $(NORM_U_SRC)/tz_application_daemon
NORM_US_APP_LIB_SRC := $(NORM_U_SRC)/tz_application_library
NORM_US_APP_SRC := $(NORM_U_SRC)/tz_application
NORM_US_APP_TZ_TEST_SRC := $(NORM_U_SRC)/tz_mem_test
NORM_US_APP_ENC_SRC := $(NORM_U_SRC)/tz_encrypter
NORM_US_APP_RSA_SRC := $(NORM_U_SRC)/tz_rsa
NORM_US_SERVICE_DAEMON_SRC := $(NORM_U_SRC)/tz_service_daemon

####
# Tool sources
####

TOOL_PACKER_SRC := $(TOOLS_SRC)/andix_packer
TOOL_SCRIPTS_SRC := $(TOOLS_SRC)/scripts
