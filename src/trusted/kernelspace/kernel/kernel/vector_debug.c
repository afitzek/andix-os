/*
 * vector_debug.c
 *
 *  Created on: Sep 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/typedefs.h>
#include <kprintf.h>
#include <platform/vector_debug.h>
#include <platform/platform.h>
#include <vectors.h>
#include <mm/mm.h>

void provokeDA() {
	volatile uint32_t* d = NULL;
	volatile uint32_t v;
	v = (*d);
	(*d) = v;
}

void dump_regs(core_reg* regs) {
	ex_debug("current Mode: 0x%x %s", getCPSR(), getModeString(getCPSR()));
	ex_debug(" abort  Mode: 0x%x %s", regs->cpsr, getModeString(regs->cpsr));
	uint32_t mode = regs->cpsr & 0x1F;
	uint32_t fp;
	uint32_t sp;
	ex_debug(
			"CPSR: 0x%x   SCR: 0x%x    PC: 0x%x", regs->cpsr, regs->scr, regs->pc);
	if (vmm_is_kernel_code_mem_addr((uintptr_t) regs->pc)) {
		ex_debug("FUNC: %s", get_function_name((void*)regs->pc));
	}
	ex_debug(
			"  R0: 0x%x    R1: 0x%x    R2: 0x%x", regs->r[0], regs->r[1], regs->r[2]);
	ex_debug(
			"  R3: 0x%x    R4: 0x%x    R5: 0x%x", regs->r[3], regs->r[4], regs->r[5]);
	ex_debug(
			"  R6: 0x%x    R7: 0x%x    R8: 0x%x", regs->r[6], regs->r[7], regs->r[8]);
	ex_debug(
			"  R9: 0x%x   R10: 0x%x   R11: 0x%x", regs->r[9], regs->r[10], regs->r[11]);
	ex_debug(" R12: 0x%x", regs->r[12]);

	if (mode == SVC_MODE) {
		fp = getSVCFPIntoAbt();
		sp = getSVCSP();
		gotoABTMode();

		dump_stack_trace_stack(sp, fp);
	}

}

char* get_da_status(uint32_t status) {
	switch (status) {
	case DA_STATUS_ALIGN:
		return (DA_STATUS_ALIGN_S);
	case DA_STATUS_INSTR:
		return (DA_STATUS_INSTR_S);
	case DA_STATUS_L1PRE:
		return (DA_STATUS_L1PRE_S);
	case DA_STATUS_L1PRE_AXI:
		return (DA_STATUS_L1PRE_AXI_S);
	case DA_STATUS_L2PRE:
		return (DA_STATUS_L2PRE_S);
	case DA_STATUS_L2PRE_AXI:
		return (DA_STATUS_L2PRE_AXI_S);
	case DA_STATUS_L1PPE:
		return (DA_STATUS_L1PPE_S);
	case DA_STATUS_L2PPE:
		return (DA_STATUS_L2PPE_S);
	case DA_STATUS_TFSEC:
		return (DA_STATUS_TFSEC_S);
	case DA_STATUS_TFPAG:
		return (DA_STATUS_TFPAG_S);
	case DA_STATUS_PFPAG:
		return (DA_STATUS_PFPAG_S);
	case DA_STATUS_AFSEC:
		return (DA_STATUS_AFSEC_S);
	case DA_STATUS_AFPAG:
		return (DA_STATUS_AFPAG_S);
	case DA_STATUS_DFSEC:
		return (DA_STATUS_DFSEC_S);
	case DA_STATUS_DFPAG:
		return (DA_STATUS_DFPAG_S);
	case DA_STATUS_PFSEC:
		return (DA_STATUS_PFSEC_S);
	case DA_STATUS_PEANT:
		return (DA_STATUS_PEANT_S);
	case DA_STATUS_PEANT_AXI:
		return (DA_STATUS_PEANT_AXI_S);
	case DA_STATUS_IMPEA:
		return (DA_STATUS_IMPEA_S);
	case DA_STATUS_IMPEA_AXI:
		return (DA_STATUS_IMPEA_AXI_S);
	case DA_STATUS_IMEPE:
		return (DA_STATUS_IMEPE_S);
	case DA_STATUS_DEBUG:
		return (DA_STATUS_DEBUG_S);
	default:
		return ("unknown status");
	}
}

void pDA(core_reg* regs) {

	uint32_t dfsr = 0xFFFFFFFF;
	uint32_t dfar = 0xFFFFFFFF;
	uint32_t status = 0;

	kprintf_ex("\n");
	dab_debug("====== INTERRUPT ========");
	dab_debug("DATA ABORT");

	__asm__ __volatile__("MRC   p15, 0, %0, c5, c0, 0": "=r" (dfsr)::"memory");
	__asm__ __volatile__("MRC   p15, 0, %0, c6, c0, 0": "=r" (dfar)::"memory");

	status = (dfsr & DFSR_STATUS)
			| ((dfsr & DFSR_STATUS_1)>> 6)|((dfsr & DFSR_SD)>> 7);

	dab_debug("STATUS: %s [%x]", get_da_status(status), status);

	dab_debug(
			"Failed to %s 0x%x @ INSTR: 0x%x", ((dfsr & DFSR_RW) >> 11) ? "write to" : "read from", dfar, regs->pc - 8);

	dab_debug("DFSR: 0x%x  STAT: 0x%x  DFAR: 0x%x", dfsr, status, dfar);

	dab_debug(
			"  RW: %x (%c)  SD (AXI): %x  DOMAIN: 0x%x", ((dfsr & DFSR_RW) >> 11), ((dfsr & DFSR_RW) >> 11) ? 'W' : 'R', ((dfsr & DFSR_SD) >> 12), ((dfsr & DFSR_DOMAIN) >> 4));

	dab_debug("Register dump:");

	dump_regs(regs);

	dab_debug("====== INTERRUPT ========");
}

void pPA(core_reg* regs) {

	uint32_t ifsr = 0xFFFFFFFF;
	uint32_t ifar = 0xFFFFFFFF;
	uint32_t status = 0;

	kprintf_ex("\n");
	dab_debug("====== INTERRUPT ========");
	dab_debug("PREFETCH ABORT");

	__asm__ __volatile__("MRC   p15, 0, %0, c5, c0, 1": "=r" (ifsr)::"memory");
	__asm__ __volatile__("MRC   p15, 0, %0, c6, c0, 2": "=r" (ifar)::"memory");

	status = (ifsr & DFSR_STATUS)
			| ((ifsr & DFSR_STATUS_1)>> 6)|((ifsr & DFSR_SD)>> 7);

	dab_debug("STATUS: %s [%x]", get_da_status(status), status);

	dab_debug(
			"Failed to %s 0x%x @ INSTR: 0x%x", ((ifsr & DFSR_RW) >> 11) ? "write to" : "read from", ifar, regs->pc - 8);

	dab_debug("IFSR: 0x%x  STAT: 0x%x  IFAR: 0x%x", ifsr, status, ifar);

	dab_debug(
			"  RW: %x (%c)  SD (AXI): %x  DOMAIN: 0x%x", ((ifsr & DFSR_RW) >> 11), ((ifsr & DFSR_RW) >> 11) ? 'W' : 'R', ((ifsr & DFSR_SD) >> 12), ((ifsr & DFSR_DOMAIN) >> 4));

	dab_debug("Register dump:");

	dump_regs(regs);

	dab_debug("====== INTERRUPT ========");
}

void pUA(core_reg* regs) {

	uint32_t dfsr = 0xFFFFFFFF;
	uint32_t dfar = 0xFFFFFFFF;
	uint32_t status = 0;

	kprintf_ex("\n");
	dab_debug("====== INTERRUPT ========");
	dab_debug("UNDEFINED ABORT");

	__asm__ __volatile__("MRC   p15, 0, %0, c5, c0, 0": "=r" (dfsr)::"memory");
	__asm__ __volatile__("MRC   p15, 0, %0, c6, c0, 0": "=r" (dfar)::"memory");

	status = (dfsr & DFSR_STATUS)
			| ((dfsr & DFSR_STATUS_1)>> 6)|((dfsr & DFSR_SD)>> 7);

	dab_debug("STATUS: %s [%x]", get_da_status(status), status);

	dab_debug(
			"Failed to %s 0x%x @ INSTR: 0x%x", ((dfsr & DFSR_RW) >> 11) ? "write to" : "read from", dfar, regs->pc - 8);

	dab_debug("DFSR: 0x%x  STAT: 0x%x  DFAR: 0x%x", dfsr, status, dfar);

	dab_debug(
			"  RW: %x (%c)  SD (AXI): %x  DOMAIN: 0x%x", ((dfsr & DFSR_RW) >> 11), ((dfsr & DFSR_RW) >> 11) ? 'W' : 'R', ((dfsr & DFSR_SD) >> 12), ((dfsr & DFSR_DOMAIN) >> 4));

	dab_debug("Register dump:");

	dump_regs(regs);

	dab_debug("====== INTERRUPT ========");
}

void pIRQ(core_reg* regs) {

	uint32_t scr;

	kprintf_ex("\n");
	dab_debug("====== INTERRUPT ========");
	dab_debug("IRQ");

	dab_debug("Register dump:");

	__asm__ __volatile__("MRC 	p15, 0, %0, c1, c1, 0": "=r" (scr)::"memory");

	dab_debug("SCR: 0x%x", scr);

	dump_regs(regs);

	dab_debug("====== INTERRUPT ========");
}

void pSWI(core_reg* regs) {

	kprintf_ex("\n");
	dab_debug("====== INTERRUPT ========");
	dab_debug("SWI!!!");

	dab_debug("Register dump:");

	dump_regs(regs);

	dab_debug("====== INTERRUPT ========");
}

