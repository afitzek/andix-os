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
