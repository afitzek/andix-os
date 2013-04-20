TOP := $(dir $(lastword $(MAKEFILE_LIST)))

include common.mk

.PHONY: all
all:
	@mkdir -p $(ANDIX_DEPLOY_DIR)/scripts
	@cp *.sh $(ANDIX_DEPLOY_DIR)/scripts
	@cp *.kld $(ANDIX_DEPLOY_DIR)/scripts
	@cp *.kuld $(ANDIX_DEPLOY_DIR)/scripts

.PHONY: clean	
clean:
	@echo Removing directory $(ANDIX_DEPLOY_DIR)/scripts
	@rm -rf $(ANDIX_DEPLOY_DIR)/scripts
	