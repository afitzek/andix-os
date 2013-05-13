/*
 * client_constants.h
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#ifndef CLIENT_CONSTANTS_H_
#define CLIENT_CONSTANTS_H_


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

/** The Shared Memory can carry data from the Client
	Application to the Trusted Application. */
#define TEEC_MEM_INPUT 0x00000001

/** The Shared Memory can carry data from the
	Trusted Application to the Client Application. */
#define TEEC_MEM_OUTPUT 0x00000002

/** The Parameter is not used */
#define TEEC_NONE 0x00000000
/** The Parameter is a TEEC_Value tagged as input.*/
#define TEEC_VALUE_INPUT 0x00000001

/** The Parameter is a TEEC_Value tagged as output.*/
#define TEEC_VALUE_OUTPUT 0x00000002

/** The Parameter is a TEEC_Value tagged
	as both as input and output, i.e., for which
	both the behaviors of TEEC_VALUE_INPUT
	and TEEC_VALUE_OUTPUT apply.*/
#define TEEC_VALUE_INOUT 0x00000003

/** The Parameter is a
	TEEC_TempMemoryReference describing
	a region of memory which needs to be
	temporarily registered for the duration of
	the Operation and is tagged as input.*/
#define TEEC_MEMREF_TEMP_INPUT 0x00000005

/** Same as TEEC_MEMREF_TEMP_INPUT,
	but the Memory Reference is tagged as
	output. The Implementation may update
	the size field to reflect the required output
	size in some use cases.*/
#define TEEC_MEMREF_TEMP_OUTPUT 0x00000006

/** A Temporary Memory Reference tagged as
	both input and output, i.e., for which both
	the behaviors of
	TEEC_MEMREF_TEMP_INPUT and
	TEEC_MEMREF_TEMP_OUTPUT apply.*/
#define TEEC_MEMREF_TEMP_INOUT 0x00000007

/** The Parameter is a Registered Memory
	Reference that refers to the entirety of its
	parent Shared Memory block. The
	parameter structure is a
	TEEC_MemoryReference. In this
	structure, the Implementation MUST read
	only the parent field and MAY update the
	size field when the operation completes.*/
#define TEEC_MEMREF_WHOLE 0x0000000C


/** A Registered Memory Reference structure
	that refers to a partial region of its parent
	Shared Memory block and is tagged as
	input.*/
#define TEEC_MEMREF_PARTIAL_INPUT 0x0000000D

/** A Registered Memory Reference structure
	that refers to a partial region of its parent
	Shared Memory block and is tagged as
	output.*/
#define TEEC_MEMREF_PARTIAL_OUTPUT 0x0000000E

/** The Registered Memory Reference
	structure that refers to a partial region of its
	parent Shared Memory block and is tagged
	as both input and output, i.e., for which
	both the behaviors of
	TEEC_MEMREF_PARTIAL_INPUT and
	TEEC_MEMREF_PARTIAL_OUTPUT apply.*/
#define TEEC_MEMREF_PARTIAL_INOUT 0x0000000F


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

#endif /* CLIENT_CONSTANTS_H_ */
