/*
 * service_task.c
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common.h>
#include <task/service.h>
#include <task/task.h>

queue* service_queue;
task_t* service_task;

void service_entry() {
	service_request* request = NULL;

	while (1) {
		request = service_get_request();

		if (request == NULL ) {
			// Nothing more to do ...
			get_current_task()->state = BLOCKED;
		} else {
			TZ_CTLR_SPACE *ctrl = mon_get_control_space();

			if (ctrl == NULL ) {
				service_error("No control structure available!");
				yield();
			}

			memcpy(ctrl, request->ctrl_space, sizeof(TZ_CTLR_SPACE));

			//service_info("Calling service ctrl request");

			mon_ctrl_request();

			memcpy(request->ctrl_space, ctrl, sizeof(TZ_CTLR_SPACE));

			//service_info("Returned service ctrl request");

			request->ready = 1;
		}
		yield();
	}
}

void service_pre(task_t* task) {
	service_queue = queue_init();
	service_task = task;
}

void service_do_request(TZ_CTLR_SPACE* space) {
	service_request request;
	request.ctrl_space = space;
	request.ready = 0;
	// add request wait until ready
	service_add_request(&request);
	service_task->state = READY;

	while (request.ready != 1) {
		switch_to_task(service_task);
	}

	service_debug("Request done!");
}

void service_add_request(service_request* request) {
	queue_put(service_queue, request);
}

service_request* service_get_request() {
	return ((service_request*) queue_get(service_queue));
}
