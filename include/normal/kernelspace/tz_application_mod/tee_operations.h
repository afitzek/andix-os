/*
 * tee_operations.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_OPERATIONS_H_
#define TEE_OPERATIONS_H_

#define TEE_EVENT_RET_SUCCESS	(0)
#define TEE_EVENT_RET_ERROR		(-1)

typedef int (*tee_op_handler)(TZ_TEE_SPACE* com_mem);

typedef struct {
	tee_op_handler pre;
	tee_op_handler post;
} op_event;

int tee_op_dummy();

int tee_ctx_init_post(TZ_TEE_SPACE* com_mem);

int tee_ctx_finalize_pre(TZ_TEE_SPACE* com_mem);
int tee_ctx_finalize_post(TZ_TEE_SPACE* com_mem);

int tee_mem_reg(TZ_TEE_SPACE* com_mem);
int tee_mem_rel(TZ_TEE_SPACE* com_mem);
int tee_session_open(TZ_TEE_SPACE* com_mem);
int tee_session_close(TZ_TEE_SPACE* com_mem);

static const op_event tee_ctx_init = {
	.pre = &tee_op_dummy;
	.port = &tee_ctx_init_post;
};

static const op_event tee_ctx_finalize = {
	.pre = &tee_op_dummy;
	.port = &tee_ctx_finalize_post;
};

#endif /* TEE_OPERATIONS_H_ */
