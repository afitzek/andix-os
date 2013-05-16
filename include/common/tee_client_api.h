/*
 * tee_client_api.h
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#ifndef TEE_CLIENT_API_H_
#define TEE_CLIENT_API_H_

#include <client_constants.h>
#include <client_error_constants.h>

// Data types

/**
 * a 32-bit unsigned integer
 */
typedef unsigned int   uint32_t;

/**
 * a 16-bit unsigned integer
 */
typedef unsigned short uint16_t;

/**
 * an 8-bit unsigned integer
 */
typedef unsigned char  uint8_t;

/**
 * an unsigned integer large enough to hold the size of an object in memory
 */
typedef uint32_t size_t;

/**
 * This type is used to contain return codes which are the results of invoking TEE Client API functions.
 */
typedef uint32_t TEEC_Result;

/**
 * Universally Unique Resource Identifier (UUID)
 */
typedef struct
{
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEEC_UUID;

/**
 * TEE Context
 */
typedef struct
{
	uint32_t id;
} TEEC_Context;

/**
 * TEE Session
 */
typedef struct
{
	uint32_t id;
} TEEC_Session;

/**
 * TEE SharedMemory
 */
typedef struct
{
	void* buffer;
	size_t size;
	uint32_t flags;
	uint32_t memid;
} TEEC_SharedMemory;

/**
 * Temporary Memory Reference
 */
typedef struct
{
	void* buffer;
	size_t size;
} TEEC_TempMemoryReference;

/**
 * Registered MemoryReference
 */
typedef struct
{
	TEEC_SharedMemory* parent;
	size_t size;
	size_t offset;
} TEEC_RegisteredMemoryReference;

/**
 * TEE Value
 */
typedef struct
{
	uint32_t a;
	uint32_t b;
} TEEC_Value;

/**
 * TEE Parameter
 */
typedef union
{
	TEEC_TempMemoryReference tmpref;
	TEEC_RegisteredMemoryReference memref;
	TEEC_Value value;
} TEEC_Parameter;

/**
 * TEE Operation
 */
typedef struct
{
	uint32_t started;
	uint32_t paramTypes;
	TEEC_Parameter params[4];
} TEEC_Operation;

// Data Constants

/**
 * The maximum size of a single Shared
 * Memory block, in bytes, of both API
 * allocated and API registered memory.
 */
#define TEEC_CONFIG_SHAREDMEM_MAX_SIZE (0x80000)


#define ANDIX_TEE_NAME "ANDIX"

// Functions

/**
 * This function initializes a new TEE Context, forming a connection between this Client Application and the
 * TEE identified by the string identifier name.
 *
 * The Client Application MAY pass a NULL name, which means that the Implementation MUST select a
 * default TEE to connect to. The supported name strings, the mapping of these names to a specific TEE,
 * and the nature of the default TEE are implementation-defined.
 *
 * The caller MUST pass a pointer to a valid TEEC Context in context. The Implementation MUST assume
 * that all fields of the TEEC_Context structure are in an undefined state.
 *
 * @param name a zero-terminated string that describes the TEE to connect to. If this parameter is set to
 * NULL the Implementation MUST select a default TEE.
 * @param context a TEEC_Context structure that MUST be initialized by the Implementation.
 *
 * @return TEEC_SUCCESS OR TEEC_ERROR*
 */
TEEC_Result TEEC_InitializeContext(
		const char* name,
		TEEC_Context* context);

/**
 * This function finalizes an initialized TEE Context, closing the connection between the Client Application
 * and the TEE. The Client Application MUST only call this function when all Sessions inside this TEE
 * Context have been closed and all Shared Memory blocks have been released.
 *
 * The implementation of this function MUST NOT be able to fail: after this function returns the Client
 * Application must be able to consider that the Context has been closed.
 * The function implementation MUST do nothing if context is NULL.
 *
 * @param context an initialized TEEC_Context structure which is to be finalized.
 */
void TEEC_FinalizeContext(
	TEEC_Context* context);


/**
 * This function registers a block of existing Client Application memory as a block of Shared Memory within
 * the scope of the specified TEE Context, in accordance with the parameters which have been set by the
 * Client Application inside the sharedMem structure.
 *
 * The parameter context MUST point to an initialized TEE Context.
 * The parameter sharedMem MUST point to the Shared Memory structure defining the memory region to
 * register. The Client Application MUST have populated the following fields of the Shared Memory structure
 * before calling this function:
 *
 * The buffer field MUST point to the memory region to be shared, and MUST not be NULL.
 * The size field MUST contain the size of the buffer, in bytes. Zero is a valid length for a buffer.
 * The flags field indicates the intended directions of data flow between the Client Application and
 * the TEE.
 * The Implementation MUST assume that all other fields in the Shared Memory structure have
 * undefined content.
 *
 * An Implementation MAY put a hard limit on the size of a single Shared Memory block, defined by the
 * constant TEEC_CONFIG_SHAREDMEM_MAX_SIZE. However note that this function may fail to register a
 * block smaller than this limit due to a low resource condition encountered at run-time.
 *
 * @param context a pointer to an initialized TEE Context
 * @param sharedMem a pointer to a Shared Memory structure to register
 *
 * @return TEEC_SUCCESS OR TEEC_ERROR*
 */
TEEC_Result TEEC_RegisterSharedMemory(
		TEEC_Context* context,
		TEEC_SharedMemory* sharedMem);

/**
 * This function allocates a new block of memory as a block of Shared Memory within the scope of the
 * specified TEE Context, in accordance with the parameters which have been set by the Client Application
 * inside the sharedMem structure.
 *
 * The context parameter MUST point to an initialized TEE Context.
 * The sharedMem parameter MUST point to the Shared Memory structure defining the region to allocate.
 * Client Applications MUST have populated the following fields of the Shared Memory structure:
 *
 *
 * The size field MUST contain the desired size of the buffer, in bytes. The size is allowed to be
 * zero. In this case memory is allocated and the pointer written in to the buffer field on return
 * MUST not be NULL but MUST never be de-referenced by the Client Application. In this case
 * however, the Shared Memory block can be used in Registered Memory References.
 * The flags field indicates the allowed directions of data flow between the Client Application and
 * the TEE.
 *
 * The Implementation MUST assume that all other fields in the Shared Memory structure have
 * undefined content.
 *
 * An Implementation MAY put a hard limit on the size of a single Shared Memory block, defined by the
 * constant TEEC_CONFIG_SHAREDMEM_MAX_SIZE. However it must be noted that this function may fail to
 * allocate a block of smaller than this limit due to low resource scenarios encountered at run-time.
 * If this function returns any code other than TEEC_SUCCESS the Implementation MUST have set the
 * buffer field of sharedMem to NULL.
 *
 * @param context a pointer to an initialized TEE Context.
 * @param sharedMem a pointer to a Shared Memory structure to allocate:
 * Before calling this function, the Client Application MUST have set the size, and flags fields.
 * On return, for a successful allocation the Implementation MUST have set the pointer
 * buffer to the address of the allocated block, otherwise it MUST set buffer to NULL.
 *
 * @return TEEC_SUCCESS OR TEEC_ERROR*
 */
TEEC_Result TEEC_AllocateSharedMemory(
		TEEC_Context* context,
		TEEC_SharedMemory* sharedMem);


/**
 * This function deregisters or deallocates a previously initialized block of Shared Memory.
 *
 * For a memory buffer allocated using TEEC_AllocateSharedMemory the Implementation MUST free the
 * underlying memory and the Client Application MUST NOT access this region after this function has been
 * called. In this case the Implementation MUST set the buffer and size fields of the sharedMem
 * structure to NULL and 0 respectively before returning.
 *
 * For memory registered using TEEC_RegisterSharedMemory the Implementation MUST deregister the
 * underlying memory from the TEE, but the memory region will stay available to the Client Application for
 * other purposes as the memory is owned by it.
 *
 * The Implementation MUST do nothing if the sharedMem parameter is NULL.
 *
 * @param sharedMem a pointer to a valid Shared Memory structure.
 */
void TEEC_ReleaseSharedMemory (
		TEEC_SharedMemory* sharedMem);


/**
 * This function opens a new Session between the Client Application and the specified Trusted Application.
 *
 * The Implementation MUST assume that all fields of this session structure are in an undefined state.
 *
 * When this function returns TEEC_SUCCESS the Implementation MUST have populated this structure with
 * any information necessary for subsequent operations within the Session.
 *
 * The target Trusted Application is identified by a UUID passed in the parameter destination.
 *
 * @param context a pointer to an initialized TEE Context.
 * @param session a pointer to a Session structure to open.
 * @param destination a pointer to a structure containing the UUID of the destination Trusted Application.
 * @param connectionMethod the method of connection to use. Refer to section 4.4.5 for more details.
 * @param connectionData any necessary data required to support the connection method chosen.
 * @param operation a pointer to an Operation containing a set of Parameters to exchange with the
 * Trusted Application, or NULL if no Parameters are to be exchanged or if the operation cannot be
 * cancelled. Refer to TEEC_InvokeCommand for more details.
 * @param returnOrigin a pointer to a variable which will contain the return origin. This field may be NULL
 * if the return origin is not needed.
 *
 * @return TEEC_SUCCESS OR TEEC_ERROR*
 */
TEEC_Result TEEC_OpenSession (
		TEEC_Context* context,
		TEEC_Session* session,
		const TEEC_UUID* destination,
		uint32_t connectionMethod,
		const void* connectionData,
		TEEC_Operation* operation,
		uint32_t* returnOrigin);


/**
 * This function closes a Session which has been opened with a Trusted Application.
 *
 * All Commands within the Session MUST have completed before calling this function.
 *
 * The Implementation MUST do nothing if the session parameter is NULL.
 *
 * The implementation of this function MUST NOT be able to fail: after this function returns the Client
 * Application must be able to consider that the Session has been closed.
 *
 * @param session the session to close.
 */
void TEEC_CloseSession (
		TEEC_Session* session);


/**
 * This function invokes a Command within the specified Session.
 *
 * The parameter session MUST point to a valid open Session.
 *
 * The parameter commandID is an identifier that is used to indicate which of the exposed Trusted
 * Application functions should be invoked. The supported command identifiers are defined by the Trusted
 * Application‟s protocol.
 *
 * @param session the open Session in which the command will be invoked.
 * @param commandID the identifier of the Command within the Trusted Application to invoke. The meaning
 * of each Command Identifier must be defined in the protocol exposed by the Trusted Application
 * @param operation a pointer to a Client Application initialized TEEC_Operation structure, or NULL if
 * there is no payload to send or if the Command does not need to support cancellation.
 * @param returnOrigin a pointer to a variable which will contain the return origin. This field may be NULL
 * if the return origin is not needed.
 *
 * @return TEEC_SUCCESS OR TEEC_ERROR*
 */
TEEC_Result TEEC_InvokeCommand(
		TEEC_Session* session,
		uint32_t commandID,
		TEEC_Operation* operation,
		uint32_t* returnOrigin);

/**
 * This function requests the cancellation of a pending open Session operation or a Command invocation
 * operation. As this is a synchronous API, this function must be called from a thread other than the one
 * executing the TEEC_OpenSession or TEEC_InvokeCommand function.
 *
 * @param operation a pointer to a Client Application instantiated Operation structure.
 */
void TEEC_RequestCancellation(
		TEEC_Operation* operation);

/**
 * Pack Parameter types into uint32_t
 *
 * @param t0 First parameter type
 * @param t1 Second parameter type
 * @param t2 Third parameter type
 * @param t3 Fourth parameter type
 */
#define TEEC_PARAM_TYPES( t0, t1, t2, t3) ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12))

#endif /* TEE_CLIENT_API_H_ */
