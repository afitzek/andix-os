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
 * monitor.c
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <monitor/monitor.h>
#include <mm/mm.h>
#include <scheduler.h>
#include <cache.h>
#include <kprintf.h>
#include <task/service.h>
#include <task/tee.h>
#include <platform/vector_debug.h>

void dump_mon_context(mon_context_t* cont) {
	mon_debug("----- REGS -----");
	mon_debug("CPSR: 0x%x   MODE: %s", cont->cpsr, getModeString(cont->cpsr));
	mon_debug("SCR: 0x%x    [%c]         PC: 0x%x", cont->scr,
			((cont->scr & 1) ? 'N' : 'S'), cont->pc);
	mon_debug("  R0: 0x%x    R1: 0x%x    R2: 0x%x", cont->r[0], cont->r[1],
			cont->r[2]);
	mon_debug("  R3: 0x%x    R4: 0x%x    R5: 0x%x", cont->r[3], cont->r[4],
			cont->r[5]);
	mon_debug("  R6: 0x%x    R7: 0x%x    R8: 0x%x", cont->r[6], cont->r[7],
			cont->r[8]);
	mon_debug("  R9: 0x%x   R10: 0x%x   R11: 0x%x", cont->r[9], cont->r[10],
			cont->r[11]);
	mon_debug(" R12: 0x%x", cont->r[12]);
	mon_debug("----- DONE -----");
}

void mon_secure_switch_context(mon_context_t *regs, struct thread_t *thread) {
	// save old context ...
	struct thread_t *current = get_current_thread();

	if (current != NULL ) {
		mon_debug("Saving thread [%d]", current->tid);
		memcpy(&current->context, regs, sizeof(mon_context_t));
		monitor_save_sys_regs(&current->sys_context);
		current->kernel_stack.sp = getSVCSP();
		current->user_stack.sp = getSYSSP();
//		current->userPD = get_user_table();		hmm this shouldn't need to be saved, its constant anyways.
		// TODO: Save different mode stacks cpsr etc
		gotoMONMode();
	}

	mon_debug("Loading thread [%d] from: 0x%x", thread->tid, thread);

	//mon_debug("Restoring context");

	// Restore the new context
	monitor_restore_sys_regs(&thread->sys_context);
	memcpy(regs, &thread->context, sizeof(mon_context_t));

	//mon_debug("Current Mode: %s", getModeString(getCPSR()));

	if (thread->kernel_stack.sp != 0) {
		//mon_debug("setting kernel stack to 0x%x", task->kernelSP);
		setSVCSP((uintptr_t) thread->kernel_stack.sp);
		gotoMONMode();
	}

	if (thread->user_stack.sp != 0) {
		//mon_debug("setting user stack to 0x%x", task->userSP);
		setSYSSP((uintptr_t) thread->user_stack.sp);
		gotoMONMode();
	}

	if (thread->process != NULL && thread->process->userPD != NULL ) {
		//mon_debug("setting user page directory to 0x%x", task->userPD);
		if (thread->process->userPD != get_user_table()) {
			// not necessary on intra-process switches
			set_user_table(thread->process->userPD);
			invalidate_tlb();
		}
	}

	//mon_debug("Current Mode: %s", getModeString(getCPSR()));
	set_user_thread_reg(thread->thread_pointer);
	sched_thread_switched(thread);

	// Invalidate exclusive monitors at context switch.
	// Required to ensure correct mutex operation.
	asm volatile ("CLREX \n" : : : "memory");

	mon_debug("Switch to thread 0x%x %d done.", thread, thread->tid);
}

