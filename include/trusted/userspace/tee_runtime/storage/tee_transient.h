/*
 * tee_transient.h
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_TRANSIENT_H_
#define TEE_TRANSIENT_H_

/**
 * \addtogroup storage
 *	\{
 */

/**
 * \addtogroup storage_transient
 *	\{
 */

/**
 * Allocate uninitialized transient object
 *
 * @param objectType The type of the object
 * @param maxObjectSize The maximum object size
 * @param object returns a valid object handle
 *
 * @return TEE_SUCCESS | TEE_ERROR_OUT_OF_MEMORY | TEE_ERROR_NOT_SUPPORTED
 */
TEE_Result TEE_AllocateTransientObject(uint32_t objectType,
		uint32_t maxObjectSize, TEE_ObjectHandle* object);

/**
 * Deallocates a transient object
 *
 * @param object The object handle
 */
void TEE_FreeTransientObject(TEE_ObjectHandle object);

/* \} group */

/* \} group */

#endif /* TEE_TRANSIENT_H_ */
