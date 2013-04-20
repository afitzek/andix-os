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
	if(operation != NULL) {
		if(operation->info.mode == TEE_MODE_DIGEST) {
			if(operation->md_context != NULL) {
				if(operation->md_context->md_info != NULL) {
					return;
				}
			}
		}
	}
	TEE_Panic(TEE_ERROR_BAD_PARAMETERS);
}

void TEE_DigestUpdate(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize)
{
	__validate_digest_operation(operation);

	if(chunk == NULL) {
		return;
	}

	if(md_update(operation->md_context, chunk, chunkSize) != 0) {
		TEE_Panic(TEE_ERROR_BAD_PARAMETERS);
	}
}

TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, void* chunk,
		size_t chunkSize, void* hash, size_t *hashLen)
{
	if(chunk != NULL) {
		TEE_DigestUpdate(operation, chunk, chunkSize);
	} else {
		__validate_digest_operation(operation);
	}

	if(operation->md_context->md_info->size < (*hashLen)) {
		return (TEE_ERROR_SHORT_BUFFER);
	}

	if(md_finish(operation->md_context, hash) != 0) {
		TEE_Panic(TEE_ERROR_BAD_PARAMETERS);
	}

	(*hashLen) = operation->md_context->md_info->size;

	return (TEE_SUCCESS);
}

// ============================================================================

