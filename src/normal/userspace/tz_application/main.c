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
