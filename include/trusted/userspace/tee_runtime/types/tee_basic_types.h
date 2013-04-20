/*
 * tee_basic_types.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_BASIC_TYPES_H_
#define TEE_BASIC_TYPES_H_

#include <stdint.h>
#include <string.h>

/**
 * boolean datatype
 */
typedef int8_t			bool;

/**
 * true value
 */
#define true 			(1)

/**
 * false value
 */
#define false			(0)

#ifndef __TEE_RESULT__
#define __TEE_RESULT__
/**
 * Result value
 */
typedef uint32_t		TEE_Result;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif


/**
 * Result value client compatible
 */
typedef TEE_Result		TEEC_Result;

#ifndef __TEE_UUID__
#define __TEE_UUID__
/**
 * Universally Unique Resource Identifier
 */
typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEE_UUID;
#endif

typedef TEE_UUID TEEC_UUID;

#endif /* TEE_BASIC_TYPES_H_ */
