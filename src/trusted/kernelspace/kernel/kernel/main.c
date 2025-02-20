/*
 * main.c
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/typedefs.h>
#include <kprintf.h>
#include <mm/mm.h>
#include <common/atags.h>
#include <common.h>
#include <platform/board.h>
#include <hal.h>
#include <monitor/monitor.h>
#include <task/thread.h>
#include <scheduler.h>
#include <platform/vector_debug.h>
#include <devices/interrupt_controller/interrupt_controller.h>
#include <devices/tzmem_controller/tzmem_controller.h>
#include <arch_irq.h>

#ifdef SHOW_MEM_LAYOUT
extern uint32_t _data;
extern uint32_t _data_end;
extern uint32_t _code_end;
extern uint32_t _rodata;
extern uint32_t _rodata_end;
extern uint32_t _bss;
extern uint32_t _bss_end;
extern uint32_t svc_stack;
extern uint32_t __phys_target_addr;
extern uint32_t __virt_load_addr;
extern uint32_t __kernel_mb_size;
extern uint32_t payload;
extern uint32_t payload_end;
extern uint32_t dtb;
extern uint32_t dtb_end;
extern uint32_t __symbol_table;
extern uint32_t __symbol_table_end;
#endif

extern uint32_t __svc_vector;
extern uint32_t __monitor_vector;
extern uint32_t _code;
extern uint32_t _end;

void dummy_irq_handler(int irq) {
	main_info("dummy_irq_handler: %d", irq);
	irq_clear(irq);
}

/**
 * Entrypoint for main kernel task
 */
void entry_main_task();

/**
 * Main entry point
 * @param atagparam ATAG Parameters Pointer
 * @param systemID Plattform ID
 */
