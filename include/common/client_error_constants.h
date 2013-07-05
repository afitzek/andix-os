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
 * client_error_constants.h
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#ifndef CLIENT_ERROR_CONSTANTS_H_
#define CLIENT_ERROR_CONSTANTS_H_

#ifndef TEEC_ERROR_CONSTANT
#define TEEC_ERROR_CONSTANT

/** The operation was successful. */
#define TEEC_SUCCESS 0x00000000

/** Non-specific cause.*/
#define TEEC_ERROR_GENERIC 0xFFFF0000

/**  Access privileges are not sufficient.*/
#define TEEC_ERROR_ACCESS_DENIED 0xFFFF0001

/** The operation was cancelled.*/
#define TEEC_ERROR_CANCEL 0xFFFF0002

/** Concurrent accesses caused conflict.*/
#define TEEC_ERROR_ACCESS_CONFLICT 0xFFFF0003

/** Too much data for the requested operation was passed. */
#define TEEC_ERROR_EXCESS_DATA 0xFFFF0004

/** Input data was of invalid format. */
#define TEEC_ERROR_BAD_FORMAT 0xFFFF0005

/** Input parameters were invalid. */
#define TEEC_ERROR_BAD_PARAMETERS 0xFFFF0006

/** Operation is not valid in the current state. */
#define TEEC_ERROR_BAD_STATE 0xFFFF0007

/** The requested data item is not found. */
#define TEEC_ERROR_ITEM_NOT_FOUND 0xFFFF0008

/** The requested operation should exist but is not yet implemented.*/
#define TEEC_ERROR_NOT_IMPLEMENTED 0xFFFF0009

/** The requested operation is valid but is not supported in this Implementation.*/
#define TEEC_ERROR_NOT_SUPPORTED 0xFFFF000A

/** Expected data was missing. */
#define TEEC_ERROR_NO_DATA 0xFFFF000B

/** System ran out of resources. */
#define TEEC_ERROR_OUT_OF_MEMORY 0xFFFF000C

/** The system is busy working on something else.  */
#define TEEC_ERROR_BUSY 0xFFFF000D

/** Communication with a remote party failed.  */
#define TEEC_ERROR_COMMUNICATION 0xFFFF000E

/** A security fault was detected.  */
#define TEEC_ERROR_SECURITY 0xFFFF000F

/** The supplied buffer is too short for the generated output.  */
#define TEEC_ERROR_SHORT_BUFFER 0xFFFF0010

#endif

/** The return code is an error that originated
 * within the TEE Client API implementation. */
#define TEEC_ORIGIN_API 0x00000001

/** The return code is an error that originated within
 * the underlying communications stack linking the rich OS with the TEE. */
#define TEEC_ORIGIN_COMMS 0x00000002

/** The return code is an error that originated within the common TEE code. */
#define TEEC_ORIGIN_TEE 0x00000003

/** The return code originated within the Trusted
 * Application code. This includes the case where
 * the return code is a success. */
#define TEEC_ORIGIN_TRUSTED_APP 0x00000004


/** No login data is provided.*/
#define TEEC_LOGIN_PUBLIC 0x00000000

/** Login data about the user running the
	Client Application process is provided.*/
#define TEEC_LOGIN_USER 0x00000001

/** Login data about the group running
	the Client Application process is
	provided.*/
#define TEEC_LOGIN_GROUP 0x00000002

/** Login data about the running Client
	Application itself is provided.*/
#define TEEC_LOGIN_APPLICATION 0x00000004

/** Login data about the user running the
	Client Application and about the
	Client Application itself is provided.*/
#define TEEC_LOGIN_USER_APPLICATION 0x00000005

/** Login data about the group running
	the Client Application and about the
	Client Application itself is provided.*/
#define TEEC_LOGIN_GROUP_APPLICATION 0x00000006


#endif /* CLIENT_ERROR_CONSTANTS_H_ */
