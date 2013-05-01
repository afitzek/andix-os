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

TEEC_Result TEEC_RegisterSharedMemory(TEEC_Context* context,
		TEEC_SharedMemory* sharedMem) {
	if (context == NULL || sharedMem == NULL ) {
		return (TEEC_ERROR_BAD_PARAMETERS);
	}

	TEEC_Result res = TEEC_SUCCESS;
	lockComm();
	TZ_TEE_SPACE* space = getCommSpace();

	space->op = TZ_TEE_OP_REGISTER_MEM;
	space->params.regMem.context = context->id;
	space->params.regMem.flags = sharedMem->flags;
	space->params.regMem.paddr = sharedMem->buffer;
	space->params.regMem.size = sharedMem->size;

	processComm();

	if (space->ret != TEEC_SUCCESS) {
		return (space->ret);
	}

	sharedMem->memid = space->params.regMem.memid;
	res = space->ret;
	unlockComm();

	return (res);
}

TEEC_Result TEEC_AllocateSharedMemory(TEEC_Context* context,
		TEEC_SharedMemory* sharedMem) {
	return (TEEC_ERROR_NOT_IMPLEMENTED);
}

void TEEC_ReleaseSharedMemory(TEEC_SharedMemory* sharedMem) {
	if (sharedMem != NULL ) {
		lockComm();
		TZ_TEE_SPACE* space = getCommSpace();
		space->op = TZ_TEE_OP_RELEASE_MEM;
		space->params.regMem.context = 0;
		space->params.regMem.memid = sharedMem->memid;
		space->params.regMem.paddr = sharedMem->buffer;
		space->params.regMem.size = sharedMem->size;
		processComm();
		sharedMem->memid = 0;
		unlockComm();
	}
}

