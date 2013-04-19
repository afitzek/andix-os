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
