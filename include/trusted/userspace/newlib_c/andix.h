/*
 * andix.h
 *
 *  Created on: Jan 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ANDIX_H_
#define ANDIX_H_

typedef unsigned char __uint8_t_;
typedef signed char __int8_t_;
typedef unsigned short __uint16_t_;
typedef signed short __int16_t_;
typedef unsigned int __uint32_t_;
typedef signed int __int32_t_;

#ifndef __TEE_UUID__
#define __TEE_UUID__
typedef struct {
	unsigned int timeLow;
	unsigned short timeMid;
	unsigned short timeHiAndVersion;
	unsigned char clockSeqAndNode[8];
} TEE_UUID;
#endif

#ifndef __TEE_PARAM__
#define __TEE_PARAM__
typedef union {
	struct {
		void* buffer;
		__uint32_t_ size;
	} memref;
	struct {
		__uint32_t_ a, b;
	} value;
} TEE_Param;
#endif

#ifndef __TEE_RESULT__
#define __TEE_RESULT__
/**
 * Result value client compatible
 */
typedef __uint32_t_		TEE_Result;
#endif

void set_uuid(const TEE_UUID* uuid);

unsigned int get_os_rand();

#endif /* ANDIX_H_ */
