/*
 * tee_data_types.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_DATA_TYPES_H_
#define TEE_DATA_TYPES_H_

#include <types/tee_basic_types.h>

/**
 * Defines full client identity
 */
typedef struct {
	uint32_t 	login;
	TEE_UUID	uuid;
} TEE_Identity;

#ifndef __TEE_PARAM__
#define __TEE_PARAM__
/**
 * Parameters for TEE functions
 */
typedef union {
	struct {
		void*	buffer;
		size_t	size;
	} memref;
	struct {
		uint32_t a;
		uint32_t b;
	} value;
} TEE_Param;
#endif

#define TEE_PARAM_TYPES(t0,t1,t2,t3) \
((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12))

#define TEE_PARAM_TYPE_GET(t, i) (((t) >> (i*4)) & 0xF)

struct __TEE_TASessionHandle {
	//TODO: add session handles
};

/**
 * Session handle
 */
typedef struct __TEE_TASessionHandle* TEE_TASessionHandle;

struct __TEE_PropSetHandle {
	//TODO: add prop set handle stuff
};

/**
 * Handle for property set enumerator
 */
typedef struct __TEE_PropSetHandle* TEE_PropSetHandle;


#endif /* TEE_DATA_TYPES_H_ */
