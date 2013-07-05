/*
 * pbkdf2.h
 *
 *  Created on: Jul 5, 2013
 *      Author: Andreas Fitzek
 */

#ifndef PBKDF2_H_
#define PBKDF2_H_

#include <tropicssl/sha1.h>
#include <tropicssl/sha2.h>
#include <common.h>

int pbkdf2_sha1_hmac(const unsigned char *password, size_t plen,
                 const unsigned char *salt, size_t slen,
                 unsigned int iteration_count,
                 uint32_t key_length, unsigned char *output);

int pbkdf2_sha256_hmac(const unsigned char *password, size_t plen,
		const unsigned char *salt, size_t slen, unsigned int iteration_count,
		int32_t key_length, unsigned char *output);

int pbkdf2_sha224_hmac(const unsigned char *password, size_t plen,
		const unsigned char *salt, size_t slen, unsigned int iteration_count,
		uint32_t key_length, unsigned char *output);

#endif /* PBKDF2_H_ */
