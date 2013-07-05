/*
 * tee_crypto_digest.c
 *
 *  Created on: Feb 27, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_internal_api.h>


/**
 * \defgroup cryptdigest Message Digest Functions
 *
 */

// ============================================================================
// Message Digest Functions
// ============================================================================

void __validate_digest_operation(TEE_OperationHandle operation) {

}

void TEE_DigestUpdate(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize)
{

}

TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize, void* hash, size_t *hashLen)
{
	return (TEE_SUCCESS);
}

// ============================================================================

