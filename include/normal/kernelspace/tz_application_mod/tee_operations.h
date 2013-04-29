/*
 * tee_operations.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_OPERATIONS_H_
#define TEE_OPERATIONS_H_

#include <communication_types.h>
#include <tz_application_mod/tee_context.h>
#include <tz_application_mod/tee_session.h>
#include <tz_application_mod/tee_memory.h>
#include <tz_application_mod/tee_logic.h>

#define TEE_EVENT_RET_SUCCESS	(0)
#define TEE_EVENT_RET_ERROR		(-1)

typedef int (*tee_op_handler)(TZ_TEE_SPACE* com_mem);

typedef struct {
	tee_op_handler pre;
	tee_op_handler post;
} op_event;

int tee_op_dummy(TZ_TEE_SPACE* com_mem);

int tee_ctx_init_post(TZ_TEE_SPACE* com_mem);

int tee_ctx_finalize_pre(TZ_TEE_SPACE* com_mem);
int tee_ctx_finalize_post(TZ_TEE_SPACE* com_mem);

int tee_mem_reg_pre(TZ_TEE_SPACE* com_mem);
int tee_mem_reg_post(TZ_TEE_SPACE* com_mem);
int tee_mem_rel_pre(TZ_TEE_SPACE* com_mem);
int tee_mem_rel_post(TZ_TEE_SPACE* com_mem);

int tee_session_open(TZ_TEE_SPACE* com_mem);
int tee_session_close(TZ_TEE_SPACE* com_mem);

static const op_event tee_ctx_init;
static const op_event tee_ctx_finalize;
static const op_event tee_mem_reg;
static const op_event tee_mem_rel;

#endif /* TEE_OPERATIONS_H_ */
