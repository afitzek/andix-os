/*
 * tee_mm.c
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_constants.h>
#include <tee_types.h>
#include <mm/tee_mm.h>
#include <stdlib.h>
#include <string.h>


/**
 * \defgroup mm Memory Management Functions
 *
 */

static void* __global_id_ = NULL;
static uint32_t __global_id_magic = 0;

TEE_Result TEE_CheckMemoryAccessRights( uint32_t accessFlags,
		void* buffer, size_t size) {
	// TODO Implement
	return (TEE_ERROR_NOT_IMPLEMENTED);
}

void TEE_SetInstanceData(void* instanceData ) {
	__global_id_ = instanceData;
	__global_id_magic = __TEE_INSTANCE_GLOBAL_MAGIC;
}

void* TEE_GetInstanceData( void ) {
	if(__global_id_magic == __TEE_INSTANCE_GLOBAL_MAGIC) {
		return (__global_id_);
	}
	return (NULL);
}

void* TEE_Malloc( size_t size, uint32_t hint ) {
	void* buffer = malloc(size);

	if(buffer == NULL) {
		return (NULL);
	}

	if(hint == 0) {
		memset(buffer, 0, size);
	}

	return (buffer);
}

void* TEE_Realloc(void* buffer, uint32_t newSize ) {
	return (realloc(buffer, newSize));
}

void TEE_Free(void *buffer) {
	free(buffer);
}

void TEE_MemMove(void* dest, void* src, uint32_t size) {
	memcpy(dest, src, size);
}

int32_t TEE_MemCompare(void* buffer1, void* buffer2, uint32_t size) {
	return (memcmp(buffer1, buffer2, size));
}

void TEE_MemFill(void* buffer, uint32_t x, uint32_t size) {
	mmemset(buffer, x, size);
}

