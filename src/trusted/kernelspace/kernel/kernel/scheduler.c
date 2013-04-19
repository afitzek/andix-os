/*
 * scheduler.c
 *
 *  Created on: Jan 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <scheduler.h>

void init_scheduler() {
	scheduler_debug("Scheduler initialization");
}

void switch_to_task(task_t *task) {
	__smc_1(SSC_TASK_SWITCH, (uint32_t)task);
}

void yield() {
	__smc_1(SSC_TASK_SCHEDULE, 0);
}
