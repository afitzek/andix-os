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
