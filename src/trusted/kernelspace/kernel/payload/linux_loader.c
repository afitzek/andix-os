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
 * linux_payload.c
 *
 *  Created on: Jun 12, 2013
 *      Author: andy
 */

#include <mm/mm.h>
#include <hal.h>
#include <task/thread.h>
#include <loader.h>
#include <scheduler.h>

extern uint32_t payload;
extern uint32_t payload_end;

struct thread_t* load_linux() {
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

	atag_generate_nonsecure((uintptr_t) dst, 0, 0, NULL);
	main_info("Setup ATAGS:");
	atag_dump((struct atag*) dst);
	//kdumpMem(dst, 100);

	//src = vdtb;

	//main_debug( "Copying ATAGS to 0x%x (p 0x%x)", dst, v_to_p(dst));

	//uint32_t* value = 0xC0008000;

	main_info("%s PREPARE PAYLOAD [DONE] %s", SEPERATOR, SEPERATOR);

	struct thread_t *thread = create_kernel_thread(SVC_MODE, NONSECURE);

	if (thread == NULL ) {
		main_error("Failed to create nonsecure thread");
		return (NULL );
	}

	thread->context.r[1] = hal_get_platform()->sys_id;
	thread->context.r[2] = 0x70000100;
	thread->context.pc = 0x70008000;

	sched_add_thread(thread);

	set_nonsecure_thread(thread);

	return (thread);
}