uint32_t create_session(uint32_t paddr) {
	struct user_process_t *proc = NULL;
	intptr_t mapped = NULL;
	uint32_t ret = 0;
	kernel_mem_info_t info;
	TASK_UUID* uuid;
	info.ap = AP_SVC_RW_USR_NO;
	info.bufferable = 0;
	info.cacheable = 0;
	info.domain = 0;
	info.execute = EXEC_NON;
	info.nonsecure = 1;
	info.shareable = 1;
	info.tex = 0;
	info.type = SMALL_PAGE;
	intptr_t vaddr;
	// map address cont->r[0]
	mapped = mmm_allocate_page();
	info.vaddr = (uint32_t) mapped;
	info.paddr = paddr;
	map_kernel_memory(&info);
	vaddr = (intptr_t) (((uint32_t) mapped & 0xFFFFF000) | (paddr & 0xFFF));
	uuid = (TASK_UUID*) vaddr;
	proc = get_process_by_uuid(uuid);
	//task = get_task_by_id(2);

	if (proc != NULL ) {
		switch_to_thread(proc->tee_context.tee_handler);
		ret = 0;
	} else {
		ret = -1;
	}
	mon_debug("UUID: %x-%x-%x-%x%x-%x%x%x%x%x%x", uuid->timeLow, uuid->timeMid,
			uuid->timeHiAndVersion, uuid->clockSeqAndNode[0],
			uuid->clockSeqAndNode[1], uuid->clockSeqAndNode[2],
			uuid->clockSeqAndNode[3], uuid->clockSeqAndNode[4],
			uuid->clockSeqAndNode[5], uuid->clockSeqAndNode[6],
			uuid->clockSeqAndNode[7]);
	mon_debug("UUID @ v 0x%x p 0x%x", uuid, paddr);
	unmap_kernel_memory((uint32_t) vaddr);
	mmm_free_page(vaddr);
	return (ret);
}

TZ_CTLR_SPACE* com_mem = NULL;
TZ_TEE_SPACE* tee_mem = NULL;
TZ_PACKAGE* package = NULL;

void free_tz_communication_memory() {
	if (com_mem != NULL ) {
		inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));
		mon_debug("Unmapping old communication memory");
		unmap_memory((uintptr_t) com_mem, sizeof(TZ_CTLR_SPACE));
		com_mem = NULL;
	}
}

uint32_t cl_size;

void free_tz_tee_memory() {
	if (tee_mem != NULL ) {
		inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
		mon_debug("Unmapping old tee memory");
		unmap_memory((uint32ptr_t) tee_mem, sizeof(TZ_TEE_SPACE));
		tee_mem = NULL;
	}
}

void inv_tz_memory(void* memory, uint32_t size) {
	uint32_t start = (uint32_t) memory;
	//start = (start & 0xFFFFFFE0);
	uint32_t end = start + size;
	start = round_down(start, 0x1F);
	end = round_up(end, 0x1F);
	while (start < end) {
		asm ("mcr p15, 0, %0, c7, c14, 1" : : "r" (start)
				: "memory");
		start += 0x1F;
	}
	CP15DSB;
	CP15ISB;
}

void free_tz_package() {
	if (package != NULL ) {
		inv_tz_memory(package, sizeof(TZ_PACKAGE));
		mon_debug("Unmapping old package memory");
		unmap_memory((uint32ptr_t) package, sizeof(TZ_PACKAGE));
		package = NULL;
	}
	free_tz_tee_memory();
	free_tz_communication_memory();
}

int set_tz_package(void* ptr) {
	uintptr_t paddr = virt_ns_to_phys(ptr);
	uintptr_t tee_paddr = NULL;
	uintptr_t ctrl_paddr = NULL;

	if (paddr == NULL ) {
		return (-1);
	}

	free_tz_package();

	package = (TZ_PACKAGE*) map_phys_mem(paddr, sizeof(TZ_PACKAGE),
			AP_SVC_RW_USR_NO, 0, 0, 1);

	if (package == NULL ) {
		mon_error("Failed to map package memory!");
		return (-1);
	}

	inv_tz_memory(package, sizeof(TZ_PACKAGE));

	tee_paddr = virt_ns_to_phys(package->physical_tee);

	if (tee_paddr == NULL ) {
		free_tz_package();
		return (-1);
	}

	if (set_tz_tee_memory(tee_paddr) != 0) {
		free_tz_package();
		return (-1);
	}

	ctrl_paddr = virt_ns_to_phys(package->physical_ctrl);

	if (ctrl_paddr == NULL ) {
		free_tz_package();
		return (-1);
	}

	if (set_tz_communication_memory(ctrl_paddr) != 0) {
		free_tz_package();
		return (-1);
	}

	return (0);
}

