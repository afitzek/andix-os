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
 * tee_error_constants.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_ERROR_CONSTANTS_H_
#define TEE_ERROR_CONSTANTS_H_

#define TEE_SUCCESS 				(0x00000000)
#define TEE_ERROR_GENERIC 			(0xFFFF0000)
#define TEE_ERROR_ACCESS_DENIED  	(0xFFFF0001)
#define TEE_ERROR_CANCEL  			(0xFFFF0002)
#define TEE_ERROR_ACCESS_CONFLICT  	(0xFFFF0003)
#define TEE_ERROR_EXCESS_DATA  		(0xFFFF0004)
#define TEE_ERROR_BAD_FORMAT  		(0xFFFF0005)
#define TEE_ERROR_BAD_PARAMETERS  	(0xFFFF0006)
#define TEE_ERROR_BAD_STATE  		(0xFFFF0007)
#define TEE_ERROR_ITEM_NOT_FOUND  	(0xFFFF0008)
#define TEE_ERROR_NOT_IMPLEMENTED  	(0xFFFF0009)
#define TEE_ERROR_NOT_SUPPORTED  	(0xFFFF000A)
#define TEE_ERROR_NO_DATA  			(0xFFFF000B)
#define TEE_ERROR_OUT_OF_MEMORY  	(0xFFFF000C)
#define TEE_ERROR_BUSY  			(0xFFFF000D)
#define TEE_ERROR_COMMUNICATION  	(0xFFFF000E)
#define TEE_ERROR_SECURITY  		(0xFFFF000F)
#define TEE_ERROR_SHORT_BUFFER  	(0xFFFF0010)
#define TEE_PENDING 				(0xFFFF2000)
#define TEE_ERROR_TIMEOUT 			(0xFFFF3001)
#define TEE_ERROR_OVERFLOW 			(0xFFFF300F)
#define TEE_ERROR_TARGET_DEAD 		(0xFFFF3024)
#define TEE_ERROR_STORAGE_NO_SPACE 	(0xFFFF3041)
#define TEE_ERROR_MAC_INVALID		(0xFFFF3071)
#define TEE_ERROR_SIGNATURE_INVALID (0xFFFF3072)
#define TEE_ERROR_TIME_NOT_SET 		(0xFFFF5000)
#define TEE_ERROR_TIME_NEEDS_RESET 	(0xFFFF5001)


#endif /* TEE_ERROR_CONSTANTS_H_ */
