/*
 * tee_crypto_types.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_CRYPTO_TYPES_H_
#define TEE_CRYPTO_TYPES_H_

/**
 * \addtogroup crypt
 *	\{
 */

/**
 * Operation Mode
 */
typedef enum {
	TEE_MODE_ENCRYPT, //!< TEE_MODE_ENCRYPT
	TEE_MODE_DECRYPT, //!< TEE_MODE_DECRYPT
	TEE_MODE_SIGN,   //!< TEE_MODE_SIGN
	TEE_MODE_VERIFY, //!< TEE_MODE_VERIFY
	TEE_MODE_MAC,    //!< TEE_MODE_MAC
	TEE_MODE_DIGEST, //!< TEE_MODE_DIGEST
	TEE_MODE_DERIVE  //!< TEE_MODE_DERIVE
} TEE_OperationMode;


/**
 * Operation Information
 */
typedef struct {
	uint32_t algorithm;
	uint32_t operationClass;
	uint32_t mode;
	uint32_t digestLength;
	uint32_t maxKeySize;
	uint32_t keySize;
	uint32_t requiredKeyUsage;
	uint32_t handleState;
} TEE_OperationInfo;

struct __TEE_OperationHandle {

};

typedef struct __TEE_OperationHandle* TEE_OperationHandle;

/* \} group */

#endif /* TEE_CRYPTO_TYPES_H_ */