int set_tz_tee_memory(void* ptr) {

	free_tz_tee_memory();

	tee_mem = (TZ_TEE_SPACE*) map_phys_mem((uintptr_t) ptr,
			sizeof(TZ_TEE_SPACE), AP_SVC_RW_USR_NO, 0, 0, 1);

	if (tee_mem == NULL ) {
		mon_error("Failed to map tee memory!");
		return (-1);
	}

	inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));

	return (0);
}

int set_tz_communication_memory(void* ptr) {

	free_tz_communication_memory();

	com_mem = (TZ_CTLR_SPACE*) map_phys_mem((uintptr_t) ptr,
			sizeof(TZ_CTLR_SPACE), AP_SVC_RW_USR_NO, 0, 0, 1);

	if (com_mem == NULL ) {
		mon_error("Failed to map com memory!");
		return (-1);
	}

	inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));

	return (0);
}

void mon_smc_non_secure_handler(mon_context_t* cont) {
	//for (int i = 0; i < 60; i++) {
	//	mon_debug("CLEANING BUFFER FROM NON SECURE!!");
	//}
	struct thread_t* target_task = NULL;
	uint32_t physical_package;
	mon_debug("FROM NON SECURE");
	dump_mon_context(cont);
	switch (cont->r[12]) {
	case SMC_PROCESS_CMEM:
		physical_package = cont->r[0];
		set_tz_package((void*) physical_package);

		com_mem = mon_get_control_space();

		// THIS REQUEST IS A RESPONSE => DISPATCH TO SERVICE TASK!
		mon_debug("Process com memory! @ v 0x%x p 0x%x", com_mem,
				v_to_p((uintptr_t)com_mem));

		mon_debug("CTRL RESULT: 0x%x", com_mem->ret);

		if (com_mem == NULL ) {
			cont->r[0] = -1;
		} else {
			inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
			inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));
			inv_tz_memory(package, sizeof(TZ_PACKAGE));
			//mon_info("COM MEMORY IN TZ:");
			//kprintHex((uint8_t*)com_mem, sizeof(TZ_CTLR_SPACE));
			get_current_thread()->state = BLOCKED;
			target_task = service_get_service_thread();
			if (target_task == NULL ) {
				cont->r[0] = -1;
				break;
			}
			target_task->state = READY;
			mon_secure_switch_context(cont, target_task);
		}
		break;
	case SMC_PROCESS_TMEM:
		physical_package = cont->r[0];
		set_tz_package((void*) physical_package);

		tee_mem = mon_get_tee_space();

		// THIS REQUEST IS A REQUEST => DISPATCH TO PROCESS TASK!
		mon_debug("Process tee memory! @ v 0x%x p 0x%x", tee_mem,
				v_to_p((uintptr_t)tee_mem));

		if (tee_mem == NULL ) {
			cont->r[0] = -1;
		} else {
			inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
			inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));
			inv_tz_memory(package, sizeof(TZ_PACKAGE));
			//mon_info("TEE MEMORY IN TZ:");
			//kprintHex((uint8_t*)tee_mem, sizeof(TZ_TEE_SPACE));
			//DEBUG_STOP;
			target_task = tee_get_tee_thread();
			if (target_task == NULL ) {
				cont->r[0] = -1;
				break;
			}
			target_task->state = READY;
			mon_secure_switch_context(cont, target_task);
		}
		break;
	}
	dump_mon_context(cont);

	if (target_task != NULL ) {
	 mon_debug(
	 "GOING FROM SECURE TO %sSECURE", (target_task->context.scr & 1) ? "NON-" : "");
	 } else {
	 mon_debug("GOING FROM SECURE TO (?)SECURE");
	 }
}

