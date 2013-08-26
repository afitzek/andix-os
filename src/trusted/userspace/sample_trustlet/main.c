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
 *  Created on: May 13, 2013
 *      Author: andy
 */

#include <trustlets/sample_trustlet.h>
#include <tee_internal_api.h>
#include <tropicssl/aes.h>
#include <tropicssl/pbkdf2.h>
#include <tropicssl/sha2.h>
#include <sample_internal.h>
#include <fcntl.h>
#include <swi.h>
#include <andix.h>
#include <client_constants.h>
#include <andix/entropy.h>
#include <string.h>
#include <stdlib.h>

#define IDX_FILE "idx_file.txt"
#define INPUT_SIZE 100
#define PBKDF2_ITERATIONS 2048
#define PBKDF2_SALT "aes_trustlet_example"

void printAESKey(uint8_t* key) {
	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[0], key[1], key[2],
			key[3], key[4], key[5], key[6], key[7]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[8], key[9], key[10],
			key[11], key[12], key[13], key[14], key[15]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[16], key[17],
			key[18], key[19], key[20], key[21], key[22], key[23]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", key[24], key[25],
			key[26], key[27], key[28], key[29], key[30], key[31]);
}

void printIVKey(uint8_t* iv) {
	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", iv[0], iv[1], iv[2],
			iv[3], iv[4], iv[5], iv[6], iv[7]);

	printf("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", iv[8], iv[9], iv[10],
			iv[11], iv[12], iv[13], iv[14], iv[15]);
}

