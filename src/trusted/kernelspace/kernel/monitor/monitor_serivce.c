/*
 * monitor_serivce.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <monitor/monitor.h>
#include <communication_types.h>
#include <drivers/block/ns_block.h>

void mon_ctrl_request() {
	__smc_1(SSC_NONS_SERVICE, 0);
}

uint32_t monitor_com_mem_dispatcher() {
	if(com_mem == NULL) {
		return (-1);
	}


	return (0);
}