void entry(uint32_t atagparam, uint32_t systemID) {
	uint32_t sysid = systemID;

	uint32_t atag_base = atagparam;

	uint32_t vectors = 0xFFFFFFFF;
	uint32_t _svc_vect = (uint32_t) &__svc_vector;
	uint32_t _mon_vect = (uint32_t) &__monitor_vector;
	uint32_t* v_load_addr = (uint32_t*) __virt_load_addr;
	//uint32_t i = 0;

	// ========================================================================
	init_serial(sysid);

	//mputchar('c');
	// ========================================================================
	main_info(
			"============================================================================");
	main_info("");
#ifdef VERSION
	main_info("      ANDIX (%s)", VERSION);
#ifdef USER
#ifdef BUILD_DATE
	main_info("      compiled by %s @ %s ", USER, BUILD_DATE);
#else
	main_info("      compiled by %s", USER);
#endif
#endif
#ifdef GITHASH
	main_info("      GIT Commit %s", GITHASH);
#endif
#ifdef BUILD_NUM
	main_info("      build number: %s", BUILD_NUM);
#endif
#endif
	main_info("");
	main_info(
			"============================================================================");

	// Show Copyright information
	main_info("");
	main_info("      ANDIX is an ARM TrustZone monitor operating system.");
	main_info("      ");
	main_info("      Copyright (c) 2012-2013, Andreas Fitzek");
	main_info("      All rights reserved.");
	main_info("      ");
	main_info("      Redistribution and use in source and binary forms, ");
	main_info("      with or without modification, are permitted provided ");
	main_info("      that the following conditions are met:");
	main_info("      Redistributions of source code must retain the above ");
	main_info("      copyright notice, this list of conditions and the following ");
	main_info("      disclaimer. Redistributions in binary form must reproduce ");
	main_info("      the above copyright notice, this list of conditions and ");
	main_info("      the following disclaimer in the documentation and/or ");
	main_info("      other materials provided with the distribution. ");
	main_info("      Neither the name of the nor the names of its contributors");
	main_info("      may be used to endorse or promote products derived from ");
	main_info("      this software without specific prior written permission. ");
	main_info("      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND ");
	main_info("      CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, ");
	main_info("      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF ");
	main_info("      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE ");
	main_info("      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ");
	main_info("      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, ");
	main_info("      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT ");
	main_info("      NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; ");
	main_info("      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ");
	main_info("      HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN ");
	main_info("      CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE ");
	main_info("      OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS ");
	main_info("      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
	main_info("      ");
	main_info(
			"============================================================================");

#ifdef SHOW_MEM_LAYOUT
	// ========================================================================
	// MEMORY LAYOUT OUTPUT
	// ========================================================================
	main_info("%s STATIC MEMORY LAYOUT %s", SEPERATOR, SEPERATOR);
	main_debug(
			"Code:      0x%x - 0x%x [%d byte]", &_code, &_code_end,
			(&_code_end - &_code) * 4);
	main_debug(
			"Data:      0x%x - 0x%x [%d byte]", &_data, &_data_end,
			(&_data_end - &_data) * 4);
	main_debug(
			"ROData:    0x%x - 0x%x [%d byte]", &_rodata, &_rodata_end,
			(&_rodata_end - &_rodata) * 4);
	main_debug(
			"BSS:       0x%x - 0x%x [%d byte]", &_bss, &_bss_end,
			(&_bss_end - &_bss) * 4);
	main_debug(
			"SYM:       0x%x - 0x%x [%d byte]", &__symbol_table,
			&__symbol_table_end, (&__symbol_table_end - &__symbol_table) * 4);
	main_debug(
			"Payload:   0x%x - 0x%x [%d byte]", &payload, &payload_end,
			(&payload_end - &payload) * 4);
	main_debug(
			"DTB:       0x%x - 0x%x [%d byte]", &dtb, &dtb_end,
			(&dtb_end - &dtb) * 4);
	main_debug("svc_stack: 0x%x", &svc_stack);
	main_debug("virt addr: 0x%x", __virt_load_addr);
	main_debug("phys addr: 0x%x", __phys_target_addr);
	main_debug("k MB size: 0x%x", __kernel_mb_size);
	main_debug("_end:      0x%x", &_end);

	main_info("%s STATIC MEMORY LAYOUT [DONE] %s", SEPERATOR, SEPERATOR);
	// ========================================================================
#endif
	// ========================================================================
	// BOOT PARAMETERS
	// ========================================================================
	main_info("%s BOOT PARAMETER %s", SEPERATOR, SEPERATOR);

	main_debug("SysID: 0x%x", sysid);

	main_debug("ATAGS @ physical 0x%x", atag_base);

	atag_base = map_atags((uintptr_t) atag_base);

	main_debug("ATAGS @ virtual 0x%x", atag_base);

	struct atag* atags = (struct atag*) atag_base;
	if (atags->hdr.tag == ATAG_CORE) {
		main_debug("ATAGS seem OK!");
	} else {
		main_debug("ATAGS not starting with ATAG_CORE!");
	}

	unmap_virtual_phys_kern();

	main_info("%s BOOT PARAMETER [DONE] %s", SEPERATOR, SEPERATOR);
	// ========================================================================

	// ========================================================================
	// MEMORY MANAGEMENT INITIALIZATION
	// ========================================================================
	main_info("%s MEMORY MANAGEMENT %s", SEPERATOR, SEPERATOR);

	init_vmm((struct atag*) atag_base);

	main_info("%s MEMORY MANAGEMENT [DONE] %s", SEPERATOR, SEPERATOR);

	main_info("%s VIRTUAL MEMORY LAYOUT %s", SEPERATOR, SEPERATOR);

	dump_kernel_mmu((uint32_t) &_code, (uint32_t) &_end);

	dump_mmu_full();

	main_info("%s VIRTUAL MEMORY LAYOUT [DONE] %s", SEPERATOR, SEPERATOR);

	main_info("%s EXCEPTION VECTORS %s", SEPERATOR, SEPERATOR);

	setup_mode_stacks();
	setup_mon_stacks();

	main_debug("Setting SVC Exception vectors to: 0x%x", _svc_vect);

	__asm__ __volatile__("MCR p15, 0, %0, c12, c0, 0":: "r" ((_svc_vect)) :
			"memory", "cc");

	main_debug("Setting MONITOR Exception vectors to: 0x%x", _mon_vect);

	__asm__ __volatile__("MCR p15, 0, %0, c12, c0, 1":: "r" ((_mon_vect)) :
			"memory", "cc");

	__asm__ __volatile__("MRC p15, 0, %0, c12, c0, 1": "=r" (vectors)::
			"memory");

	main_debug("VECTORS: 0x%x", vectors);

	main_info("%s EXCEPTION VECTORS [DONE] %s", SEPERATOR, SEPERATOR);

	atag_base = (uint32_t) atag_copy((struct atag*) atag_base);

	// Unmap old ATAGS
	unmap_memory((uintptr_t) atags, 0x100);

	atags = (struct atag*) atag_base;
	main_info("Atags copied to : @ 0x%x", atag_base);
	atag_dump(atags);
	//arm_irq_enable();
	// ========================================================================

	// ========================================================================
	// HAL SYSTEM
	// ========================================================================
	main_info("%s HAL SYSTEM %s", SEPERATOR, SEPERATOR);

	hal_init(systemID, (atag_t*) atag_base);

	clean_user();

	random_init();

	irq_init();
	//wdog_init();

	send_fiq_irq_to_monitor();
	unlock_nsacr();
	unlock_aux();
	enable_irq();
	enable_fiq();

	if (tzmem_init() != HAL_SUCCESS) {
		main_info("No TZ memory controller booting anyway!");
	} else {

		tzmem_dump();

		if (pmm_protect_secure_mem() != 0) {
			main_error("=====================================================");
			main_error("CANNOT PERFORM SECURE BOOT!");
			main_error("FAILED TO PROTECT ALL SECURE MEMORY!");
			main_error("=====================================================");
			kpanic();
		}

		tzmem_dump();

	}
	main_info("%s HAL SYSTEM [DONE] %s", SEPERATOR, SEPERATOR);
	// ========================================================================

	// ========================================================================
	// CREATE MAIN KERNEL TASK
	// ========================================================================
	main_info("%s STARTING MAIN KERNEL TASK %s", SEPERATOR, SEPERATOR);

	sched_init();
	init_scheduler();

	struct thread_t *main_thread = create_kernel_thread(SVC_MODE, SECURE);
	thread_set_entry(main_thread, entry_main_task);
	sched_add_thread(main_thread);

	print_threads();

	main_info("Protected MEM 0x%x @ 0x%x", (*v_load_addr), __phys_target_addr);

	switch_to_thread(main_thread);

	main_info("%s STARTING MAIN KERNEL TASK [DONE] %s", SEPERATOR, SEPERATOR);
	main_info("%s SHOULD NOT BE HERE FREEZING SYSTEM ... %s", SEPERATOR,
			SEPERATOR);
	DEBUG_STOP;
	// ========================================================================
}
