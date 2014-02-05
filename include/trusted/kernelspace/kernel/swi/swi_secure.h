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
 * swi_secure.h
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SWI_SECURE_H_
#define SWI_SECURE_H_

#include <task/uuid.h>

/**
 * \addtogroup swi
 *	\{
 */

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

/**
 * Set uuid of task
 */
void swi_set_uuid(TASK_UUID* uuid);

/**
 * Gets tee rpc request
 */
void swi_get_secure_request(TA_RPC* rpc);

/* \} group */

#endif /* SWI_SECURE_H_ */
