/*
 * linux_payload.c
 *
 *  Created on: Jun 12, 2013
 *      Author: andy
 */

#include <mm/mm.h>
#include <hal.h>
#include <task/task.h>

extern uint32_t payload;
extern uint32_t payload_end;

task_t* load_linux() {
	// prepare payload ...
	uint32_t vpayload_end = (uint32_t) &payload_end;
	uint32_t vpayload = (uint32_t) &payload;
	uint32_t payload_size = vpayload_end - vpayload;

	if (payload_load_data((void*) vpayload, (void*) 0x70008000, payload_size)
			!= 0) {
		main_error("Failed to load Linux kernel data!");
		return (NULL );
	}

	main_debug("ATAGS ...");

	uintptr_t vatag_ptr = map_phys_mem((uintptr_t) 0x70000000,
			SMALL_PAGE_SIZE - 1, AP_SVC_RW_USR_NO, 1, 1, 0);

	if (vatag_ptr == NULL ) {
		kpanic();
	}
	vatag_ptr += 0x40;
	uint8_t* dst = (uint8_t*) vatag_ptr;

	atag_generate_nonsecure((uintptr_t) dst, "", 0, 0);
	main_info("Setup ATAGS:");
	atag_dump((struct atag*) dst);
	kdumpMem(dst, 100);

	//src = vdtb;

	//main_debug( "Copying ATAGS to 0x%x (p 0x%x)", dst, v_to_p(dst));

	//uint32_t* value = 0xC0008000;

	main_info("%s PREPARE PAYLOAD [DONE] %s", SEPERATOR, SEPERATOR);

	task_t* task = create_kernel_task(SVC_MODE, NONSECURE);

	if (task == NULL ) {
		main_error("Failed to create nonsecure task");
		return (NULL );
	}

	task->context.r[1] = hal_get_platform()->sys_id;
	task->context.r[2] = 0x70000100;
	task->context.pc = 0x70008000;

	task_set_name(task, "NONSECURE_LINUX");

	add_task(task);

	set_nonsecure_task(task);

	return (task);
}
