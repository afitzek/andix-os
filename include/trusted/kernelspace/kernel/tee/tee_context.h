/*
 * tee_context.h
 *
 *  Created on: Mar 21, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_CONTEXT_H_
#define TEE_CONTEXT_H_

#include <common.h>

typedef struct {
	uint32_t	_id;
} tee_context;

void tee_context_initialize();
tee_context* tee_context_create();
void tee_context_remove(tee_context* ctx);
tee_context* tee_context_find(uint32_t id);

#endif /* TEE_CONTEXT_H_ */
