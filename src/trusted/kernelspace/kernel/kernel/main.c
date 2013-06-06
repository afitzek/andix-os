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
#include <task/task.h>
#include <scheduler.h>
#include <platform/vector_debug.h>

#ifdef SHOW_MEM_LAYOUT
extern uint32_t _data;
extern uint32_t _data_end;
extern uint32_t _code_end;
extern uint32_t _rodata;
extern uint32_t _rodata_end;
extern uint32_t _bss;
extern uint32_t _bss_end;
extern uint32_t svc_stack;
extern uint32_t __phys_load_addr;
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
 * Main entry point
 * @param atagparam ATAG Parameters Pointer
 * @param systemID Plattform ID
 */
void entry(uint32_t atagparam, uint32_t systemID) {
	uint32_t sysid = systemID;

	uint32_t atag_base = atagparam;

	uint32_t cp15 = 0xFFFFFFFF;
	uint32_t vectors = 0xFFFFFFFF;
	uint32_t _svc_vect = (uint32_t) &__svc_vector;
	uint32_t _mon_vect = (uint32_t) &__monitor_vector;
	uint32_t* csu = (uint32_t*) 0x63F9C000;
	uintptr_t csu_base;
	uint32_t* v_load_addr = (uint32_t*)__virt_load_addr;
	clk_request_t clk_request;
	//uint32_t i = 0;

	// ========================================================================
	init_serial();
	init_early_task();
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
	main_info(
			"      Copyright (C) 2013  Andreas Fitzek <andreas.fitzek[at]iaik.tugraz.at>");
	main_info("");
	main_info(
			"      This system is free software: you can redistribute it and/or modify");
	main_info(
			"      it under the terms of the GNU General Public License as published by");
	main_info(
			"      the Free Software Foundation, either version 3 of the License, or");
	main_info("      (at your option) any later version.");
	main_info("");
	main_info(
			"      This program is distributed in the hope that it will be useful,");
	main_info(
			"      but WITHOUT ANY WARRANTY; without even the implied warranty of");
	main_info(
			"      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
	main_info("      GNU General Public License for more details.");
	main_info("");
	main_info(
			"      You should have received a copy of the GNU General Public License");
	main_info(
			"      along with this program.  If not, see <http://www.gnu.org/licenses/>.");
	main_info("");
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
	main_debug("phys addr: 0x%x", __phys_load_addr);
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
	enable_irq();
	enable_fiq();
	send_fiq_irq_to_monitor();
	uint32_t c = get_cpsr();
	uint32_t s = get_scr();

	main_info("CPSR: 0x%x", c);
	main_info("SCR : 0x%x", s);

	irq_register_handler(109, &dummy_irq_handler);

	platform_device_t* timer = hal_find_device(TIMER_DEVICE, 0);

	if(timer != NULL) {
		if(hal_ioctl(timer, IOCTL_CLOCK_GET_CTR,
				&clk_request, sizeof(clk_request)) != HAL_SUCCESS) {
			main_error("Failed to get counter value!");
		} else {
			main_info("Counter value: 0x%x", clk_request.value);
			clk_request.value = clk_request.value + 1000;
			main_info("Alarm @: 0x%x", clk_request.value);
			hal_ioctl(timer, IOCTL_CLOCK_ALARM_AT, &clk_request, sizeof(clk_request));
			//sleep(2);
			hal_ioctl(timer, IOCTL_CLOCK_GET_CTR, &clk_request, sizeof(clk_request));
			main_info("Counter value: 0x%x", clk_request.value);
		}
	}

	irq_dump();

	//irq_swint(109);

	irq_dump();

	main_info("%s HAL SYSTEM [DONE] %s", SEPERATOR, SEPERATOR);
	// ========================================================================

	// ========================================================================
	// TZ STUFF (TODO: INCLUDE INTO HAL SYSTEM)
	// ========================================================================
	main_info("%s TRUSTZONE STUFF %s", SEPERATOR, SEPERATOR);

	main_debug("Allowing nonsecure access to all devices via CSU");

	csu_base = (uintptr_t) map_io_mem((uintptr_t) 0x63F9C000, 0x80);

	csu = csu_base;

	while (csu < (uint32_t*) ((uint32_t) csu_base + 0x80)) {
		//uint32_t old = (*csu);
		(*csu) = 0x00FF00FF;
		//main_debug("CSU @ 0x%x : 0x%x -> 0x%x", csu, old, (*csu));
		csu++;
	}
/*
	uintptr_t base_tzic = (uintptr_t) map_io_mem((uintptr_t) 0x0FFFC080, 0x80);

	main_debug("TZIC @ 0x%x p (0x%x)", base_tzic, v_to_p(base_tzic));

	uintptr_t tzic = base_tzic + 0x20;

	for (int i = 0; i < 4; i++) {
		main_debug("TZIC SET 0x%X = 0x%X", v_to_p(tzic), 0xFFFFFFFF);
		(*tzic) = 0xFFFFFFFF;
		tzic++;
	}

	tzic = base_tzic + 0x100;

	for (int i = 0; i < 32; i++) {
		main_debug("TZIC SET 0x%X = 0x%X", v_to_p(tzic), 0x80808080);
		(*tzic) = 0x80808080;
		tzic++;
	}

	tzic = base_tzic + 3;
	main_debug("TZIC SET 0x%X = 0x%X", v_to_p(tzic), 0x1F);
	(*tzic) = 0x1f;
*/
	main_debug("Protecting memory ...");
	uint8_t* base_m4if = (uint8_t*) map_io_mem((uintptr_t) 0x63FD8000, 0x1000);
	uint8_t* base_extmc = (uint8_t*) map_io_mem((uintptr_t) 0x63FDBF00, 0x1000);
	uint32_t* wm_start = (uint32_t*) (base_m4if + 0xF0);
	uint32_t* wm_end = (uint32_t*) (base_m4if + 0x110);
	uint32_t* wm_status = (uint32_t*) (base_m4if + 0x114);
	uint32_t* extmc_lock = (uint32_t*) (base_extmc);
	uint32_t extmc = 0;
	__raw_writel(0x0, wm_start);
	__raw_writel(0xbffff, wm_end);
	__raw_writel(0x80000000 | 0xb0000, wm_start);
	__raw_writel(0x80000000, wm_status);
	extmc = __raw_readl(extmc_lock);
	main_debug("extmc_lock: 0x%x", extmc);
	extmc = extmc | 0x8;
	__raw_writel(extmc, extmc_lock);

	main_debug("wm_start: 0x%x", __raw_readl(wm_start));
	main_debug("wm_end: 0x%x", __raw_readl(wm_end));
	main_debug("wm_status: 0x%x", __raw_readl(wm_status));
	main_debug("extmc_lock: 0x%x", __raw_readl(extmc_lock));

	main_debug("Protected memory!");

	/*(*wm_start) = 0x80000000 | 0xb0000;
	(*wm_end) = 0xc0000;
	(*wm_status) = 0x80000000;*/

	__asm__ __volatile__("MRC   p15, 0, %0, c1, c1, 0": "=r" (cp15)::"memory");

	main_debug("SCR: 0x%x", cp15);

	main_info("%s TRUSTZONE STUFF [DONE] %s", SEPERATOR, SEPERATOR);
	// ========================================================================

	// ========================================================================
	// CREATE MAIN KERNEL TASK
	// ========================================================================
	main_info("%s STARTING MAIN KERNEL TASK %s", SEPERATOR, SEPERATOR);

	init_task();
	init_scheduler();

	task_t* main_task = create_kernel_task(SVC_MODE, SECURE);
	main_task->context.pc = (uint32_t) (&(entry_main_task));
	task_set_name(main_task, "MAIN KERNEL TASK");
	add_task(main_task);

	print_tasks();

	main_info("Protected MEM 0x%x @ 0x%x", (*v_load_addr), __phys_load_addr);

	switch_to_task(main_task);

	main_info("%s STARTING MAIN KERNEL TASK [DONE] %s", SEPERATOR, SEPERATOR);
	main_info("%s SHOULD NOT BE HERE FREEZING SYSTEM ... %s", SEPERATOR,
			SEPERATOR);
	DEBUG_STOP;
	// ========================================================================
}
