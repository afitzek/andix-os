/*
 * tee_mm.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_MM_H_
#define TEE_MM_H_

#include <tee_types.h>

/**
 * \addtogroup mm
 *	\{
 */

#define __TEE_INSTANCE_GLOBAL_MAGIC 0x19271927

/**
 * Checks if the Application is compatible with the accessFlags
 *
 * @param accessFlags The accessFlags to check
 * @param buffer The buffer
 * @param size The buffer size
 * @return TEE_SUCCESS OR TEE_ERROR_ACCESS_DENIED.
 */
TEE_Result TEE_CheckMemoryAccessRights( uint32_t accessFlags,
		void* buffer, size_t size);


/**
 * Sets a global data pointer
 * @param instanceData The instance data
 */
void TEE_SetInstanceData(void* instanceData );

/**
 * Gets the global data pointer
 * @return NULL or the data set by TEE_SetInstanceData
 */
void* TEE_GetInstanceData( void );

/**
 * Allocates a memory buffer with size
 * @param size The size of the memory buffer
 * @param hint The hint (no meaning till now)
 */
void* TEE_Malloc( size_t size, uint32_t hint );

/**
 * Changes the size of the memory buffer
 * @param buffer The pointer to the object to be reallocated
 * @param newSize The new size required for the object
 */
void* TEE_Realloc(void* buffer, uint32_t newSize );

/**
 * Release memory region
 * @param buffer The pointer to the object to be freed
 */
void TEE_Free(void *buffer);

/**
 * Copies size bytes from src to dest
 * @param dest A pointer to the destination buffer
 * @param src A pointer to the source buffer
 * @param size The number of bytes to be copied
 */
void TEE_MemMove(void* dest, void* src, uint32_t size);

/**
 * The TEE_MemCompare function compares the first size bytes of the
 * object pointed to by buffer1 to the first size
 * bytes of the object pointed to by buffer2.
 * @param buffer1 A pointer to the first buffer
 * @param buffer2 A pointer to the second buffer
 * @param size The number of bytes to be compared
 * @return The sign of a non-zero return value is determined
 * by the sign of the difference between the values of the first pair
 * of bytes (both interpreted as type uint8_t) that differ in
 * the objects being compared.
 */
int32_t TEE_MemCompare(void* buffer1, void* buffer2, uint32_t size);

/**
 * The TEE_MemFill function writes the byte x (converted to a uint8_t)
 * into the first size bytes of the object pointed to by buffer.
 * @param buffer A pointer to the destination buffer
 * @param x The value to be set
 * @param size The number of bytes to be set
 */
void TEE_MemFill(void* buffer, uint32_t x, uint32_t size);

/* \} group */


#endif /* TEE_MM_H_ */
