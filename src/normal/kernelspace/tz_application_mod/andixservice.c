/*
 * andixservice.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_application_mod/andixtee.h>

TZ_CTLR_SPACE ctrlspace;
int pending;

int poll_ctrl_task() {

}

int register_tee_mem_in_tz(TZ_TEE_SPACE *com) {
	uint32_t paddr = virt_to_phys((void*) com);
	uint32_t res = 0;
	printk(
			KERN_INFO "Registering tee memory virtual 0x%x physical 0x%x",
			(unsigned int)com, paddr);
	res = __smc_1(SMC_REGISTER_TMEM, paddr);
	return ((int) res);
}

int unregister_tee_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering tee memory");
	res = __smc_1(SMC_UNREGISTER_TMEM, 0);
	return ((int) res);
}
