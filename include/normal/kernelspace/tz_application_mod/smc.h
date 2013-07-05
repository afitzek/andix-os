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
 * smc.h
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SMC_H_
#define SMC_H_

#include <tz_application_mod/types.h>
#include <linux/mm.h>

static inline uint32_t __smc_1(uint32_t call, uint32_t arg0) {
	register uint32_t __call __asm__("ip") = call;
	register uint32_t __arg0 __asm__("r0") = arg0;
	//flush_tlb_all();
	local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: [result]"=&r"(__arg0)
			: [arg0]"0"(__arg0),
			[call]"r"(__call)
			: "memory");
	local_irq_enable();
	return __arg0;
}

static inline uint32_t __smc_2(uint32_t call, uint32_t arg0, uint32_t arg1) {
	register uint32_t __call __asm__("ip") = call;
	register uint32_t __arg0 __asm__("r0") = arg0;
	register uint32_t __arg1 __asm__("r1") = arg1;
	//flush_tlb_all();
	//local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: "=r"(__arg0)
			: "0"(__arg0),
			"r"(__arg1),
			"r"(__call));
	//__asm__ __volatile__ ("BL .\n");
	//local_irq_enable();
	return __arg0;
}

static inline uint32_t __smc_2_ret(uint32_t call, uint32_t arg0, uint32_t arg1,
		uint32_t* resultv) {
	register uint32_t __call __asm__("ip") = call;
	register uint32_t __arg0 __asm__("r0") = arg0;
	register uint32_t __arg1 __asm__("r1") = arg1;
	//flush_tlb_all();
	local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: [result]"=&r"(__arg0)
			: [arg0]"0"(__arg0),
			[arg1]"r"(__arg1),
			[call]"r"(__call)
			: "memory");
	*(volatile uint32_t *)(resultv) = __arg1;
	local_irq_enable();
	return __arg0;
}

static inline uint32_t __smc_3(uint32_t call, uint32_t arg0, uint32_t arg1,
		uint32_t arg2) {
	register uint32_t __call __asm__("ip") = call;
	register uint32_t __arg0 __asm__("r0") = arg0;
	register uint32_t __arg1 __asm__("r1") = arg1;
	register uint32_t __arg2 __asm__("r2") = arg2;
	//flush_tlb_all();
	local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: [result]"=&r"(__arg0)
			: [arg0]"0"(__arg0),
			[arg1]"r"(__arg1),
			[arg2]"r"(__arg2),
			[call]"r"(__call)
			: "memory");
	local_irq_enable();
	return __arg0;
}

static inline uint32_t __smc_4(uint32_t call, uint32_t arg0, uint32_t arg1,
		uint32_t arg2, uint32_t arg3) {
	register uint32_t __call __asm__("ip") = call;
	register uint32_t __arg0 __asm__("r0") = arg0;
	register uint32_t __arg1 __asm__("r1") = arg1;
	register uint32_t __arg2 __asm__("r2") = arg2;
	register uint32_t __arg3 __asm__("r3") = arg3;
	//flush_tlb_all();
	local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: [result]"=&r"(__arg0)
			: [arg0]"0"(__arg0),
			[arg1]"r"(__arg1),
			[arg2]"r"(__arg2),
			[arg3]"r"(__arg3),
			[call]"r"(__call)
			: "memory");
	local_irq_enable();
	return __arg0;
}

#define CP15ISB	asm volatile ("ISB")
#define CP15DSB	asm volatile ("MCR 	p15, 0, r0, c7, c10, 4")
#define CP15DMB	asm volatile ("MCR 	p15, 0, r0, c7, c10, 5")

#endif /* SMC_H_ */
