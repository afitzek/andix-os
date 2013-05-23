/*
 * monitor.c
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <monitor/monitor.h>
#include <mm/mm.h>
#include <task/task.h>
#include <cache.h>

task_t* returningTask;
task_t* returningTaskTEE;

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

void mon_secure_switch_context(mon_context_t *regs, task_t *task) {
	// save old context ...
	task_t* current = get_current_task();

	if (current != NULL ) {
		//mon_debug("Saving task %s [%d]", current->name, current->tid);
		memcpy(&current->context, regs, sizeof(mon_context_t));
		monitor_save_sys_regs(&current->sys_context);
		current->kernelSP = getSVCSP();
		current->userSP = getSYSSP();
		current->userPD = get_user_table();
		// TODO: Save different mode stacks cpsr etc
		gotoMONMode();
	}

	//mon_debug("Loading task %d [%s] from: 0x%x", task->tid, task->name, task);

	//mon_debug("Restoring context");

	// Restore the new context
	monitor_restore_sys_regs(&task->sys_context);
	memcpy(regs, &task->context, sizeof(mon_context_t));

	//mon_debug("Current Mode: %s", getModeString(getCPSR()));

	if (task->kernelSP != 0) {
		//mon_debug("setting kernel stack to 0x%x", task->kernelSP);
		setSVCSP((uintptr_t) task->kernelSP);
		gotoMONMode();
	}

	if (task->userSP != 0) {
		//mon_debug("setting user stack to 0x%x", task->userSP);
		setSYSSP((uintptr_t) task->userSP);
		gotoMONMode();
	}

	if (task->userPD != NULL ) {
		//mon_debug("setting user page directory to 0x%x", task->userPD);
		set_user_table(task->userPD);
		invalidate_tlb();
	}

	//mon_debug("Current Mode: %s", getModeString(getCPSR()));

	set_current_task(task);

	//mon_debug("ID of task %d:", task->tid);
}

uint32_t create_session(uint32_t paddr) {
	task_t* task = NULL;
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
	task = get_task_by_uuid(uuid);
	//task = get_task_by_id(2);
	if (task == NULL ) {
		task = get_task_by_id(1);
	}
	if (task != NULL ) {
		task->state = READY;
		__smc_1(SSC_TASK_SWITCH, (uint32_t) task);
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
		mon_info("Unmapping old communication memory");
		unmap_memory((uint32_t) com_mem, sizeof(TZ_CTLR_SPACE));
		com_mem = NULL;
	}
}

uint32_t cl_size;

void free_tz_tee_memory() {
	if (tee_mem != NULL ) {
		inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
		mon_info("Unmapping old tee memory");
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
		mon_info("Unmapping old package memory");
		unmap_memory((uint32ptr_t) package, sizeof(TZ_PACKAGE));
		package = NULL;
	}
	free_tz_tee_memory();
	free_tz_communication_memory();
}

int set_tz_package(void* ptr) {
	free_tz_package();

	package = (TZ_PACKAGE*) map_phys_mem((uintptr_t) ptr, sizeof(TZ_PACKAGE),
			AP_SVC_RW_USR_NO, 0, 0, 1);

	if (package == NULL ) {
		mon_error("Failed to map package memory!");
		return (-1);
	}

	inv_tz_memory(package, sizeof(TZ_PACKAGE));

	if (set_tz_tee_memory(package->physical_tee) != 0) {
		free_tz_package();
		return (-1);
	}

	if (set_tz_communication_memory(package->physical_ctrl) != 0) {
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
	task_t* target_task = NULL;
	uint32_t physical_package;
	mon_debug("FROM NON SECURE");
	dump_mon_context(cont);
	switch (cont->r[12]) {
	case SMC_PROCESS_CMEM:
		physical_package = cont->r[0];
		set_tz_package((void*) physical_package);

		com_mem = mon_get_control_space();

		// THIS REQUEST IS A RESPONSE => DISPATCH TO SERVICE TASK!
		mon_info("Process com memory! @ v 0x%x p 0x%x", com_mem,
				virt_to_phys((uintptr_t)com_mem));

		mon_info("CTRL RESULT: 0x%x", com_mem->ret);

		if (com_mem == NULL ) {
			cont->r[0] = -1;
		} else {
			inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
			inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));
			inv_tz_memory(package, sizeof(TZ_PACKAGE));
			mon_info("COM MEMORY IN TZ:");
			kprintHex(com_mem, sizeof(TZ_CTLR_SPACE));
			get_current_task()->state = BLOCKED;
			target_task = get_task_by_name(SERVICE_TASK);
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
		mon_info("Process tee memory! @ v 0x%x p 0x%x", tee_mem,
				virt_to_phys((uintptr_t)tee_mem));

		if (tee_mem == NULL ) {
			cont->r[0] = -1;
		} else {
			inv_tz_memory(tee_mem, sizeof(TZ_TEE_SPACE));
			inv_tz_memory(com_mem, sizeof(TZ_CTLR_SPACE));
			inv_tz_memory(package, sizeof(TZ_PACKAGE));
			mon_info("TEE MEMORY IN TZ:");
			kprintHex(tee_mem, sizeof(TZ_TEE_SPACE));
			//DEBUG_STOP;
			target_task = get_task_by_name(TEE_TASK);
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

	/*if (target_task != NULL ) {
	 mon_debug(
	 "GOING FROM SECURE TO %sSECURE", (target_task->context.scr & 1) ? "NON-" : "");
	 } else {
	 mon_debug("GOING FROM SECURE TO (?)SECURE");
	 }*/
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
	task_t* task = NULL;
	uint32_t tmp1;
	mon_debug("ACTION: %d [%s]", cont->r[12], get_ssc_name(cont->r[12]));
	switch (cont->r[12]) {
	case SSC_TASK_SWITCH:
		task = (task_t*) cont->r[0];
		if (task == NULL ) {
			mon_debug("FAILED TO CHANGE TO TASK NULL!!");
			return;
		}
		mon_info("Switching to %s", task->name);
		mon_secure_switch_context(cont, task);
		break;
	case SSC_TASK_SCHEDULE:
		task = task_get_next_ready();
		if (task == NULL ) {
			mon_debug("FAILED TO CHANGE TO TASK NULL!!");
			return;
		}
		mon_secure_switch_context(cont, task);
		break;
	case SSC_NONS_SERVICE:
		// Return to non secure world

		package->result = cont->r[0];
		//v7_flush_dcache_all();
		free_tz_package();
		mon_secure_switch_context(cont, get_nonsecure_task());
		mon_info("SSC_NONS_SERVICE context switched!");
		cont->r[0] = tmp1;
		break;
	default:
		mon_debug("Invalid Monitor command: %d", cont->r[12]);
		break;
	}
	dump_mon_context(cont);
	/*if (task != NULL ) {
	 mon_debug(
	 "GOING FROM SECURE TO %sSECURE", (task->context.scr & 1) ? "NON-" : "");
	 } else {
	 mon_debug("GOING FROM SECURE TO (?)SECURE");
	 }*/
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

	uint32_t ctr;

	//asm volatile("mrc p15, 0, %0, c0, c0, 1" : "=r" (ctr));
	//cl_size = 4 << ((ctr >> 16) & 0xf);

	cl_size = 0x20;

	mon_info("Cacheline size: %d", cl_size);

	returningTask = NULL;
	returningTaskTEE = NULL;
	uintptr_t mon_stack_addr = map_stack_mem(0x4000);
	setMONSP(mon_stack_addr);
	//mon_debug("Setup Monitor stack @ 0x%x p 0x%x", mon_stack_addr, v_to_p(mon_stack_addr));
}