TEE_Result sample_new_key(uint32_t paramTypes, TEE_Param params[4]) {
	// TODO: create new key
	keystore_t store;
	aes_context aes;
	char *pers = PBKDF2_SALT;
	int ret;
	uint8_t iv[16];
	uint8_t masterkey[AES_KEY_SIZE_BYTES];
	uint8_t userkey[AES_KEY_SIZE_BYTES];
	char key_file_name[20];
	char input[INPUT_SIZE];
	int fd;
	size_t olen;
	// check parameters...

	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_VALUE_INPUT, // key_id
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE)) {
		printf("SAMPLE-TRUSTLET: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	// Open key file
	memset(key_file_name, 0, 20);

	snprintf(key_file_name, 20, "key_%X", params[0].value.a);

	fd = open(key_file_name, O_RDWR | O_CREAT | O_TRUNC);

	if (fd < 0) {
		printf("Failed to open file key\n");
		return (TEEC_ERROR_GENERIC);
	}

	// Generate KEY IV
	platform_pseudo_entropy(NULL, iv, 16, &olen);

	memcpy(store.crypted_iv, iv, 16);

	// Generate secret key
	platform_pseudo_entropy(NULL, masterkey, AES_KEY_SIZE_BYTES, &olen);

	// Store hash of key
	sha2(masterkey, AES_KEY_SIZE_BYTES, store.hash, 0);

	printf("Please enter PIN for new key: \n");
	printf("------------------------------\n");
	read(0, input, INPUT_SIZE);
	printf("------------------------------\n");

	// Generate aes key to encrypt real key
	pbkdf2_sha256_hmac(input, strlen(input), pers, strlen(pers),
			PBKDF2_ITERATIONS, AES_KEY_SIZE_BYTES, userkey);

	// Encrypt master key with derived key
	aes_setkey_enc(&aes, userkey, AES_KEY_SIZE_BITS);

	aes_crypt_cbc(&aes, AES_ENCRYPT, AES_KEY_SIZE_BYTES, iv, masterkey,
			store.crypted_key);
	/*
	 printf("Stored IV: \n");
	 printIVKey(store.crypted_iv);

	 printf("Stored encrypted Key: \n");
	 printAESKey(store.crypted_key);

	 printf("Stored plain Key: \n");
	 printAESKey(masterkey);
	 */
	// store generated key to file
	write(fd, &store, sizeof(keystore_t));

	close(fd);
	return (TEEC_SUCCESS);
}

TEE_Result read_key(const char* file, uint8_t* decryptedKey, uint32_t keyID) {
	int fd = 0;
	keystore_t store;
	size_t ssize = 0;
	char input[INPUT_SIZE];
	int tries = 0;
	int ok = 0;
	uint8_t masterkey[AES_KEY_SIZE_BYTES];
	uint8_t userkey[AES_KEY_SIZE_BYTES];
	uint8_t keyhash[HASH_BYTES];
	char *pers = PBKDF2_SALT;
	aes_context aes;

	fd = open(file, O_RDONLY);

	if (fd <= 0) {
		printf("Failed to open key\n");
		return (TEEC_ERROR_ITEM_NOT_FOUND);
	}

	if (read(fd, &store, sizeof(keystore_t)) != sizeof(keystore_t)) {
		printf("Failed to open key\n");
		close(fd);
		return (TEEC_ERROR_ITEM_NOT_FOUND);
	}

	close(fd);
	/*
	 printf("Stored IV: \n");
	 printIVKey(store.crypted_iv);

	 printf("Stored encrypted Key: \n");
	 printAESKey(store.crypted_key);
	 */
	while (1) {
		memset(input, 0, INPUT_SIZE);
		printf("Please enter PIN key [%u]: \n", keyID);
		printf("------------------------------\n");
		read(0, input, INPUT_SIZE);
		printf("------------------------------\n");

		// Generate aes key to encrypt real key
		pbkdf2_sha256_hmac(input, strlen(input), pers, strlen(pers),
				PBKDF2_ITERATIONS, AES_KEY_SIZE_BYTES, userkey);

		// Decrypt master key with derived key
		aes_setkey_dec(&aes, userkey, AES_KEY_SIZE_BITS);

		aes_crypt_cbc(&aes, AES_DECRYPT, AES_KEY_SIZE_BYTES, store.crypted_iv,
				store.crypted_key, masterkey);

		// check if key is correct
		sha2(masterkey, AES_KEY_SIZE_BYTES, keyhash, 0);

		if (memcmp(keyhash, store.hash, HASH_BYTES) == 0) {
			memcpy(decryptedKey, masterkey, AES_KEY_SIZE_BYTES);
			/*
			 printf("Stored plain Key: \n");
			 printAESKey(decryptedKey);
			 */
			return (TEEC_SUCCESS);
		}

		tries++;
		if (tries > 3) {
			return (TEEC_ERROR_SECURITY);
		}
	}

	return (TEEC_ERROR_SECURITY);
}

TEE_Result sample_encrypt(uint32_t paramTypes, TEE_Param params[4]) {
	TEE_Result res;
	char key_file_name[20];
	uint8_t masterkey[AES_KEY_SIZE_BYTES];
	aes_context aes;
	uint8_t iv[16];
	uint8_t* enc_data;
	crypt_pack_t* pack;
	size_t olen;
	uint8_t* ptr;
	size_t ptrlen = 0;

	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_VALUE_INPUT, // key_id [in]
			TEEC_MEMREF_WHOLE,// plain blob [in]
			TEEC_MEMREF_WHOLE,// crypt_pack_t [out]
			TEEC_NONE)) {
		printf("SAMPLE-TRUSTLET: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	if (params[1].memref.size % 16) {
		printf("SAMPLE-TRUSTLET: Invalid buffer size (padding has to be done "
				"in normal world)!\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	pack = params[2].memref.buffer;

	if ((params[1].memref.size) != pack->datasize) {
		printf("SAMPLE-TRUSTLET: Output buffer too small\n");
		printf("SAMPLE-TRUSTLET: PLAIN %d < OUTPUT SIZE %d\n",
				params[1].memref.size, pack->datasize);
		/* Bad parameter types */
		return (TEE_ERROR_SHORT_BUFFER);
	}

	memset(key_file_name, 0, 20);

	snprintf(key_file_name, 20, "key_%X", params[0].value.a);

	res = read_key(key_file_name, masterkey, params[0].value.a);

	if (res == TEEC_SUCCESS) {
		platform_pseudo_entropy(NULL, pack->iv, 16, &olen);

		memcpy(iv, pack->iv, 16);

		enc_data = malloc(pack->datasize);

		if (enc_data == NULL ) {
			printf("malloc for enc data failed!\n");
			return (TEEC_ERROR_OUT_OF_MEMORY);
		}
		/*
		 printf("Plaintext to encrypt:\n");
		 ptr = params[1].memref.buffer;

		 for (ptrlen = 0; ptrlen < params[1].memref.size; ptrlen = ptrlen + 16) {
		 if (ptrlen == 0) {
		 ptr = params[1].memref.buffer;
		 } else {
		 ptr = ptr + 16;
		 }
		 printIVKey(ptr);
		 }

		 printf("Generated random IV:\n");
		 printIVKey(iv);
		 */
		// Encrypt payload
		aes_setkey_enc(&aes, masterkey, AES_KEY_SIZE_BITS);

		aes_crypt_cbc(&aes, AES_ENCRYPT, params[1].memref.size, iv,
				params[1].memref.buffer, enc_data);

		memcpy(pack->data, enc_data, pack->datasize);
		/*
		 ptr = pack->data;

		 printf("Encrypted Data:\n");

		 for (ptrlen = 0; ptrlen < params[1].memref.size; ptrlen = ptrlen + 16) {
		 if (ptrlen == 0) {
		 ptr = pack->data;
		 } else {
		 ptr = ptr + 16;
		 }
		 printIVKey(ptr);
		 }
		 */
		printf("Encryption done!\n");
		free(enc_data);

		res = TEEC_SUCCESS;
	}

	return (res);
}

TEE_Result sample_decrypt(uint32_t paramTypes, TEE_Param params[4]) {
	TEE_Result res;
	char key_file_name[20];
	uint8_t masterkey[AES_KEY_SIZE_BYTES];
	uint8_t iv[16];
	aes_context aes;
	crypt_pack_t* pack;
	uint8_t* ptr;
	size_t ptrlen = 0;

	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_VALUE_INPUT, // key_id [in]
			TEEC_MEMREF_WHOLE,// crypt_pack_t [in]
			TEEC_MEMREF_WHOLE,// plain blob [out]
			TEEC_NONE)) {
		printf("SAMPLE-TRUSTLET: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	pack = params[1].memref.buffer;

	if (pack->datasize % 16) {
		printf("SAMPLE-TRUSTLET: Invalid buffer size (padding has to be done "
				"in normal world)!\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	if (pack->datasize > params[2].memref.size) {
		printf("SAMPLE-TRUSTLET: Output buffer too small\n");
		printf("SAMPLE-TRUSTLET: IN: %d OUT: %d\n", pack->datasize,
				params[2].memref.size);
		/* Bad parameter types */
		return (TEE_ERROR_SHORT_BUFFER);
	}

	memset(key_file_name, 0, 20);

	snprintf(key_file_name, 20, "key_%X", params[0].value.a);

	res = read_key(key_file_name, masterkey, params[0].value.a);

	if (res == TEEC_SUCCESS) {

		//printf("decrypting %d bytes\n", pack->datasize);

		// Decrypt payload
		aes_setkey_dec(&aes, masterkey, AES_KEY_SIZE_BITS);

		memcpy(iv, pack->iv, 16);

		aes_crypt_cbc(&aes, AES_DECRYPT, pack->datasize, iv, pack->data,
				params[2].memref.buffer);

		printf("Decryption done!\n");

		res = TEEC_SUCCESS;
	}

	return (res);
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

TEE_Result test_tmp(uint32_t paramTypes, TEE_Param params[4]) {
	if (paramTypes != TEE_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INOUT, // key_id [in]
			TEEC_MEMREF_TEMP_INPUT,// plain blob [in]
			TEEC_MEMREF_TEMP_OUTPUT,// crypt_pack_t [out]
			TEEC_NONE)) {
		printf("SAMPLE-TRUSTLET: Bad Parameters\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	if (params[1].memref.size != params[2].memref.size) {
		printf("SAMPLE-TRUSTLET: Size differs\n");
		/* Bad parameter types */
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	unsigned char* sbuffer = params[0].memref.buffer;
	unsigned char* dbuffer = params[0].memref.buffer;
	int i = 0;

	for (i = 0; i < params[0].memref.size; i++) {
		dbuffer[i] = ~(sbuffer[i]);
	}

	sbuffer = params[1].memref.buffer;
	dbuffer = params[2].memref.buffer;

	for (i = 0; i < params[1].memref.size; i++) {
		dbuffer[i] = ~(sbuffer[i]);
	}

	return (TEE_SUCCESS);
}

TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		__uint32_t_ commandID, __uint32_t_ paramTypes, TEE_Param params[4]) {

	printf("SAMPLE-TRUSTLET: Parameter Types 0x%x\n", paramTypes);
	printf("SAMPLE-TRUSTLET: Command invoked 0x%x\n", commandID);
	switch (commandID) {
	case TZ_NEW_KEY:
		//return test_entropy();
		return (sample_new_key(paramTypes, params));
	case TZ_ENCRYPT:
		return (sample_encrypt(paramTypes, params));
	case TZ_DECRYPT:
		return (sample_decrypt(paramTypes, params));
	case TZ_TMP_TEST:
		return (test_tmp(paramTypes, params));
	default:
		return (TEE_ERROR_NOT_SUPPORTED);
	}
}
