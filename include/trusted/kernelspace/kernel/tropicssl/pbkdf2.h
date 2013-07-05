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
 * pbkdf2.h
 *
 *  Created on: Jul 5, 2013
 *      Author: Andreas Fitzek
 */

#ifndef PBKDF2_H_
#define PBKDF2_H_

/**
 * \addtogroup crypto
 *	\{
 */

#include <tropicssl/sha1.h>
#include <tropicssl/sha2.h>
#include <common.h>

/**
 * PBKDF2 Implementation using SHA1
 * @param password The password
 * @param plen The password len
 * @param salt The salt
 * @param slen The salt len
 * @param iteration_count Iteration count
 * @param key_length The key len
 * @param output The output key
 */
int pbkdf2_sha1_hmac(const unsigned char *password, size_t plen,
                 const unsigned char *salt, size_t slen,
                 unsigned int iteration_count,
                 uint32_t key_length, unsigned char *output);

/**
 * PBKDF2 Implementation using SHA256
 * @param password The password
 * @param plen The password len
 * @param salt The salt
 * @param slen The salt len
 * @param iteration_count Iteration count
 * @param key_length The key len
 * @param output The output key
 */
int pbkdf2_sha256_hmac(const unsigned char *password, size_t plen,
		const unsigned char *salt, size_t slen, unsigned int iteration_count,
		int32_t key_length, unsigned char *output);

/**
 * PBKDF2 Implementation using SHA224
 * @param password The password
 * @param plen The password len
 * @param salt The salt
 * @param slen The salt len
 * @param iteration_count Iteration count
 * @param key_length The key len
 * @param output The output key
 */
int pbkdf2_sha224_hmac(const unsigned char *password, size_t plen,
		const unsigned char *salt, size_t slen, unsigned int iteration_count,
		uint32_t key_length, unsigned char *output);


/* \} group */

#endif /* PBKDF2_H_ */
