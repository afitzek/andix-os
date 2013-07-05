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
 * tee_crypto.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_CRYPTO_H_
#define TEE_CRYPTO_H_

/**
 * \addtogroup crypt
 *	\{
 */

#include <tee_types.h>

/**
 * \addtogroup cryptgeneric
 *	\{
 */

/**
 * allocates a handle for a new cryptographic operation
 *
 * @param operation Reference to generated operation handle
 * @param algorithm One of the cipher algorithms
 * @param mode The operation mode
 * @param maxKeySize Maximum key size in bits for the operation
 * @return TEE_SUCCESS | TEE_ERROR_OUT_OF_MEMORY | TEE_ERROR_NOT_SUPPORTED
 */
TEE_Result TEE_AllocateOperation(
		TEE_OperationHandle* operation,
		uint32_t algorithm,
		uint32_t mode,
		uint32_t maxKeySize
);

/**
 * Frees a handle for a cryptographic operation
 * @param operation Reference to operation handle
 */
void TEE_FreeOperation(TEE_OperationHandle operation);

/**
 * Get operation Information for operation handle
 * @param operation The operation handle
 * @param operationInfo Information about the handle
 */
void TEE_GetOperationInfo(TEE_OperationHandle operation,
		TEE_OperationInfo* operationInfo);

/* \} group */

/**
 * \addtogroup cryptdigest
 *	\{
 */

/**
 * Accumulates message data for hashing
 *
 * @param operation The operation handle
 * @param chunk Chunk data to be hashed
 * @param chunkSize Size of chunk data
 */
void TEE_DigestUpdate(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize);

/**
 *
 * @param operation The operation handle
 * @param chunk Last Chunk data to be hashed
 * @param chunkSize Size of chunk data
 * @param hash The outputbuffer
 * @param hashLen The buffer size afterwards the hash length
 * @return TEE_SUCCESS | TEE_ERROR_SHORT_BUFFER
 */
TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize, void* hash, size_t *hashLen);

/* \} group */

/* \} group */

#endif /* TEE_CRYPTO_H_ */
