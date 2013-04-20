/*
 * tee_crypto.c
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

/**
 * \defgroup crypt Cryptographic Operations API
 *
 */

/**
 * \defgroup cryptgeneric Generic Cryptographic Functions
 *
 */

#include <stdlib.h>
#include <tee_internal_api.h>

/**
 * Returns true if algorithm and mode are allowed together
 */
bool __check_alg_mode(uint32_t algorithm, uint32_t mode) {
	if (mode == TEE_MODE_ENCRYPT || mode == TEE_MODE_DECRYPT) {
		switch (algorithm) {
		case TEE_ALG_AES_ECB_NOPAD:
		case TEE_ALG_AES_CBC_NOPAD:
		case TEE_ALG_AES_CTR:
		case TEE_ALG_AES_CTS:
		case TEE_ALG_AES_XTS:
		case TEE_ALG_AES_CCM:
		case TEE_ALG_AES_GCM:
		case TEE_ALG_DES_ECB_NOPAD:
		case TEE_ALG_DES_CBC_NOPAD:
		case TEE_ALG_DES3_ECB_NOPAD:
		case TEE_ALG_DES3_CBC_NOPAD:
		case TEE_ALG_RSAES_PKCS1_V1_5:
		case TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1:
		case TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224:
		case TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256:
		case TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384:
		case TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512:
		case TEE_ALG_RSA_NOPAD:
			return (true);
		default:
			return (false);
		}
	} else if (mode == TEE_MODE_MAC) {
		switch (algorithm) {
		case TEE_ALG_DES_CBC_MAC_NOPAD:
		case TEE_ALG_AES_CBC_MAC_NOPAD:
		case TEE_ALG_AES_CBC_MAC_PKCS5:
		case TEE_ALG_AES_CMAC:
		case TEE_ALG_DES_CBC_MAC_PKCS5:
		case TEE_ALG_DES3_CBC_MAC_NOPAD:
		case TEE_ALG_DES3_CBC_MAC_PKCS5:
		case TEE_ALG_HMAC_MD5:
		case TEE_ALG_HMAC_SHA1:
		case TEE_ALG_HMAC_SHA224:
		case TEE_ALG_HMAC_SHA256:
		case TEE_ALG_HMAC_SHA384:
		case TEE_ALG_HMAC_SHA512:
			return (true);
		default:
			return (false);
		}
	} else if (mode == TEE_MODE_DIGEST) {
		switch (algorithm) {
		case TEE_ALG_MD5:
		case TEE_ALG_SHA1:
		case TEE_ALG_SHA224:
		case TEE_ALG_SHA256:
		case TEE_ALG_SHA384:
		case TEE_ALG_SHA512:
			return (true);
		default:
			return (false);
		}
	} else if (mode == TEE_MODE_DERIVE&&
	algorithm == TEE_ALG_DH_DERIVE_SHARED_SECRET) {
		return (true);
	} else if (mode == TEE_MODE_SIGN || mode == TEE_MODE_VERIFY) {
		switch (algorithm) {
		case TEE_ALG_RSASSA_PKCS1_V1_5_MD5:
		case TEE_ALG_RSASSA_PKCS1_V1_5_SHA1:
		case TEE_ALG_RSASSA_PKCS1_V1_5_SHA224:
		case TEE_ALG_RSASSA_PKCS1_V1_5_SHA256:
		case TEE_ALG_RSASSA_PKCS1_V1_5_SHA384:
		case TEE_ALG_RSASSA_PKCS1_V1_5_SHA512:
		case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1:
		case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224:
		case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256:
		case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384:
		case TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512:
		case TEE_ALG_DSA_SHA1:
			return (true);
		default:
			return (false);
		}
	}
	return (false);
}

