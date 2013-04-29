/*
 * tee_logic.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_logic.h>
#include <communication_types.h>
#include <tz_application_mod/tee_operations.h>
#include <tee_client_api.h>

int tee_init() {
	if (tee_context_init() != 0) {
		return (-1);
	}

	if (tee_session_init() != 0) {
		return (-1);
	}

	if (tee_memory_init() != 0) {
		return (-1);
	}

	return (0);
}

int tee_process(TZ_TEE_SPACE* com_mem) {
	int result = 0;
	int tz_result = 0;
	op_event* event = NULL;

	switch (com_mem->op) {
	case TZ_TEE_OP_INIT_CTX:
		event = &tee_ctx_init;
		break;
	case TZ_TEE_OP_FIN_CTX:
		event = &tee_ctx_finalize;
		break;
	case TZ_TEE_OP_REGISTER_MEM:
		event = &tee_ctx_init;
		break;
	case TZ_TEE_OP_RELEASE_MEM:
		event = &tee_ctx_init;
		break;
	}

	// Perform normal world operations
	if (event == NULL) {
		printk(KERN_ERR "tee_process: no event found");
		com_mem->ret = TEEC_ERROR_COMMUNICATION;
		return (-1);
	}

	result = event->pre(com_mem);

	if (result != TEE_EVENT_RET_SUCCESS) {
		printk(KERN_ERR "tee_process: pre event trigger failed!");
		com_mem->ret = TEEC_ERROR_COMMUNICATION;
		return (-1);
	}

	// call TZ
	CP15DMB;
	CP15DSB;
	CP15ISB;
	tz_result = __smc_1(SMC_PROCESS_TMEM, 0);

	if (tz_result == CTRL_STRUCT) {
		tz_result = tz_process_ctrl_mem();
	}

	result = event->post(com_mem);

	if (result != TEE_EVENT_RET_SUCCESS) {
		printk(KERN_ERR "tee_process: post event trigger failed!");
		com_mem->ret = TEEC_ERROR_COMMUNICATION;
		return (-1);
	}

	return (tz_result);
}
