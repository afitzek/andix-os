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
 * irq.c
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

void enable_irq() {
	__asm__ __volatile__ (
			"MRS r1, CPSR\n"
			"BIC r1, r1, #0x80\n"
			"MSR CPSR_c, r1");
}

void disable_irq() {
	__asm__ __volatile__ (
			"MRS r1, CPSR\n"
			"ORR r1, r1, #0x80\n"
			"MSR CPSR_c, r1");
}

void enable_fiq() {
	__asm__ __volatile__ (
			"MRS r1, CPSR\n"
			"BIC r1, r1, #0x40\n"
			"MSR CPSR_c, r1");
}

void disable_fiq() {
	__asm__ __volatile__ (
			"MRS r1, CPSR\n"
			"ORR r1, r1, #0x40\n"
			"MSR CPSR_c, r1");
}

unsigned int get_cpsr() {
	register unsigned int __cpsr __asm__("r0") = 0;
	__asm__ __volatile__ ("MRS %0, CPSR\n"
			: [result]"=&r"(__cpsr)
			:: "memory");
	return (__cpsr);
}

void send_fiq_irq_to_monitor() {
	register unsigned int __scr = 0x14;
	__asm__ __volatile__ ("MCR p15, 0, %0, c1, c1, 0\n"
			:: "r"(__scr));
}

unsigned int get_scr() {
	register unsigned int __scr __asm__("r0") = 0;
	__asm__ __volatile__ ("MRC  p15, 0, %0, c1, c1, 0\n"
			: [result]"=&r"(__scr)
			:: "memory");
	return (__scr);
}
