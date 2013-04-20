/*
 * andixservice.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_service_mod/andixtz.h>

int register_com_mem_in_tz(TZ_CTLR_SPACE *com) {
	uint32_t paddr = virt_to_phys((void*) com);
	uint32_t res = 0;
	printk(
			KERN_INFO "Registering communication memory virtual 0x%x physical 0x%x",
			(unsigned int)com, paddr);
	res = __smc_1(SMC_REGISTER_CMEM, paddr);
	return ((int) res);
}

int unregister_com_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering communication memory");
	res = __smc_1(SMC_UNREGISTER_CMEM, 0);
	return ((int) res);
}
