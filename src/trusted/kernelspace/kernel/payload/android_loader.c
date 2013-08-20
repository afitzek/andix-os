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
 * android_payload.c
 *
 *  Created on: Jun 12, 2013
 *      Author: andy
 */

#include <task/task.h>
#include <mm/mm.h>
#include <hal.h>

extern uint32_t payload_and;
extern uint32_t payload_and_end;

extern uint32_t payload_and_rd;
extern uint32_t payload_and_rd_end;


#define ANDROID_RD_LOCATION 0x71D00000
#define ANDROID_BOOTARGS_VGA "console=ttymxc0,115200 di1_primary video=mxcdi1fb:GBR24,VGA-XGA vga init=/init androidboot.console=ttymxc0 jtag=on mem=768M"
#define ANDROID_BOOTARGS_LVDS "console=ttymxc0,115200 di0_primary video=mxcdi0fb:RGB666,XGA ldb=di0 init=/init androidboot.console=ttymxc0 jtag=on mem=768M"


task_t* load_android() {
	// prepare payload ...
	uint32_t android_kernel_end = (uint32_t) &payload_and_end;
	uint32_t android_kernel = (uint32_t) &payload_and;
	uint32_t android_kernel_size = android_kernel_end - android_kernel;

	if (payload_load_data((void*) android_kernel, (void*) 0x70008000,
			android_kernel_size) != 0) {
		main_error("Failed to load Android kernel data!");
		return (NULL );
	}

	uint32_t android_ramdisk_end = (uint32_t) &payload_and_rd_end;
	uint32_t android_ramdisk = (uint32_t) &payload_and_rd;
	uint32_t android_ramdisk_size = android_ramdisk_end - android_ramdisk;

	if (payload_load_data((void*) android_ramdisk, (void*) ANDROID_RD_LOCATION,
			android_ramdisk_size) != 0) {
		main_error("Failed to load Android ramdisk data!");
		return (NULL );
	}

	main_debug("ATAGS ...");

	uintptr_t vatag_ptr = map_phys_mem((uintptr_t) 0x70000000,
			SMALL_PAGE_SIZE - 1, AP_SVC_RW_USR_NO, 1, 1, 0);

	if (vatag_ptr == NULL ) {
		main_error("Failed to map ATAGS!");
		return (NULL );
	}

	vatag_ptr += 0x40;
	uint8_t* dst = (uint8_t*) vatag_ptr;

	atag_generate_nonsecure((uintptr_t) dst,
			ANDROID_RD_LOCATION, android_ramdisk_size, ANDROID_BOOTARGS_LVDS);

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

	atag_dump((atag_t*)vatag_ptr);

	return (task);
}
