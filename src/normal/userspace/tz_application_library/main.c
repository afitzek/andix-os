/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
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
#include <string.h>

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
	space->params.regMem.paddr = (uint32_t) sharedMem->buffer;
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
		space->params.regMem.paddr = (uint32_t)sharedMem->buffer;
		space->params.regMem.size = sharedMem->size;
		space->params.regMem.flags = sharedMem->flags;
		processComm();
		sharedMem->memid = 0;
		unlockComm();
	}
}

TEEC_Result TEEC_OpenSession(TEEC_Context* context, TEEC_Session* session,
		const TEEC_UUID* destination, uint32_t connectionMethod,
		const void* connectionData, TEEC_Operation* operation,
		uint32_t* returnOrigin) {
	TEEC_Result res = TEEC_SUCCESS;
	if (context == NULL || session == NULL || destination == NULL ) {
		if (returnOrigin != NULL ) {
			(*returnOrigin) = TEEC_ORIGIN_COMMS;
		}
		return (TEEC_ERROR_BAD_PARAMETERS);
	}

	lockComm();
	TZ_TEE_SPACE* space = getCommSpace();
	space->op = TZ_TEE_OP_OPEN_SESSION;
	space->params.openSession.connectionData = (uint32_t)connectionData;
	space->params.openSession.connectionMethod = connectionMethod;
	space->params.openSession.context = context->id;
	space->params.openSession.session = session->id;
	memcpy(&space->params.openSession.uuid, destination, sizeof(TEEC_UUID));

	if (operation != NULL ) {
		if (processParameters(operation,
				&space->params.openSession.operation) != TEEC_SUCCESS) {
			if (returnOrigin != NULL ) {
				(*returnOrigin) = TEEC_ORIGIN_COMMS;
			}
			unlockComm();
			return (TEEC_ERROR_BAD_PARAMETERS);
		}
		space->params.openSession.operation.valid = 1;
	} else {
		space->params.openSession.operation.valid = 0;
	}

	processComm();

	if (returnOrigin != NULL ) {
		(*returnOrigin) = space->params.openSession.returnOrigin;
	}

	if(space->ret == TEEC_SUCCESS) {
		session->id = space->params.openSession.session;
	}

	res = space->ret;

	unlockComm();

	return (res);
}

void TEEC_CloseSession(TEEC_Session* session) {
	if (session != NULL ) {
		lockComm();

		TZ_TEE_SPACE* space = getCommSpace();
		space->op = TZ_TEE_OP_CLOSE_SESSION;
		space->params.closeSession.session = session->id;

		processComm();

		unlockComm();
	}
}

TEEC_Result TEEC_InvokeCommand(TEEC_Session* session, uint32_t commandID,
		TEEC_Operation* operation, uint32_t* returnOrigin) {

	TEEC_Result res = TEEC_SUCCESS;

	if (session == NULL ) {
		if (returnOrigin != NULL ) {
			(*returnOrigin) = TEEC_ORIGIN_COMMS;
		}
		return (TEEC_ERROR_BAD_PARAMETERS);
	}

	lockComm();
	TZ_TEE_SPACE* space = getCommSpace();

	space->op = TZ_TEE_OP_INVOKE_CMD;
	space->params.invokeCommand.command = commandID;

	if (processParameters(operation,
			&space->params.invokeCommand.operation) != TEEC_SUCCESS) {
		if (returnOrigin != NULL ) {
			(*returnOrigin) = TEEC_ORIGIN_COMMS;
		}
		unlockComm();
		return (TEEC_ERROR_BAD_PARAMETERS);
	}
	space->params.invokeCommand.operation.valid = 1;
	space->params.invokeCommand.session = session->id;

	processComm();

	if (processParameters(&space->params.invokeCommand.operation,
			operation) != TEEC_SUCCESS) {
			if (returnOrigin != NULL ) {
				(*returnOrigin) = TEEC_ORIGIN_COMMS;
			}
			unlockComm();
			return (TEEC_ERROR_BAD_PARAMETERS);
		}

	if (returnOrigin != NULL ) {
		(*returnOrigin) = space->params.openSession.returnOrigin;
	}

	res = space->ret;

	unlockComm();

	return (res);
}

void TEEC_RequestCancellation(TEEC_Operation* operation) {
	// not supported!!
}

const char* TEEC_StringifyOrigin(uint32_t origin) {
	if(origin == TEEC_ORIGIN_API) {
		return ("TEEC_ORIGIN_API");
	} else if(origin == TEEC_ORIGIN_COMMS) {
		return ("TEEC_ORIGIN_COMMS");
	} else if(origin == TEEC_ORIGIN_TEE) {
		return ("TEEC_ORIGIN_TEE");
	} else if(origin == TEEC_ORIGIN_TRUSTED_APP) {
		return ("TEEC_ORIGIN_TRUSTED_APP");
	} else {
		return ("UNKNOWN");
	}
}

const char* TEEC_StringifyError(TEEC_Result result) {
	if(result == TEEC_SUCCESS) {
		return ("TEEC_SUCCESS");
	} else if(result == TEEC_ERROR_ACCESS_CONFLICT) {
		return ("TEEC_ERROR_ACCESS_CONFLICT");
	} else if(result == TEEC_ERROR_ACCESS_DENIED) {
		return ("TEEC_ERROR_ACCESS_DENIED");
	} else if(result == TEEC_ERROR_BAD_FORMAT) {
		return ("TEEC_ERROR_BAD_FORMAT");
	} else if(result == TEEC_ERROR_BAD_PARAMETERS) {
		return ("TEEC_ERROR_BAD_PARAMETERS");
	} else if(result == TEEC_ERROR_BAD_STATE) {
		return ("TEEC_ERROR_BAD_STATE");
	} else if(result == TEEC_ERROR_BUSY) {
		return ("TEEC_ERROR_BUSY");
	} else if(result == TEEC_ERROR_CANCEL) {
		return ("TEEC_ERROR_CANCEL");
	} else if(result == TEEC_ERROR_COMMUNICATION) {
		return ("TEEC_ERROR_COMMUNICATION");
	} else if(result == TEEC_ERROR_EXCESS_DATA) {
		return ("TEEC_ERROR_EXCESS_DATA");
	} else if(result == TEEC_ERROR_GENERIC) {
		return ("TEEC_ERROR_GENERIC");
	} else if(result == TEEC_ERROR_ITEM_NOT_FOUND) {
		return ("TEEC_ERROR_ITEM_NOT_FOUND");
	} else if(result == TEEC_ERROR_NOT_IMPLEMENTED) {
		return ("TEEC_ERROR_NOT_IMPLEMENTED");
	} else if(result == TEEC_ERROR_NOT_SUPPORTED) {
		return ("TEEC_ERROR_NOT_SUPPORTED");
	} else if(result == TEEC_ERROR_NO_DATA) {
		return ("TEEC_ERROR_NO_DATA");
	} else if(result == TEEC_ERROR_OUT_OF_MEMORY) {
		return ("TEEC_ERROR_OUT_OF_MEMORY");
	} else if(result == TEEC_ERROR_SECURITY) {
		return ("TEEC_ERROR_SECURITY");
	} else if(result == TEEC_ERROR_SHORT_BUFFER) {
		return ("TEEC_ERROR_SHORT_BUFFER");
	} else {
		return ("UNKNOWN");
	}
}

