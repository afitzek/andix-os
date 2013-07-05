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

int pbkdf2_sha1_hmac(const unsigned char *password, unsigned int plen,
                 const unsigned char *salt, unsigned int slen,
                 unsigned int iteration_count,
                 unsigned int key_length, unsigned char *output);

int pbkdf2_sha256_hmac(const unsigned char *password, unsigned int plen,
		const unsigned char *salt, unsigned int slen, unsigned int iteration_count,
		int key_length, unsigned char *output);

int pbkdf2_sha224_hmac(const unsigned char *password, unsigned int plen,
		const unsigned char *salt, unsigned int slen, unsigned int iteration_count,
		unsigned int key_length, unsigned char *output);

#endif /* PBKDF2_H_ */