TZ_CTLR_SPACE* mon_get_control_space() {
	if (com_mem != NULL ) {
		return (com_mem);
	}
	return (NULL );
}

TZ_TEE_SPACE* mon_get_tee_space() {
	if (tee_mem != NULL ) {
		return (tee_mem);
	}
	return (NULL );
}

char* get_ssc_name(uint32_t num) {
	switch (num) {
	case SSC_TASK_SWITCH:
		return ("SSC_TASK_SWITCH");
	case SSC_TASK_SCHEDULE:
		return ("SSC_TASK_SCHEDULE");
	case SSC_NONS_SERVICE:
		return ("SSC_NONS_SERVICE");
	default:
		return ("NO NAME");
	}
}

void mon_smc_secure_handler(mon_context_t* cont) {
	//mon_debug("FROM SECURE");
	dump_mon_context(cont);
	struct thread_t *thread = NULL;
	//uint32_t tmp1;
	mon_debug("ACTION: %d [%s]", cont->r[12], get_ssc_name(cont->r[12]));
	switch (cont->r[12]) {
	case SSC_TASK_SWITCH:
		thread = (struct thread_t*) cont->r[0];
		if (thread == NULL ) {
			mon_debug("FAILED TO CHANGE TO TASK NULL!!");
			return;
		}
		mon_info("Switching to thread %d of %s", thread->tid,
				thread->process ? thread->process->name : "Kernel Thread");
		mon_secure_switch_context(cont, thread);
		break;
	case SSC_TASK_SCHEDULE:
		thread = sched_get_next_ready_thread();
		if (thread == NULL ) {
			mon_debug("FAILED TO CHANGE TO TASK NULL!!");
			return;
		}
		mon_secure_switch_context(cont, thread);
		break;
	case SSC_NONS_SERVICE:
		// Return to non secure world

		package->result = cont->r[0];
		//v7_flush_dcache_all();
		free_tz_package();
		mon_secure_switch_context(cont, get_nonsecure_thread());
		mon_debug("SSC_NONS_SERVICE context switched!");
		break;
	default:
		mon_debug("Invalid Monitor command: %d", cont->r[12]);
		break;
	}
	dump_mon_context(cont);
	if (thread != NULL ) {
	 mon_debug(
	 "GOING FROM SECURE TO %sSECURE", (thread->context.scr & 1) ? "NON-" : "");
	 } else {
	 mon_debug("GOING FROM SECURE TO (?)SECURE");
	 }
}

void mon_smc_handler(mon_context_t* cont) {
	kprintf_ex("\n");
	mon_debug("====== SECURE MONITOR CALL ========");
	if (cont->scr & 1) {
		// from non-secure
		mon_smc_non_secure_handler(cont);
	} else {
		// from secure
		mon_smc_secure_handler(cont);
	}
	mon_debug("RETURNING FROM SMC ... ");
	mon_debug("======   RETURN FROM SMC   ========");
	//usleep(500);
}

void setup_mon_stacks() {

	com_mem = NULL;
	tee_mem = NULL;
	package = NULL;

	//uint32_t ctr;

	//asm volatile("mrc p15, 0, %0, c0, c0, 1" : "=r" (ctr));
	//cl_size = 4 << ((ctr >> 16) & 0xf);

	cl_size = 0x20;

	mon_info("Cacheline size: %d", cl_size);

	uintptr_t mon_stack_addr = map_stack_mem(0x4000);
	setMONSP(mon_stack_addr);
	//mon_debug("Setup Monitor stack @ 0x%x p 0x%x", mon_stack_addr, v_to_p(mon_stack_addr));
}

