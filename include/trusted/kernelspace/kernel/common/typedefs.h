/*
 * typedefs.h
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#ifndef ANDIX_TYPES
#define ANDIX_TYPES
typedef unsigned char		uint8_t;
typedef char				int8_t;
typedef uint8_t*			uint8ptr_t;
typedef int8_t*				int8ptr_t;

typedef unsigned short		uint16_t;
typedef short				int16_t;
typedef uint16_t*			uint16ptr_t;
typedef int16_t*			int16ptr_t;

typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef uint32_t*			uint32ptr_t;
typedef uint32_t*			uintptr_t;
typedef int32_t*			int32ptr_t;
typedef int32_t*			intptr_t;

typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef uint64_t*			uint64ptr_t;
typedef int64_t*			int64ptr_t;
#endif

#define forceinline __inline__ __attribute__((always_inline))
#define ensure_forceinline __attribute__((always_inline))

#ifndef NULL
#define NULL ((void *)0)
#endif

#define UINT_MAX			(0xFFFFFFFF)

typedef uint32_t			size_t;

typedef uint32_t			tid_t;

#define NOP	__asm__ __volatile__("NOP")

#define DEBUG_STOP		while(1) { NOP; }

#define ANDIZ_TYPEDEFS

#endif /* TYPEDEFS_H_ */
