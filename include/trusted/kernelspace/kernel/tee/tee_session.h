/*
 * tee_session.h
 *
 *  Created on: Mar 21, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_SESSION_H_
#define TEE_SESSION_H_

#include <common.h>
#include <tee/tee_context.h>
#include <task/task.h>

typedef struct {
	uint32_t _id;
	task_t*  tee_application;
	tee_context* context;
	uint32_t	tee_session_handle;
} tee_session;

void tee_session_initialize();
tee_session* tee_session_create();
void tee_session_destroy(tee_session* session);
tee_session* tee_session_find(uint32_t id);
tee_session* tee_session_find_by_ctx(tee_context* context);

#endif /* TEE_SESSION_H_ */
