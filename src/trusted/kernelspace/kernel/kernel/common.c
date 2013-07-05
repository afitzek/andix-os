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
 * common.c
 *
 *  Created on: Sep 7, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/typedefs.h>
#include <platform/platform.h>

/*void *memset(void *s, int c, unsigned int n) {
	uint8_t *d = s;

	while (n--)
		*(d++) = c;

	return (s);
}*/

uint8_t is_kernel_addr(uint32_t value) {
	if(value >= 0x80000000) {
		return (1);
	}
	return (0);
}

uint8_t is_valid_addr(void* ptr) {
	uint32_t addr;

	if(ptr == NULL) {
		return (0);
	}

	//vmm_debug("Translating v (0x%x)", ptr);
	__asm__ __volatile__(
			"MCR p15, 0, %1, c7, c8, 1\n"
			"MRC p15, 0, %0, c7, c4, 0":
			"=r" (addr): "r" (ptr):"memory", "cc");

	if (addr < 0xFF) {
		return (0);
	}
	return (1);

}

uint8_t is_valid_kernel_addr(void* ptr) {
	if(!is_kernel_addr((uint32_t)ptr)) {
		return (0);
	}

	return (is_valid_addr(ptr));
}

uint8_t is_valid_user_addr(void* ptr) {
	if(is_kernel_addr((uint32_t)ptr)) {
		return (0);
	}

	return (is_valid_addr(ptr));
}

uint8_t is_valid_nonsecure_ram_addr(void* ptr) {
	// TODO: Check that physical ptr is nonsecure addr in ram
	return (1);
}

uint32_t getCPSR() {
	uint32_t value;
	__asm__ __volatile__("MRS %0, CPSR": "=r" (value)::"memory");
	return (value);
}

char* getModeString(uint32_t cpsr) {
	uint32_t v = cpsr & 0x1F;
	switch (v) {
	case USR_MODE:
		return (USR_MODE_STR);
	case FIQ_MODE:
		return (FIQ_MODE_STR);
	case IRQ_MODE:
		return (IRQ_MODE_STR);
	case SVC_MODE:
		return (SVC_MODE_STR);
	case ABT_MODE:
		return (ABT_MODE_STR);
	case UND_MODE:
		return (UND_MODE_STR);
	case SYS_MODE:
		return (SYS_MODE_STR);
	case MON_MODE:
		return (MON_MODE_STR);
	default:
		return ("UNK");
	}
}
