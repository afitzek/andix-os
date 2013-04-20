/*
 * tee_storage.h
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_STORAGE_H_
#define TEE_STORAGE_H_

#include <storage/tee_transient.h>

/**
 * \addtogroup storage
 *	\{
 */

/**
 * \addtogroup storage_generic
 *	\{
 */

/**
 * Fills the info object according to the object handle
 * @param object The object handle
 * @param objectInfo The object information
 */
void TEE_GetObjectInfo(TEE_ObjectHandle object, TEE_ObjectInfo* objectInfo);

/**
 * Restricts the usage of the object to most of flags of objectUsage
 *
 * usage = usage & objectUsage;
 *
 * @param object The object handle
 * @param objectUsage The usage flags
 */
void TEE_RestrictObjectUsage(TEE_ObjectHandle object, uint32_t objectUsage);

/**
 * Get an buffer attribute from the object
 *
 * (Not implemented!)
 *
 * @param object The object handle
 * @param attributeID The attribute ID
 * @param buffer The buffer
 * @param size The size of the buffer
 *
 * @return TEE_SUCCESS | TEE_ERROR_ITEM_NOT_FOUND | TEE_ERROR_SHORT_BUFFER
 */
TEE_Result TEE_GetObjectBufferAttribute(TEE_ObjectHandle object,
		uint32_t attributeID, void* buffer, size_t* size);

/**
 * Get value attribute from an object
 *
 * @param object The object handle
 * @param attributeID The attribute ID
 * @param buffer The buffer
 * @param size The size of the buffer
 *
 * @return TEE_SUCCESS | TEE_ERROR_ITEM_NOT_FOUND | TEE_ERROR_SHORT_BUFFER
 */
TEE_Result TEE_GetObjectValueAttribute(TEE_ObjectHandle object,
		uint32_t attributeID, uint32_t* a, uint32_t* b);

/**
 * Closes the object handle
 *
 * @param object The object handle
 */
void TEE_CloseObject(TEE_ObjectHandle object);

/* \} group */

/* \} group */

#endif /* TEE_STORAGE_H_ */
