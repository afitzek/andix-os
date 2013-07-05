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
 * mutex.c
 *
 *  Created on: Nov 28, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/mutex.h>

void spinlock_lock(spinlock_t * lock) {
	atomic_lock(&lock->_value);
}

void spinlock_unlock(spinlock_t * lock) {
	lock->_value = 0;
}

// =====================================================
// Internal stuff
// =====================================================

uint32_t	atomic_inc(uint32_t* lock) {
	int result, modified;
	result = 0;
	modified = 0;

	asm volatile ("1:LDREX %0,[%1]\n"
	              "ADD   %0,%0,#1\n"
	              "STREX %2,%0,[%1]\n"
	              "CMP   %2,#0\n"
	              "BNE   1b"
	              : "=&r" (result)
	              : "r" (lock), "r" (modified)
	              : "cc", "memory"
	             );

	  return (result);
}

uint32_t	atomic_dec(uint32_t* lock) {
	int result, modified;
	result = 0;
	modified = 0;

	asm volatile ("1:LDREX %0,[%1]\n"
	              "SUB   %0,%0,#1\n"
	              "STREX %2,%0,[%1]\n"
	              "CMP   %2,#0\n"
	              "BNE   1b"
	              : "=&r" (result)
	              : "r" (lock), "r" (modified)
	              : "cc", "memory"
	             );

	  return (result);
}

void atomic_lock(uint32_t* lock) {
	uint32_t tmp = 0;
    asm volatile("try:\n"
    		"LDREX %2, [%0]\n"
    		"CMP %2, #0\n"
    		"STREXEQ %2, %1, [%0]\n"
    		"CMPEQ %2, #0\n"
    		"BNE try"
    		:
    		: "r" (lock), "r" (LOCKED), "r" (tmp)
    		: "cc", "memory");
}
