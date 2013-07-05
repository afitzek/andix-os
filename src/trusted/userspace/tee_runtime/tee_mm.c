/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
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

