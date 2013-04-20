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
			__swi_2(SWI_SYSD_INSTALL, (uint32_t)elf, pack->datalen);
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
	return (TEE_SUCCESS);
}

void TA_CloseSessionEntryPoint(void* sessionContext) {
}

TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4]) {

	switch (commandID) {
	case SYSD_CMD_INSTALL:
		return (sysd_install(paramTypes, params));
	default:
		return (TEE_ERROR_NOT_SUPPORTED);
	}
}