void mon_DA(mon_context_t* context) {

	uint32_t s_dfsr = 0xFFFFFFFF;
	uint32_t s_dfar = 0xFFFFFFFF;

	uint32_t n_dfsr = 0xFFFFFFFF;
	uint32_t n_dfar = 0xFFFFFFFF;

	uint32_t clr = 0;
	uint32_t n_vector = 0;

	uint32_t status = 0;

	kprintf_ex("\n");
	mon_info("====== INTERRUPT ========");
	mon_info("WARNING MONITOR DATA ABORT");
	gotoSecure();

	__asm__ __volatile__("MRC   p15, 0, %0, c5, c0, 0": "=r" (s_dfsr)::"memory");
	__asm__ __volatile__("MRC   p15, 0, %0, c6, c0, 0": "=r" (s_dfar)::"memory");

	if((context->scr & 1) == 0) {
		// secure
		mon_error("DATA ABORT FROM SECURE WORLD!");
		dump_mon_context(context);
		kpanic();
	}
	// non secure

	// translate s_dfar to physical location
	uintptr_t phy_s_dfar = virt_ns_to_phys((uintptr_t)s_dfar);

	// check for protected memory access
	pmm_stop_on_protected_mem((uint32_t)phy_s_dfar);

	gotoNSecure();

	__asm__ __volatile__("MRC   p15, 0, %0, c5, c0, 0": "=r" (n_dfsr)::"memory");
	__asm__ __volatile__("MRC   p15, 0, %0, c6, c0, 0": "=r" (n_dfar)::"memory");

	__asm__ __volatile__("MCR   p15, 0, %0, c5, c0, 0":: "r" (s_dfsr):"memory");
	__asm__ __volatile__("MCR   p15, 0, %0, c6, c0, 0":: "r" (s_dfar):"memory");

	gotoSecure();

	mon_info("Moved S-DFAR to N-DFAR etc.");
	mon_info("S-DFAR: 0x%x S-DFSR: 0x%x", s_dfar, s_dfsr);
	mon_info("N-DFAR: 0x%x N-DFSR: 0x%x", n_dfar, n_dfsr);

	status = (s_dfsr & DFSR_STATUS)
			| ((s_dfsr & DFSR_STATUS_1)>> 6)|((s_dfsr & DFSR_SD)>> 7);

	mon_info("STATUS: %s [%x]", get_da_status(status), status);

	mon_info("Failed to %s 0x%x @ INSTR: 0x%x",
			((s_dfsr & DFSR_RW) >> 11) ? "write to" : "read from", s_dfar,
			context->pc);

	mon_info("DFSR: 0x%x  STAT: 0x%x  DFAR: 0x%x", s_dfsr, status, s_dfar);

	mon_info("  RW: %x (%c)  SD (AXI): %x  DOMAIN: 0x%x",
			((s_dfsr & DFSR_RW) >> 11), ((s_dfsr & DFSR_RW) >> 11) ? 'W' : 'R',
			((s_dfsr & DFSR_SD) >> 12), ((s_dfsr & DFSR_DOMAIN) >> 4));

	mon_info("Register dump:");

	dump_mon_context(context);

	gotoNSecure();

	// MRC p15, 0, <Rd>, c1, c0, 0 wenn bit 13 -> FFFF0000
	__asm__ __volatile__("MRC   p15, 0, %0, c1, c0, 0": "=r" (clr)::"memory");

	// read MRC p15, 0, <Rd>, c12, c0, 0 ->
	__asm__ __volatile__("MRC   p15, 0, %0, c12, c0, 0": "=r" (n_vector)::"memory");

	gotoSecure();

	mon_info("NonSecure Vector: 0x%x", n_vector);

	restoreABT(context->pc+8, context->cpsr);

	if(CHECK_BIT(clr, 13)) {
		n_vector = 0xFFFF0000;
	}

	n_vector += 0x10;

	mon_info("NonSecure Vector done: 0x%x", n_vector);

	context->pc = n_vector;

	context->cpsr = ABT_MODE | PSR_I | PSR_A;

	if(CHECK_BIT(clr, 30)) {
		context->cpsr |= PSR_T;
	}

	mon_info("====== INTERRUPT ========");
}