TEE_Result __allocate_digest(TEE_OperationHandle* operation, uint32_t algorithm,
		uint32_t mode, uint32_t maxKeySize) {
	const md_info_t *md_info = NULL;

	TEE_OperationHandle op = malloc(sizeof(struct __TEE_OperationHandle));

	if (op == NULL ) {
		return (TEE_ERROR_OUT_OF_MEMORY);
	}

	memset(op, 0, sizeof(struct __TEE_OperationHandle));

	op->md_context = NULL;
	TEE_Result result;

	switch (algorithm) {
	case TEE_ALG_MD5:
	case TEE_ALG_HMAC_MD5:
		md_info = md_info_from_type(POLARSSL_MD_MD5);
		break;
	case TEE_ALG_SHA1:
	case TEE_ALG_HMAC_SHA1:
		md_info = md_info_from_type(POLARSSL_MD_SHA1);
		break;
	case TEE_ALG_SHA224:
	case TEE_ALG_HMAC_SHA224:
		md_info = md_info_from_type(POLARSSL_MD_SHA224);
		break;
	case TEE_ALG_SHA256:
	case TEE_ALG_HMAC_SHA256:
		md_info = md_info_from_type(POLARSSL_MD_SHA256);
		break;
	case TEE_ALG_SHA384:
	case TEE_ALG_HMAC_SHA384:
		md_info = md_info_from_type(POLARSSL_MD_SHA384);
		break;
	case TEE_ALG_SHA512:
	case TEE_ALG_HMAC_SHA512:
		md_info = md_info_from_type(POLARSSL_MD_SHA512);
		break;
	default:
		md_info = NULL;
		break;
	}

	if (md_info == NULL ) {
		result = TEE_ERROR_NOT_SUPPORTED;
		goto cleanup;
	}

	op->md_context = malloc(sizeof(md_context_t));
	if (op->md_context == NULL ) {
		result = TEE_ERROR_OUT_OF_MEMORY;
		goto cleanup;
	}

	if (md_init_ctx(op->md_context, md_info)) {
		result = TEE_ERROR_NOT_SUPPORTED;
		goto cleanup;
	}

	if (md_starts(op->md_context) != 0) {
		result = TEE_ERROR_NOT_SUPPORTED;
		goto cleanup;
	}

	op->info.algorithm = algorithm;
	op->info.digestLength = md_get_size(md_info);
	op->info.mode = mode;
	op->info.requiredKeyUsage = false;

	(*operation) = op;
	result = TEE_SUCCESS;
	goto done;
	cleanup: if (op != NULL ) {
		if (op->md_context != NULL ) {
			if (op->md_context->md_ctx != NULL ) {
				md_free_ctx(op->md_context);
			}
			free(op->md_context);
			op->md_context = NULL;
		}
		free(op);
	}
	done: return (result);
}

// ============================================================================
// Generic Operation Functions
// ============================================================================

TEE_Result TEE_AllocateOperation(TEE_OperationHandle* operation,
		uint32_t algorithm, uint32_t mode, uint32_t maxKeySize) {

	if (!__check_alg_mode(algorithm, mode)) {
		return (TEE_ERROR_NOT_SUPPORTED);
	}

	if (mode == TEE_MODE_DIGEST || mode == TEE_MODE_MAC) {
		return (__allocate_digest(operation, algorithm, mode, maxKeySize));
	}
	// TODO IMPLEMENT MORE OPERATIONS

	return (TEE_ERROR_NOT_SUPPORTED);
}

void TEE_FreeOperation(TEE_OperationHandle operation) {
	if (operation != NULL ) {
		if (operation->md_context != NULL ) {
			md_free_ctx(operation->md_context);
			free(operation->md_context);
			operation->md_context = NULL;
		}
		free(operation);
	}
}

void TEE_GetOperationInfo(TEE_OperationHandle operation,
		TEE_OperationInfo* operationInfo) {
	memcpy((void*) operationInfo, (const void*) &operation->info,
			sizeof(TEE_OperationInfo));
}

// ============================================================================

