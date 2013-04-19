/*
 * swi_secure.h
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SWI_SECURE_H_
#define SWI_SECURE_H_

typedef uint32_t			ta_rpc_command;

#define TA_INVALID			0x00
#define TA_CREATE			0x01
#define TA_DESTROY			0x02
#define TA_OPEN_SESSION		0x03
#define TA_CLOSE_SESSION	0x04
#define TA_INVOKE			0x05

typedef union {
	struct {
		void* buffer;
		uint32_t size;
	} memref;
	struct {
		uint32_t a, b;
	} value;
} TEE_Param;

typedef struct
{
	ta_rpc_command	operation;
	uint32_t		commandID;
	uint32_t		paramTypes;
	void*			sessionContext;
	TEE_Param		tee_param[4];
	uint32_t		result;
} TA_RPC;

void swi_set_uuid(TASK_UUID* uuid);
void swi_get_secure_request(TA_RPC* rpc);

#endif /* SWI_SECURE_H_ */
