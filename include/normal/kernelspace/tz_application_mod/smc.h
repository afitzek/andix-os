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
	local_irq_disable();
	__asm__ __volatile__ ("smc #0\n"
			: [result]"=&r"(__arg0)
			: [arg0]"0"(__arg0),
			[arg1]"r"(__arg1),
			[call]"r"(__call)
			: "memory");
	local_irq_enable();
	return __arg0;
}

static inline uint32_t __smc_2_ret(uint32_t call, uint32_t arg0, uint32_t arg1,
		uint32_t* result) {
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
	local_irq_enable();
	(*result) = __arg1;
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
#define CP15DSB	asm volatile ("DSB")
#define CP15DMB	asm volatile ("DMB")

#endif /* SMC_H_ */
