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
 * tee_storage_constants.h
 *
 *  Created on: Mar 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_STORAGE_CONSTANTS_H_
#define TEE_STORAGE_CONSTANTS_H_

/**
 * \addtogroup storage
 *	\{
 */

#define TEE_OBJECT_STORAGE_PRIVATE 0x00000001

#define TEE_DATA_FLAG_ACCESS_READ 0x00000001
#define TEE_DATA_FLAG_ACCESS_WRITE 0x00000002
#define TEE_DATA_FLAG_ACCESS_WRITE_META 0x00000004
#define TEE_DATA_FLAG_SHARE_READ 0x00000010
#define TEE_DATA_FLAG_SHARE_WRITE 0x00000020
#define TEE_DATA_FLAG_CREATE 0x00000200
#define TEE_DATA_FLAG_EXCLUSIVE 0x00000400

#define TEE_USAGE_EXTRACTABLE 0x00000001
#define TEE_USAGE_ENCRYPT 0x00000002
#define TEE_USAGE_DECRYPT 0x00000004
#define TEE_USAGE_MAC 0x00000008
#define TEE_USAGE_SIGN 0x00000010
#define TEE_USAGE_VERIFY 0x00000020
#define TEE_USAGE_DERIVE 0x00000040

#define TEE_HANDLE_NULL (0x0)
#define TEE_HANDLE_FLAG_PERSISTENT 0x00010000
#define TEE_HANDLE_FLAG_INITIALIZED 0x00020000
#define TEE_HANDLE_FLAG_KEY_SET 0x00040000
#define TEE_HANDLE_FLAG_EXPECT_TWO_KEYS 0x00080000

#define TEE_DATA_MAX_POSITION 0xFFFFFFFF
#define TEE_OBJECT_ID_MAX_LEN 64

/* \} group */

#endif /* TEE_STORAGE_CONSTANTS_H_ */
