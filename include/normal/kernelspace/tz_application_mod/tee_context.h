/*
 * tee_context.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_CONTEXT_H_
#define TEE_CONTEXT_H_

#include <communication_types.h>
#include <tz_application_mod/tee_list.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/random.h>

typedef struct {
	uint32_t id;
	uint32_t tz_id;
} tee_context;

int tee_context_init(void);
tee_context* tee_context_add(void);
void tee_context_free(tee_context* ctx);
tee_context* tee_context_find_by_id(uint32_t id);
tee_context* tee_context_find_by_tzid(uint32_t tzid);

#endif /* TEE_CONTEXT_H_ */
