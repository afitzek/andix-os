/*
 * communication_types.h
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef COMMUNICATION_TYPES_H_
#define COMMUNICATION_TYPES_H_

#ifndef AKERNEL
#ifndef ANDIX_TYPES
#define ANDIX_TYPES
typedef unsigned char uint8_t;
//typedef char				int8_t;
typedef uint8_t* uint8ptr_t;
//typedef int8_t*				int8ptr_t;

typedef unsigned short uint16_t;
typedef short int16_t;
typedef uint16_t* uint16ptr_t;
typedef int16_t* int16ptr_t;

typedef unsigned int uint32_t;
typedef int int32_t;
typedef uint32_t* uint32ptr_t;
//typedef uint32_t* uintptr_t;
typedef int32_t* int32ptr_t;
//typedef int32_t*			intptr_t;

typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef uint64_t* uint64ptr_t;
typedef int64_t* int64ptr_t;
#endif
#else
#include <common/typedefs.h>
#endif

// Define for which the communication memory is used!
#define CTRL_STRUCT			0x01
#define TEE_STRUCT			0x02

#define NUM_SSC_COMMANDS	2

#define SMC_REGISTER_CMEM	0x01
#define SMC_UNREGISTER_CMEM	0x02
#define SMC_PROCESS_CMEM	0x03

#define SMC_REGISTER_TMEM	0x11
#define SMC_UNREGISTER_TMEM	0x12
#define SMC_PROCESS_TMEM	0x13

#define NSC_OPEN_CONTEXT	1
#define NSC_CLOSE_CONTEXT	2

// REE FS OPS:
#define TZ_CTRL_REE_FS_OPEN			0x100
#define TZ_CTRL_REE_FS_REMOVE		0x101
#define TZ_CTRL_REE_FS_CLOSE		0x102
#define TZ_CTRL_REE_FS_READ			0x103
#define TZ_CTRL_REE_FS_WRITE		0x104
#define TZ_CTRL_REE_FS_FSTAT		0x105
#define TZ_CTRL_REE_FS_LSEEK		0x106

#define TZ_CTRL_REE_FS_OPEN_RET		0x180
#define TZ_CTRL_REE_FS_REMOVE_RET	0x181
#define TZ_CTRL_REE_FS_CLOSE_RET	0x182
#define TZ_CTRL_REE_FS_READ_RET		0x183
#define TZ_CTRL_REE_FS_WRITE_RET	0x184
#define TZ_CTRL_REE_FS_FSTAT_RET	0x185
#define TZ_CTRL_REE_FS_LSEEK_RET	0x186

//#define TEEC_CONFIG_SHAREDMEM_MAX_SIZE (0x1000000) // 16MB

#define TZ_FS_HASH_SIZE			(32)
#define TZ_BLOCK_SIZE	(1024)

#define REE_SEEK_SET				(0x0)
#define REE_SEEK_CUR				(0x1)

typedef struct {
	uint8_t dirname[TZ_FS_HASH_SIZE]; // SHA256 HASH
	uint8_t filename[TZ_FS_HASH_SIZE]; // SHA256 HASH
} REE_FS_NAME;

typedef struct {
	uint32_t st_size; /* total size, in bytes */
} REE_FS_STAT;

typedef struct {
	REE_FS_NAME pathname;
	int32_t flags;
	int32_t mode;
} TZ_CTRL_REE_OPEN;

typedef struct {
	REE_FS_NAME pathname;
} TZ_CTRL_REE_REMOVE;

typedef struct {
	int32_t fd;
} TZ_CTRL_REE_CLOSE;

typedef struct {
	int32_t fd;
	uint8_t buffer[TZ_BLOCK_SIZE];
	uint32_t count;
} TZ_CTRL_REE_READ;

typedef struct {
	int32_t fd;
	uint8_t buffer[TZ_BLOCK_SIZE];
	uint32_t count;
} TZ_CTRL_REE_WRITE;

typedef struct {
	int32_t fd;
	REE_FS_STAT stat;
} TZ_CTRL_REE_FSTAT;

typedef struct {
	int32_t fd;
	uint32_t offset;
	int32_t whence;
} TZ_CTRL_REE_LSEEK;

// NON SECURE CONTROL OPS:
#define TZ_CTRL_OP_IDLE			0x01
#define TZ_CTRL_OP_DEXIT		0x02

#define TZ_CTRL_OP_BOPEN		0x11
#define TZ_CTRL_OP_BWRITE		0x12
#define TZ_CTRL_OP_BREAD		0x13
#define TZ_CTRL_OP_BCLOSE		0x14
#define TZ_CTRL_OP_BCREATE		0x15
#define TZ_CTRL_OP_BSEEK		0x16
#define TZ_CTRL_OP_BFEXISTS		0x17

