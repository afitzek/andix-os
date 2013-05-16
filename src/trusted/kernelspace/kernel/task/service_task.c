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
#include <scheduler.h>

queue* service_queue;
task_t* service_task;

void service_entry() {
	service_request* request = NULL;

	while (1) {
		request = service_get_request();

		//request = cur_request;

		if (request == NULL ) {
			// Nothing more to do ...
			get_current_task()->state = BLOCKED;
			service_info("Service task has nothing to do ...");

			service_info("yielding");
			yield();
		} else {
			if (request->ready == 1) {
				service_error("Allready processed Service request!");
				DEBUG_STOP;
			}
			TZ_CTLR_SPACE *ctrl = mon_get_control_space();

			if (ctrl == NULL ) {
				service_error("No control structure available!");
				//yield();
				if (request->returnTask != NULL ) {
					service_info("switching to %s", request->returnTask->name);
					switch_to_task(request->returnTask);
				} else {
					service_info("yielding");
					yield();
				}
			}

			// Copy control com space
			memcpy(ctrl, request->ctrl_space, sizeof(TZ_CTLR_SPACE));
			/*service_info("PRIVATE CTRL SPACE:");
			 kprintHex(request->ctrl_space, sizeof(TZ_CTLR_SPACE));
			 service_info("SHARED CTRL SPACE:");
			 kprintHex(ctrl, sizeof(TZ_CTLR_SPACE));
			 */
			service_info("Calling service ctrl request");
			mon_ctrl_request();

			ctrl = mon_get_control_space();
			//service_info("SHARED CTRL SPACE:");
			//kprintHex(ctrl, sizeof(TZ_CTLR_SPACE));
			// Copy control com space
			memcpy(request->ctrl_space, ctrl, sizeof(TZ_CTLR_SPACE));

			service_info("Returned service ctrl request");

			request->ready = 1;

			switch_to_task(request->returnTask);
		}
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
	//cur_request = &request;
	request.returnTask = get_current_task();
	service_task->state = READY;
	//return_task = get_current_task();
	service_info("Requesting Task: %s", request.returnTask->name);
	while (request.ready != 1) {
		switch_to_task(service_task);
	}
	service_info("Returned to Task: %s", get_current_task()->name);
	//return_task = NULL;
	//cur_request = NULL;
	service_debug("Request done!");
}

void service_add_request(service_request* request) {
	queue_put(service_queue, request);
}

service_request* service_get_request() {
	return ((service_request*) queue_get(service_queue));
}
