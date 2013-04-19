/*
 * scheduler.h
 *
 *  Created on: Jan 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <common/list.h>
#include <common/string.h>
#include <common/atags.h>
#include <common.h>
#include <common/typedefs.h>
#include <monitor/monitor.h>
#include <task/task.h>

void init_scheduler();
void switch_to_task(task_t *task);
void yield();


#endif /* SCHEDULER_H_ */
