/*
 * crypt.h
 *
 *  Created on: Jan 20, 2014
 *      Author: andy
 */

#ifndef CRYPT_H_
#define CRYPT_H_

#include <tee_client_api.h>

#define AES_KEY_SIZE_BITS 256
#define AES_KEY_SIZE_BYTES 32
#define HASH_BYTES 32

typedef struct {
	uint8_t	crypted_iv[16];
	uint8_t crypted_key[AES_KEY_SIZE_BYTES];
	uint8_t hash[HASH_BYTES];
} keystore_t;

TEEC_Result TA_InvokeCommandEntryPoint(void* sessionContext,
		uint32_t commandID, uint32_t paramTypes, TEEC_Parameter params[4]);

#endif /* CRYPT_H_ */
