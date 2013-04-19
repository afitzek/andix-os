/*
 * service.h
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <common/typedefs.h>
#include <communication_types.h>
#include <task/task.h>

typedef struct {
	uint8_t ready;
	TZ_CTLR_SPACE* ctrl_space;
} service_request;

void service_do_request(TZ_CTLR_SPACE* space);
void service_add_request(service_request* request);
service_request* service_get_request();
void service_entry();
void service_pre(task_t* task);

#endif /* SERVICE_H_ */
