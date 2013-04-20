/*
 * ta_interface.c
 *
 *  Created on: Jan 5, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <ta_interface.h>

extern TEE_Result TA_CreateEntryPoint( void );

extern void TA_DestroyEntryPoint( void );

extern TEE_Result TA_OpenSessionEntryPoint(__uint32_t_ paramTypes,
TEE_Param params[4], void** sessionContext );

extern void TA_CloseSessionEntryPoint(void* sessionContext);

extern TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4] );

void __ta_main() {
	TA_RPC rpc_request;
	__uint8_t_ running = 1;

	//TA_CreateEntryPoint();

	while(running) {
		rpc_request.operation = TA_INVALID;
		rpc_request.commandID = 0;
		rpc_request.paramTypes = 0;
		__ta_get_secure_request(&rpc_request);
		rpc_request.result = 0;
		switch(rpc_request.operation) {
			case TA_CREATE:
				rpc_request.result = TA_CreateEntryPoint();
				break;
			case TA_DESTROY:
				running = 0;
				break;
			case TA_OPEN_SESSION:
				rpc_request.result = TA_OpenSessionEntryPoint(
						rpc_request.paramTypes,
						rpc_request.tee_param,
						(void**)(&rpc_request.sessionContext));
				break;
			case TA_CLOSE_SESSION:
				TA_CloseSessionEntryPoint(rpc_request.sessionContext);
				break;
			case TA_INVOKE:
				rpc_request.result = TA_InvokeCommandEntryPoint(
						rpc_request.sessionContext, rpc_request.commandID,
						rpc_request.paramTypes, rpc_request.tee_param);
				break;
			default:
				break;
		}
	}

	TA_DestroyEntryPoint();
}


