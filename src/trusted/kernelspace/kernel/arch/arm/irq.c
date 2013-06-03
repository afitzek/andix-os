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
