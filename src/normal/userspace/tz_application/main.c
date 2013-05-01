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

int main(int argc, char** argv) {
	printf("Starting TZ Supported Application!\n");
	TEEC_Result result;
	TEEC_Context ctx;

	result = TEEC_InitializeContext(ANDIX_TEE_NAME, &ctx);

	if(result == TEEC_SUCCESS) {
		printf("Context created successfully! (CTXID: 0x%x)\n", ctx.id);
	} else {
		printf("Context creation failed!\n");
	}

	TEEC_FinalizeContext(&ctx);

	return (0);
}
