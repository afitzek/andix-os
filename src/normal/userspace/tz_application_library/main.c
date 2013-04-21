/*
 * main.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_client_api.h>
#include <stdlib.h>
#include <communication_types.h>
#include <k_comm.h>

TEEC_Result TEEC_InitializeContext(const char* name, TEEC_Context* context) {
	if (name == NULL || strcmp(name, ANDIX_TEE_NAME) == 0) {
		if (context == NULL ) {
			return (TEEC_ERROR_BAD_PARAMETERS);
		}
		TEEC_Result res = TEEC_SUCCESS;
		lockComm();
		TZ_TEE_SPACE* space = getCommSpace();

		space->op = TZ_TEE_OP_INIT_CTX;
		space->params.initCtx.context = 0;

		processComm();

		if (space->ret != TEEC_SUCCESS) {
			return space->ret;
		}

		context->id = space->params.initCtx.context;
		res = space->ret;
		unlockComm();

		return (res);
	} else {
		return (TEEC_ERROR_NOT_SUPPORTED);
	}
}

void TEEC_FinalizeContext(TEEC_Context* context) {
	if (context != NULL ) {
		TEEC_Result res = TEEC_SUCCESS;
		lockComm();
		TZ_TEE_SPACE* space = getCommSpace();

		space->op = TZ_TEE_OP_FIN_CTX;
		space->params.initCtx.context = context->id;
		context->id = 0;
		processComm();
		unlockComm();
	}
}
