/*
 * main.c
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tee_client_api.h>
#include <trustlets/sample_trustlet.h>
#include <tee_utils.h>

#define INPUT_BUFFER_SIZE 256

void report_error(uint32_t origin, TEEC_Result result) {
	printf("Invoke failed!\n");
	printf("  ORIGIN: %s (0x%08X)\n", TEEC_StringifyOrigin(origin), origin);
	printf("  ERROR : %s (0x%08X)\n", TEEC_StringifyError(result), result);
}

int initialize_key(TEEC_Session* session, unsigned int key) {
	TEEC_Operation operation;
	uint32_t origin;
	TEEC_Result result;

	operation.params[0].value.a = key;

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);

	result = TEEC_InvokeCommand(session, TZ_NEW_KEY, &operation, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Failed to invoke command!\n");
		report_error(origin, result);
		return (-1);
	}
	return (0);
}

int encrypt(TEEC_Context* ctx, TEEC_Session* session, unsigned int key,
		unsigned char* in, size_t inlen, crypt_pack_t* out) {
	TEEC_Operation operation;
	TEEC_SharedMemory shared_input;
	TEEC_SharedMemory shared_pack;
	uint32_t origin = 0;
	int res;
	TEEC_Result result;

	shared_input.flags = 0;
	shared_input.size = inlen;
	shared_input.buffer = in;

	shared_pack.flags = 0;
	shared_pack.size = sizeof(crypt_pack_t) + out->datasize;
	shared_pack.buffer = out;

	result = TEEC_RegisterSharedMemory(ctx, &shared_input);

	if (result != TEEC_SUCCESS) {
		printf("Failed to register memory!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	result = TEEC_RegisterSharedMemory(ctx, &shared_pack);

	if (result != TEEC_SUCCESS) {
		printf("Failed to register memory!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
			TEEC_MEMREF_WHOLE,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE);

	operation.params[0].value.a = key;

	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = shared_input.size;
	operation.params[1].memref.parent = &shared_input;

	operation.params[2].memref.offset = 0;
	operation.params[2].memref.size = shared_pack.size;
	operation.params[2].memref.parent = &shared_pack;

	result = TEEC_InvokeCommand(session, TZ_ENCRYPT, &operation, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Failed to invoke command!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	res = 0;

	clean: TEEC_ReleaseSharedMemory(&shared_input);
	TEEC_ReleaseSharedMemory(&shared_pack);
	return (res);
}

int decrypt(TEEC_Context* ctx, TEEC_Session* session, unsigned int key,
		crypt_pack_t* in, unsigned char* out, size_t outlen) {
	TEEC_Operation operation;
	TEEC_SharedMemory shared_output;
	TEEC_SharedMemory shared_pack;
	uint32_t origin = 0;
	int res;
	TEEC_Result result;

	shared_output.flags = 0;
	shared_output.size = outlen;
	shared_output.buffer = out;

	shared_pack.flags = 0;
	shared_pack.size = sizeof(crypt_pack_t) + in->datasize;
	shared_pack.buffer = in;

	result = TEEC_RegisterSharedMemory(ctx, &shared_output);

	if (result != TEEC_SUCCESS) {
		printf("Failed to register memory!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	result = TEEC_RegisterSharedMemory(ctx, &shared_pack);

	if (result != TEEC_SUCCESS) {
		printf("Failed to register memory!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
			TEEC_MEMREF_WHOLE,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE);

	operation.params[0].value.a = key;

	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = shared_pack.size;
	operation.params[1].memref.parent = &shared_pack;

	operation.params[2].memref.offset = 0;
	operation.params[2].memref.size = shared_output.size;
	operation.params[2].memref.parent = &shared_output;

	result = TEEC_InvokeCommand(session, TZ_DECRYPT, &operation, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Failed to invoke command!\n");
		report_error(origin, result);
		res = -1;
		goto clean;
	}

	res = 0;

	clean: TEEC_ReleaseSharedMemory(&shared_output);
	TEEC_ReleaseSharedMemory(&shared_pack);
	return (res);
}

void printHexMem(unsigned char* buffer, size_t size) {
	size_t idx = 0;
	printf("0x%08X:  ", idx);
	for (idx = 0; idx < size; idx++) {
		if (idx > 0 && idx % 16 == 0) {
			printf("\n0x%08X:  ", idx);
		}
		printf("%02X ", buffer[idx]);
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	TEEC_UUID uuid;
	TEEC_Result result;
	TEEC_Context ctx;
	TEEC_Session session;
	uint32_t origin;
	char buffer[INPUT_BUFFER_SIZE];
	crypt_pack_t* pack = NULL;
	char* command;
	char* args;
	char* decrypted;
	size_t declen;
	int running = 1;
	int idx = 0;
	int argIdx = 0;
	unsigned int keyID = 0;

	origin = 0;

	uuid_parse(TRUSTLET_UUID, &uuid);

	result = TEEC_InitializeContext(ANDIX_TEE_NAME, &ctx);

	if (result != TEEC_SUCCESS) {
		printf("Failed to create context!\n");
		report_error(origin, result);
		goto cleanup;
	}

	result = TEEC_OpenSession(&ctx, &session, &uuid, 0, NULL, NULL, &origin);

	if (result != TEEC_SUCCESS) {
		printf("Failed to create session!\n");
		report_error(origin, result);
		goto cleanup;
	}

	do {
		printf("> ");
		memset(buffer, 0, INPUT_BUFFER_SIZE);
		gets(buffer);
		//fgets(buffer, INPUT_BUFFER_SIZE - 1, stdin);

		command = buffer;

		// find command
		for (idx = 0; idx < INPUT_BUFFER_SIZE; idx++) {
			if (buffer[idx] == ' ') {
				buffer[idx] = '\0';
				argIdx = idx + 1;
				args = &buffer[argIdx];
				break;
			}
		}

		printf("[DBG] CMD: %s ARG: %s\n", command, args);

		if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0
				|| strcmp(command, "?") == 0) {
			printf("Available commands\n");
			printf("help                         shows this message\n");
			printf("quit                         exit application\n");
			printf("enc <keyID> <message>        encrypt\n");
			printf("init <keyID>                 initialize new key\n");
			printf("dec <keyID>                  decrypt\n");
		} else if (strcmp(command, "init") == 0) {
			for (idx = argIdx; idx < INPUT_BUFFER_SIZE; idx++) {
				if (buffer[idx] == ' ') {
					buffer[idx] = '\0';
					argIdx = idx + 1;
					break;
				}
			}

			keyID = atoi(args);

			printf("KeyID: %d\n", keyID);

			if (initialize_key(&session, keyID) == 0) {
				printf("Key %d created\n", keyID);
			}

		} else if (strcmp(command, "dec") == 0) {
			if (pack != NULL ) {
				printf("Decrypting ....\n");

				for (idx = argIdx; idx < INPUT_BUFFER_SIZE; idx++) {
					if (buffer[idx] == ' ') {
						buffer[idx] = '\0';
						argIdx = idx + 1;
						break;
					}
				}

				keyID = atoi(args);

				printf("KeyID: %d\n", keyID);

				declen = pack->datasize;
				decrypted = malloc(declen + 1);
				memset(decrypted, 0, declen + 1);

				if (decrypt(&ctx, &session, keyID, pack, decrypted, declen)
						== 0) {
					printf("Decrypted Message!\n");
					printf("PLAIN: \n");
					printHexMem(decrypted, declen);
					printf("STRING: %s\n", decrypted);
				}
				free(decrypted);
			} else {
				printf("Nothing to decrypt ....\n");
			}

		} else if (strcmp(command, "enc") == 0) {
			printf("Encrypting ....\n");

			for (idx = argIdx; idx < INPUT_BUFFER_SIZE; idx++) {
				if (buffer[idx] == ' ') {
					buffer[idx] = '\0';
					argIdx = idx + 1;
					break;
				}
			}

			keyID = atoi(args);

			args = &buffer[argIdx];

			printf("KeyID: %d\n", keyID);
			printf("Plaintext: %s (%d)\n", args, strlen(args));

			if (pack != NULL ) {
				free(pack);
				pack = NULL;
			}

			pack = malloc(sizeof(crypt_pack_t) + strlen(args));
			pack->datasize = strlen(args);

			if (encrypt(&ctx, &session, keyID, args, strlen(args), pack) == 0) {
				printf("Encrypted Message!\n");
				printf("IV: \n");
				printHexMem(pack->iv, 16);
				printf("DATA: \n");
				printHexMem(pack->data, pack->datasize);
			}
		} else if (strcmp(command, "quit") == 0) {
			printf("Exiting ....\n");

			running = 0;
		} else {
			printf("Unknown command (use h for help)\n");
		}
	} while (running);

	cleanup: if (pack != NULL ) {
		free(pack);
		pack = NULL;
	}

	if (session.id != 0) {
		TEEC_CloseSession(&session);
	}

	if (ctx.id != 0) {
		TEEC_FinalizeContext(&ctx);
	}

	return 0;
}

/*
 int main(int argc, char* argv[]) {
 TEEC_UUID uuid;
 TEEC_Context ctx;
 TEEC_Session session;
 TEEC_Result result;
 TEEC_Operation operation;
 uint32_t origin;
 size_t size = 100;

 uuid_parse(TRUSTLET_UUID, &uuid);

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
 }*/
// ABCDEFGHIJKLMNOP
// ABCDEFGHIJKLMNOPABCDEFGHIJKLMNOPABCDEFGHIJKLMNOPABCDEFGHIJKLMNOPABCDEFGHIJKLMNOPABCDEFGHIJKLMNOP
