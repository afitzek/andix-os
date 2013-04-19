/*
 * idle_task.c
 *
 *  Created on: Jan 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/typedefs.h>
#include <scheduler.h>

void idle_task_entry() {
	while(1) {
		NOP;
		yield();
	}
}
