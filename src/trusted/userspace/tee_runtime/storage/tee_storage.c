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
 * tee_storage.c
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_internal_api.h>

/**
 * \defgroup storage Trusted Storage API
 *
 */

/**
 * \defgroup storage_generic Generic Object Functions
 *
 */

/**
 * Fills the info object according to the object handle
 * @param object The object handle
 * @param objectInfo The object information
 */
void TEE_GetObjectInfo(TEE_ObjectHandle object, TEE_ObjectInfo* objectInfo) {
	memcpy((void*)objectInfo, (void*)&(object->info), sizeof(TEE_ObjectInfo));
}

/**
 * Restricts the usage of the object to most of flags of objectUsage
 *
 * usage = usage & objectUsage;
 *
 * @param object The object handle
 * @param objectUsage The usage flags
 */
void TEE_RestrictObjectUsage(TEE_ObjectHandle object, uint32_t objectUsage) {
	//TODO
}

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
		uint32_t attributeID, void* buffer, size_t* size) {
	return (TEE_ERROR_NOT_IMPLEMENTED);
}

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
		uint32_t attributeID, uint32_t* a, uint32_t* b) {
	return (TEE_ERROR_NOT_IMPLEMENTED);
}

/**
 * Closes the object handle
 *
 * @param object The object handle
 */
void TEE_CloseObject(TEE_ObjectHandle object) {
	//TODO
}

