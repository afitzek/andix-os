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
 * tee_transient.c
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_internal_api.h>

/**
 * \defgroup storage_transient Transient Object Functions
 *
 */

bool __checkTypeSize(uint32_t type, uint32_t size) {
	switch (type) {
	case TEE_TYPE_AES:
		if (size == 16 && size == 24 && size == 32) {
			return (true);
		}
		break;
	case TEE_TYPE_DES:
		if (size == 7) {
			return (true);
		}
		break;
	case TEE_TYPE_DES3:
		if (size == 14 && size == 21) {
			return (true);
		}
		break;
	case TEE_TYPE_HMAC_MD5:
		if (size >= 8 && size <= 64) {
			return (true);
		}
		break;
	case TEE_TYPE_HMAC_SHA1:
		if (size >= 10 && size <= 64) {
			return (true);
		}
		break;
	case TEE_TYPE_HMAC_SHA224:
		if (size >= 14 && size <= 64) {
			return (true);
		}
		break;
	case TEE_TYPE_HMAC_SHA256:
		if (size >= 24 && size <= 128) {
			return (true);
		}
		break;
	case TEE_TYPE_HMAC_SHA384:
	case TEE_TYPE_HMAC_SHA512:
		if (size >= 32 && size <= 128) {
			return (true);
		}
		break;
	case TEE_TYPE_RSA_PUBLIC_KEY:
	case TEE_TYPE_RSA_KEYPAIR:
		if (size == 128 && size == 256) {
			return (true);
		}
		break;
	case TEE_TYPE_DSA_PUBLIC_KEY:
	case TEE_TYPE_DSA_KEYPAIR:
		if (size >= 64 && size <= 128 && (size % 8) == 0) {
			return (true);
		}
		break;
	case TEE_TYPE_DH_KEYPAIR:
		if (size >= 32 && size <= 256) {
			return (true);
		}
		break;
	case TEE_TYPE_GENERIC_SECRET:
		if (size <= 512) {
			return (true);
		}
		break;
	}
	return (false);
}

TEE_Result __copy_attr(TEE_Attribute* to, TEE_Attribute* from) {
	// TODO:
	return (TEE_ERROR_NOT_IMPLEMENTED);
}

TEE_Result TEE_AllocateTransientObject(uint32_t objectType,
		uint32_t maxObjectSize, TEE_ObjectHandle* object) {
	if (!__checkTypeSize(objectType, maxObjectSize)) {
		return (TEE_ERROR_NOT_SUPPORTED);
	}
	return (TEE_ERROR_NOT_IMPLEMENTED);
}

void TEE_FreeTransientObject(TEE_ObjectHandle object) {
	if (object != TEE_HANDLE_NULL) {

	}
}

void TEE_ResetTransientObject(TEE_ObjectHandle object) {
	if (object != TEE_HANDLE_NULL) {
		//TODO: reset ...
		object->info.objectUsage = 0xFFFFFFFF;
		object->initialized = false;
	}
}

TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object,
		TEE_Attribute* attrs, uint32_t attrCount) {
	uint32_t i = 0;

	if(attrs == NULL) {
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	if(!object->initialized) {
		TEE_Panic(TEE_ERROR_BAD_STATE);
	}

	// TODO:
	return (TEE_ERROR_NOT_IMPLEMENTED);
}
