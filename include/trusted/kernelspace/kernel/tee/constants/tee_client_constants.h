/*
 * tee_client_constants.h
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#ifndef TEE_CLIENT_CONSTANTS_H_
#define TEE_CLIENT_CONSTANTS_H_


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


#endif /* TEE_CLIENT_CONSTANTS_H_ */
