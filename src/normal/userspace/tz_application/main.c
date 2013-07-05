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

#include <stdio.h>
#include <errno.h>
#include <tee_client_api.h>
#include <stdio.h>

void printHex(const char* name, uint8_t* buffer, uint32_t size) {
	uint32_t idx = 0;
	printf("Buffer: %s", name);
	for (idx = 0; idx < size; idx++) {
		if (idx % 0xF == 0) {
			printf("\n0x%08X:      ", idx);
		}
		printf("%02X ", buffer[idx]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	printf("Starting TZ Supported Application!\n");
	TEEC_Result result;
	TEEC_Context ctx;
	TEEC_SharedMemory mema;
	TEEC_SharedMemory memb;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_UUID uuid;
	uint32_t origin;
	size_t size = 100;
	void* buffera = malloc(size);
	void* bufferb = malloc(size);

	uuid.timeLow = 0x47b57610;
	uuid.timeMid = 0x925b;
	uuid.timeHiAndVersion = 0x11e2;
	uuid.clockSeqAndNode[7] = 0x9e;
	uuid.clockSeqAndNode[6] = 0x96;
	uuid.clockSeqAndNode[5] = 0x08;
	uuid.clockSeqAndNode[4] = 0x00;
	uuid.clockSeqAndNode[3] = 0x20;
	uuid.clockSeqAndNode[2] = 0x0c;
	uuid.clockSeqAndNode[1] = 0x9a;
	uuid.clockSeqAndNode[0] = 0x66;

	memset(buffera, 0x9A, size);
	memset(bufferb, 0x0, size);

	mema.buffer = buffera;
	mema.size = size;
	mema.flags = 0;

	memb.buffer = bufferb;
	memb.size = size;
	memb.flags = 0;

	result = TEEC_InitializeContext(ANDIX_TEE_NAME, &ctx);

	if (result == TEEC_SUCCESS) {
		printf("Context created successfully! (CTXID: 0x%x)\n", ctx.id);

		result = TEEC_RegisterSharedMemory(&ctx, &mema);

		if (result == TEEC_SUCCESS) {
			printf("Shared memory created successfully!\n");

			result = TEEC_RegisterSharedMemory(&ctx, &memb);

			result = TEEC_OpenSession(&ctx, &session, &uuid, 0, NULL, NULL,
					&origin);

			if (result == TEEC_SUCCESS) {
				printf("session opened successfully!\n");

				operation.paramTypes = TEEC_PARAM_TYPES(
						TEEC_MEMREF_PARTIAL_INPUT,TEEC_MEMREF_PARTIAL_OUTPUT,
						TEEC_VALUE_INOUT, TEEC_NONE);

				operation.params[0].memref.parent = &mema;
				operation.params[0].memref.offset = 0;
				operation.params[0].memref.size = size;
				operation.params[1].memref.parent = &memb;
				operation.params[1].memref.offset = 0;
				operation.params[1].memref.size = size;

				operation.params[2].value.a = 0xAA;
				operation.params[2].value.b = 0xBB;

				printf("INVOKE COMMAND\n");

				TEEC_InvokeCommand(&session, 0x2, &operation, &origin);

				TEEC_CloseSession(&session);
			} else {
				printf("open session failed!\n");
			}

			TEEC_ReleaseSharedMemory(&mema);
			TEEC_ReleaseSharedMemory(&memb);
		} else {
			printf("Shared memory creation failed!\n");
		}
	} else {
		printf("Context creation failed!\n");
	}

	TEEC_FinalizeContext(&ctx);

	printHex("bufferA", buffera, size);
	printHex("bufferB", bufferb, size);

	return (0);
}