#define TZ_CTRL_OP_BOPEN_RET	0x21
#define TZ_CTRL_OP_BWRITE_RET	0x22
#define TZ_CTRL_OP_BREAD_RET	0x23
#define TZ_CTRL_OP_BCLOSE_RET	0x24
#define TZ_CTRL_OP_BCREATE_RET	0x25
#define TZ_CTRL_OP_BSEEK_RET	0x26
#define TZ_CTRL_OP_BFEXISTS_RET	0x27

typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEECOM_UUID;

typedef struct {
	uint32_t	memid;
} TEEC_MemRefParameter;

typedef struct {
	uint32_t	a;
	uint32_t	b;
} TEEC_ValueParameter;

typedef union {
	TEEC_MemRefParameter memref;
	TEEC_ValueParameter	value;
} TEEC_Paramter;

typedef struct
{
	uint32_t		started;
	uint32_t		paramTypes;
	TEEC_Paramter	params[4];
} TEECOM_Operation;

// DEVICE FLAGS

#define TZ_BLOCK_CREATE			(0x01)

typedef struct {
	uint8_t dirname[TZ_FS_HASH_SIZE]; // SHA256 HASH
	uint8_t filename[TZ_FS_HASH_SIZE]; // SHA256 HASH
	uint32_t flags;
	uint32_t mode;
	void* nsfd;
} TZ_CTRL_BOPEN;

typedef struct {
	uint32_t blkCount;
	void* nsfd;
} TZ_CTRL_BCOUNT;

typedef struct {
	void* nsfd;
	uint8_t buffer[TZ_BLOCK_SIZE]; // We process in 1024 bytes  between secure and nonsecure
	uint32_t blockpos;
} TZ_CTRL_BLOCK;

typedef struct {
	uint32_t op;
	uint32_t ret;
	union {
		TZ_CTRL_BOPEN bopen;
		TZ_CTRL_BLOCK bwrite;
		TZ_CTRL_BCOUNT bcount;
		TZ_CTRL_REE_OPEN ree_open;
		TZ_CTRL_REE_CLOSE ree_close;
		TZ_CTRL_REE_WRITE ree_write;
		TZ_CTRL_REE_READ ree_read;
		TZ_CTRL_REE_FSTAT ree_fstat;
		TZ_CTRL_REE_LSEEK ree_lseek;
		TZ_CTRL_REE_REMOVE ree_remove;
	} params;
} TZ_CTLR_SPACE;

#define TZ_TEE_OP_INIT_CTX		(0x01)
#define TZ_TEE_OP_FIN_CTX		(0x02)
#define TZ_TEE_OP_REGISTER_MEM	(0x03)
#define TZ_TEE_OP_RELEASE_MEM	(0x05)
#define TZ_TEE_OP_OPEN_SESSION	(0x06)
#define TZ_TEE_OP_CLOSE_SESSION	(0x07)
#define TZ_TEE_OP_INVOKE_CMD	(0x08)
#define TZ_TEE_OP_CANCEL_REQ	(0x09)

typedef struct {
	uint32_t	context;
} TZ_TEE_INIT_CTX;

typedef struct {
	uint32_t	context;
} TZ_TEE_FIN_CTX;

typedef struct {
	uint32_t	context;
	uint32_t	paddr;
	uint32_t	size;
	uint32_t	flags;
	uint32_t	memid;
} TZ_TEE_REGISTER_MEM;

typedef struct {
	uint32_t	memid;
} TZ_TEE_RELEASE_MEM;

typedef struct {
	uint32_t	context;
	uint32_t	session;
	TEECOM_UUID	uuid;
	uint32_t	connectionMethod;
	uint32_t	connectionData;
	TEECOM_Operation	operation;
	uint32_t	returnOrigin;
} TZ_TEE_OPEN_SESSION;

typedef struct {
	uint32_t	session;
} TZ_TEE_CLOSE_SESSION;

typedef struct {
	uint32_t	session;
	uint32_t	command;
	TEECOM_Operation	operation;
	uint32_t	returnOrigin;
} TZ_TEE_INVOKE_CMD;

typedef struct {
	uint32_t op;
	uint32_t ret;
	union {
		TZ_TEE_INIT_CTX initCtx;
		TZ_TEE_FIN_CTX finCtx;
		TZ_TEE_REGISTER_MEM regMem;
		TZ_TEE_OPEN_SESSION	openSession;
		TZ_TEE_CLOSE_SESSION closeSession;
		TZ_TEE_INVOKE_CMD	invokeCommand;
	} params;
} TZ_TEE_SPACE;

typedef struct {
	uint32_t req_type;
	union {
		TZ_CTLR_SPACE ctrl;
		TZ_TEE_SPACE tee;
	} p;
} TZ_MAIN_COM;

#endif /* COMMUNICATION_TYPES_H_ */
