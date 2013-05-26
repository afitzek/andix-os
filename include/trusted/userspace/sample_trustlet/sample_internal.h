/*
 * sample_internal.h
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#ifndef SAMPLE_INTERNAL_H_
#define SAMPLE_INTERNAL_H_

#include <tee_internal_api.h>

#define AES_KEY_SIZE_BITS 256
#define AES_KEY_SIZE_BYTES 32
#define HASH_BYTES 32

typedef struct {
	uint8_t	crypted_iv[16];
	uint8_t crypted_key[AES_KEY_SIZE_BYTES];
	uint8_t hash[HASH_BYTES];
} keystore_t;

#endif /* SAMPLE_INTERNAL_H_ */
