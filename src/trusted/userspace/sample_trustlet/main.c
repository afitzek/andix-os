/*
 * main.c
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#include <trustlets/sample_trustlet.h>
#include <tee_internal_api.h>
#include <polarssl/aes.h>
#include <polarssl/entropy.h>
#include <polarssl/entropy_poll.h>
#include <polarssl/ctr_drbg.h>
#include <polarssl/pbkdf2.h>
#include <polarssl/md.h>
#include <polarssl/sha2.h>
#include <sample_internal.h>
#include <fcntl.h>
#include <swi.h>
#include <andix.h>
#include <client_constants.h>
#include <andix/entropy.h>

#define IDX_FILE "idx_file.txt"
#define INPUT_SIZE 100

TEE_Result sample_new_key(uint32_t paramTypes, TEE_Param params[4]) {
	// TODO: create new key
	keystore_t store;
	ctr_drbg_context ctr_drbg;
	entropy_context entropy;
	aes_context aes;
	char *pers = "aes_trustlet_example";
	int ret;
	uint8_t iv[16];
	uint8_t key[AES_KEY_SIZE_BYTES];
	uint8_t userkey[AES_KEY_SIZE_BYTES];
	md_context_t md_ctx;
	md_context_t md_ctx2;
	char input[INPUT_SIZE];
	int fd;
	// check parameters...

	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE)) {
		printf("SAMPLE-TRUSTLET: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	fd = open("key", O_RDWR | O_CREAT | O_TRUNC);

	if (fd < 0) {
		printf("Failed to open file key\n");
		return (TEEC_ERROR_GENERIC);
	}

	entropy_init(&entropy);
	if ((ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
			(unsigned char *) pers, strlen(pers))) != 0) {
		printf(" failed\n ! ctr_drbg_init returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	if ((ret = ctr_drbg_random(&ctr_drbg, iv, 16)) != 0) {
		printf(" failed\n ! ctr_drbg_random returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	memcpy(store.crypted_iv, iv, 16);

	if ((ret = ctr_drbg_random(&ctr_drbg, key, AES_KEY_SIZE_BYTES)) != 0) {
		printf(" failed\n ! ctr_drbg_random returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	sha2(key, AES_KEY_SIZE_BYTES, store.hash, 0);

	printf("Please enter PIN for new key: ");
	fgets(input, INPUT_SIZE, stdin);
	input[strlen(input) - 1] = '\0';

	pbkdf2_hmac(&md_ctx, input, strlen(input), pers, strlen(pers), 1024,
			AES_KEY_SIZE_BYTES, userkey);

	aes_setkey_enc(&aes, userkey, AES_KEY_SIZE_BITS);

	aes_crypt_cbc(&aes, AES_ENCRYPT, AES_KEY_SIZE_BYTES, iv, input,
			store.crypted_key);

	write(fd, &store, sizeof(keystore_t));

	close(fd);
}

TEE_Result sample_encrypt(uint32_t paramTypes, TEE_Param params[4]) {

}

TEE_Result sample_decrypt(uint32_t paramTypes, TEE_Param params[4]) {

}

TEE_Result test_entropy() {
	entropy_context entropy;
	ctr_drbg_context ctr_drbg;
	int ret;
	uint8_t iv[16];
	size_t olen;
	uint8_t key[AES_KEY_SIZE_BYTES];
	uint8_t userkey[AES_KEY_SIZE_BYTES];
	char *pers = "aes_trustlet_example";

	memset(iv, 0, 16);
	memset(key, 0, AES_KEY_SIZE_BYTES);

	printf("platform_entropy_poll\n");

	platform_pseudo_entropy(NULL, key, AES_KEY_SIZE_BYTES, &olen);

	printf("Generated random KEY:\n");

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[8], key[9], key[10],
			key[11], key[12], key[13], key[14], key[15]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[8], key[9], key[10],
			key[11], key[12], key[13], key[14], key[15]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[16], key[17], key[18],
			key[19], key[20], key[21], key[22], key[23]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[24], key[25], key[26],
			key[27], key[28], key[29], key[30], key[31]);

	printf("entropy_init\n");

	entropy_init(&entropy);

	//entropy_update_manual(&entropy, pers, strlen(pers));

	printf("entropy_add_source\n");

	entropy_add_source(&entropy, &platform_pseudo_entropy, NULL,
			ENTROPY_MIN_PLATFORM);

	printf("ctr_drbg_init\n");

	if ((ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
			(unsigned char *) pers, strlen(pers))) != 0) {
		printf(" failed! ctr_drbg_init returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	printf("ctr_drbg_random\n");

	if ((ret = ctr_drbg_random(&ctr_drbg, iv, 16)) != 0) {
		printf(" failed! ctr_drbg_random returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	printf("ctr_drbg_random\n");

	if ((ret = ctr_drbg_random(&ctr_drbg, key, AES_KEY_SIZE_BYTES)) != 0) {
		printf(" failed! ctr_drbg_random returned -0x%04x\n", -ret);
		return (TEEC_ERROR_GENERIC);
	}

	printf("Generated random IV:\n");
	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", iv[0], iv[1], iv[2],
			iv[3], iv[4], iv[5], iv[6], iv[7]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", iv[8], iv[9], iv[10],
			iv[11], iv[12], iv[13], iv[14], iv[15]);

	printf("Generated random KEY:\n");

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[8], key[9], key[10],
			key[11], key[12], key[13], key[14], key[15]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[8], key[9], key[10],
			key[11], key[12], key[13], key[14], key[15]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[16], key[17], key[18],
			key[19], key[20], key[21], key[22], key[23]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[24], key[25], key[26],
			key[27], key[28], key[29], key[30], key[31]);

	return (TEEC_SUCCESS);
}

void TA_CreateEntryPoint() {
	printf("SAMPLE-TRUSTLET: Started\n");
}

void TA_DestroyEntryPoint() {
	printf("SAMPLE-TRUSTLET: Destroyed\n");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[4],
		void** sessionContext) {
	(*sessionContext) = NULL;
	printf("SAMPLE-TRUSTLET: SESSION OPENED!\n");
	return (TEE_SUCCESS);
}

void TA_CloseSessionEntryPoint(void* sessionContext) {
	printf("SAMPLE-TRUSTLET: SESSION CLOSED!\n");
}

TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4]) {

	printf("SAMPLE-TRUSTLET: Parameter Types 0x%x\n", paramTypes);
	printf("SAMPLE-TRUSTLET: Command invoked 0x%x\n", commandID);
	switch (commandID) {
	case TZ_NEW_KEY:
		return test_entropy();
		//return (sample_new_key(paramTypes, params));
	case TZ_ENCRYPT:
		return (sample_encrypt(paramTypes, params));
	case TZ_DECRYPT:
		return (sample_decrypt(paramTypes, params));
	default:
		return (TEE_ERROR_NOT_SUPPORTED);
	}
}
