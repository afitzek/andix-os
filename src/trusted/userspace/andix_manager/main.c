/*
 * main.c
 *
 *  Created on: Mar 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <andix.h>
#include <tee_internal_api.h>
#include <sysd.h>
#include <swi.h>
#include <andix_packer.h>
#include <client_constants.h>

void printHex(const char* name, uint8_t* buffer, uint32_t size) {
	uint32_t idx = 0;
	printf("Buffer: %s (%d)", name, size);
	for(idx = 0; idx < size; idx++) {
		if(idx % 0xF == 0) {
			printf("\n0x%08X:      ", idx);
		}
		printf("%02X ", buffer[idx]);
	}
	printf("\n");
}

TEE_Result sysd_echo(__uint32_t_ paramTypes, TEE_Param params[4]) {
	uint8_t* src = params[0].memref.buffer;
	uint8_t* dst = params[1].memref.buffer;
	uint32_t src_size = params[0].memref.size;
	uint32_t dst_size = params[1].memref.size;
	uint32_t size = 0;
	uint32_t idx = 0;

	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_INPUT,
			TEEC_MEMREF_PARTIAL_OUTPUT,
			TEEC_VALUE_INOUT,
			TEEC_NONE)) {
		printf("SYSTEM-DAEMON: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	size = (src_size > dst_size) ? dst_size : src_size;

	if (src == NULL ) {
		printf("SYSTEM-DAEMON: Error source is NULL\n");
		return (TEE_ERROR_GENERIC);
	}

	if (dst == NULL ) {
		printf("SYSTEM-DAEMON: Error destination is NULL\n");
		return (TEE_ERROR_GENERIC);
	}

	printHex("INPUT", src, src_size);

	printHex("OUTPUT", dst, dst_size);

	//memcpy(dst, src, size);
	for(idx = 0; idx < size; idx++) {
		dst[idx] = src[idx] ^ 0x76;
	}

	printf("Hello world from SYSTEM-DAEMON (a: 0x%X, b: 0x%X)\n",
			params[2].value.a, params[2].value.b);

	return (TEE_SUCCESS);
}

TEE_Result sysd_install(__uint32_t_ paramTypes, TEE_Param params[4]) {
	andixpack* pack;
	uint8_t* elf;
	char c = "t";
	if (paramTypes != TEE_PARAM_TYPES(
			TEE_PARAM_TYPE_MEMREF_INPUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE)) {
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}
	pack = params[0].memref.buffer;
	elf = params[0].memref.buffer;
	elf = elf + sizeof(andixpack);
	uint32_t totalsize = pack->datalen + sizeof(andixpack);
	if (totalsize != params[0].memref.size) {
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}
	// TODO: verify application package
	printf("Got request to install:\n");
	printf("    NAME: %s\n", pack->name);
	printf("    UUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
			pack->uuid.timeLow, pack->uuid.timeMid, pack->uuid.timeHiAndVersion,
			pack->uuid.clockSeqAndNode[7], pack->uuid.clockSeqAndNode[6],
			pack->uuid.clockSeqAndNode[5], pack->uuid.clockSeqAndNode[4],
			pack->uuid.clockSeqAndNode[3], pack->uuid.clockSeqAndNode[2],
			pack->uuid.clockSeqAndNode[1], pack->uuid.clockSeqAndNode[0]);
	printf("Do you want to install this application? (y/n)");
	while (c != 'n' && c != 'y') {
		c = getchar();
		if (c == 'y') {
			printf("\ninstalling... (0x%x)\n", elf);
			//TODO call install routine
			__swi_2(SWI_SYSD_INSTALL, (uint32_t) elf, pack->datalen);
		} else if (c == 'n') {
			printf("\nNot installing...\n");
			return (TEE_ERROR_ACCESS_DENIED);
		} else {
			printf("\nDidn't get your answer? (y/n)");
		}
	}
	return (TEE_SUCCESS);
}

void TA_CreateEntryPoint() {
	printf("SYSTEM-DAEMON: Started\n");
}

void TA_DestroyEntryPoint() {
	printf("SYSTEM-DAEMON: Destroyed\n");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[4],
		void** sessionContext) {
	(*sessionContext) = NULL;
	printf("SYSTEM-DAEMON: SESSION OPENED!\n");
	return (TEE_SUCCESS);
}

void TA_CloseSessionEntryPoint(void* sessionContext) {
	printf("SYSTEM-DAEMON: SESSION CLOSED!\n");
}

TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4]) {

	printf("SYSTEM-DAEMON: Parameter Types 0x%x\n", paramTypes);
	printf("SYSTEM-DAEMON: Command invoked 0x%x\n", commandID);
	switch (commandID) {
	case SYSD_CMD_INSTALL:
		return (sysd_install(paramTypes, params));
	case 0x2:
		return (sysd_echo(paramTypes, params));
	default:
		return (TEE_ERROR_NOT_SUPPORTED);
	}
}
