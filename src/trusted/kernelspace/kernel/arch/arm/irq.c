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
