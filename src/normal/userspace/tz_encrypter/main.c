/*
 * main.c
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#include <stdlib.h>
#include <tee_client_api.h>
#include <trustlets/sample_trustlet.h>
#include <tee_utils.h>

int main(int argc, char* argv[]) {
	TEEC_UUID uuid;
	TEEC_Context ctx;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_Operation operation;
	uint32_t origin;
	size_t size = 100;

	// 7e58ce53-0ff2-4356-b1bd-cf81b708c6d5
	/*uuid.timeLow = 0x7e58ce53;
	uuid.timeMid = 0x0ff2;
	uuid.timeHiAndVersion = 0x4356;
	uuid.clockSeqAndNode[7] = 0xb1;
	uuid.clockSeqAndNode[6] = 0xbd;
	uuid.clockSeqAndNode[5] = 0xcf;
	uuid.clockSeqAndNode[4] = 0x81;
	uuid.clockSeqAndNode[3] = 0xb7;
	uuid.clockSeqAndNode[2] = 0x08;
	uuid.clockSeqAndNode[1] = 0xc6;
	uuid.clockSeqAndNode[0] = 0xd6;*/

	uuid_parse("7e58ce53-0ff2-4356-b1bd-cf81b708c6d5", &uuid);

	result = TEEC_InitializeContext(ANDIX_TEE_NAME, &ctx);

	if (result != TEEC_SUCCESS) {
		printf("Failed to create context!\n");
		goto cleanup;
	}

	result = TEEC_OpenSession(&ctx, &session, &uuid, 0, NULL, NULL, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Failed to create session @ %d!\n");
		goto cleanup;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = 100;

	result = TEEC_InvokeCommand(&session, TZ_NEW_KEY, &operation, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Invoke failed @ %d!\n", origin);
		goto cleanup;
	}

	cleanup: if (session.id != 0) {
		TEEC_CloseSession(&session);
	}

	if (ctx.id != 0) {
		TEEC_FinalizeContext(&ctx);
	}

	return 0;
}
