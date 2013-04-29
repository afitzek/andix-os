/*
 * tee_session.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_SESSION_H_
#define TEE_SESSION_H_

#include <tz_application_mod/tee_context.h>

typedef struct {
	uint32_t id;
	tee_context* ctx;
	uint32_t tz_id;
} tee_session;

int tee_session_init();
tee_session* tee_session_add(tee_context* ctx);
void tee_session_free(tee_session* session);
tee_session* tee_session_find_by_id(uint32_t id);
tee_session* tee_session_find_by_tzid(uint32_t tzid);
tee_session* tee_session_find_by_ctx(tee_context* ctx);

#endif /* TEE_SESSION_H_ */
