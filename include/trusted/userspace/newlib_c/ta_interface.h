/*
 * ta_interface.h
 *
 *  Created on: Jan 5, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TA_INTERFACE_H_
#define TA_INTERFACE_H_

#include <andix.h>

typedef __uint32_t_			ta_rpc_command;

#define TA_INVALID			0x00
#define TA_CREATE			0x01
#define TA_DESTROY			0x02
#define TA_OPEN_SESSION		0x03
#define TA_CLOSE_SESSION	0x04
#define TA_INVOKE			0x05

typedef struct
{
	ta_rpc_command	operation;
	__uint32_t_		commandID;
	__uint32_t_		paramTypes;
	void*			sessionContext;
	TEE_Param		tee_param[4];
	__uint32_t_		result;
} TA_RPC;

/**
 * Trustlet will wait on this call and will return when a request is available
 */
void __ta_get_secure_request(TA_RPC* rpc);

void __ta_main();

#endif /* TA_INTERFACE_H_ */
