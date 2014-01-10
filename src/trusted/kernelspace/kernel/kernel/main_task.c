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
 * main_task.c
 *
 *  Created on: Nov 28, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common.h>
#include <kprintf.h>
#include <platform/board.h>
#include <hal.h>
#include <task/task.h>
#include <task/service.h>
#include <task/tee.h>
#include <loader.h>
#include <mm/mm.h>
#include <fs/fs.h>
#include <tropicssl/sha1.h>
#include <tee/tee.h>

extern uint32_t payload;
//extern uint32_t payload_os;
extern uint32_t payload_end;
//extern uint32_t dtb;
//extern uint32_t dtb_end;
//extern uint32_t payload_dtb;
extern uint8_t _userpayload;
extern uint8_t _userpayload_end;
extern uint8_t _samplepayload;
extern uint8_t _samplepayload_end;
extern uint8_t _rsapayload;
extern uint8_t _rsapayload_end;

/**
 * Entrypoint for main kernel task
 */
void entry_main_task() {
	uint8_t pwd_hash[20];
	uint32_t udid;
	sha1_context sha1ctx;
	//cfs_blk_t blk;
	uint32_t crc;
	uint32_t i;
	char passwordbuffer[100];

	tee_initialize();

	//getSerial();

	main_info("%s ENTERING MAIN MONITOR TASK %s", SEPERATOR, SEPERATOR);

	// ========================================================================
	// Reading User input for password
	// ========================================================================

	getSerial();

	int decision = 'n';

	kprintf("%s\n", LINE_SEPERATOR);

	udid = getUDID();

	kprintf("Unique Device ID: 0x%x", udid);

	while (decision != 'y') {

		kprintf("\nPlease enter user password: ");

		memset(passwordbuffer, 0, 100);
		memset(pwd_hash, 0, 20);

		getinput(passwordbuffer, 100);

		sha1_starts(&sha1ctx);
		sha1_update(&sha1ctx, (const unsigned char*) &udid, 8);
		sha1_update(&sha1ctx, (const unsigned char*) passwordbuffer,
				strlen(passwordbuffer));
		sha1_finish(&sha1ctx, pwd_hash);

		//sha1_hmac(passwordbuffer, strlen(passwordbuffer), &digest);

		kprintf("\nGot comparison value: ");

		kprintHex(pwd_hash, 20);

		crc = 0;

		for (i = 0; i < 20; i = i + 4) {
			uint32_t t = (pwd_hash[i] << 24) | (pwd_hash[i + 1] << 16)
					| (pwd_hash[i + 2] << 8) | (pwd_hash[i + 3]);
			crc = crc ^ t;
		}

		kprintf("\nGot comparison value (small): 0x%X", crc);

		kprintf("\nLooks this correct? [y/n]");

		decision = getchar();
	}

	kprintf("\nGot password, if this is incorrect you will have to reboot!\n");

	// Currently using password as secret
	fs_set_secret((uint8_t*) passwordbuffer, strlen(passwordbuffer));

	random_seed_time();

	kprintf("%s\n", LINE_SEPERATOR);

	// ========================================================================

	// ========================================================================
	// STARTING SYSTEM TASKS
	// ========================================================================

	main_info("%s STARTING SERVICE TASKS %s", SEPERATOR, SEPERATOR);

	task_t* service = create_kernel_task(SVC_MODE, SECURE);

	if (service == NULL ) {
		main_error("Failed to create service task");
		kpanic();
	}

	service->context.pc = (uint32_t) (&(service_entry));
	task_set_name(service, SERVICE_TASK);
	add_task(service);
	service_pre(service);

	main_info("%s STARTING SERVICE TASKS [DONE] %s", SEPERATOR, SEPERATOR);
	/*
	 main_info("%s STARTING PROCESS TASKS %s", SEPERATOR, SEPERATOR);

	 task_t* process = create_kernel_task(SVC_MODE, SECURE);

	 if (process == NULL ) {
	 main_error("Failed to create process task");
	 kpanic();
	 }

	 process->context.pc = (uint32_t) (&(process_entry));
	 task_set_name(process, PROCESS_TASK);
	 add_task(process);

	 main_info("%s STARTING PROCESS TASKS [DONE] %s", SEPERATOR, SEPERATOR);
	 */
	main_info("%s STARTING TEE TASKS %s", SEPERATOR, SEPERATOR);

	task_t* tee = create_kernel_task(SVC_MODE, SECURE);

	if (tee == NULL ) {
		main_error("Failed to create process task");
		kpanic();
	}

	tee->context.pc = (uint32_t) (&(tee_task_entry));
	task_set_name(tee, TEE_TASK);
	add_task(tee);

	main_info("%s STARTING TEE TASKS [DONE] %s", SEPERATOR, SEPERATOR);

	// ========================================================================

	main_info("%s PREPARING SECURE USERSPACE TASKS %s", SEPERATOR, SEPERATOR);

	//TASK_UUID uuid;

	// 47b57610-925b-11e2-9e96-0800200c9a66
	/*uuid.timeLow = 0x47b57610;
	 uuid.timeMid = 0x925b;
	 uuid.timeHiAndVersion = 0x11e2;
	 uuid.clockSeqAndNode[7] = 0x9e;
	 uuid.clockSeqAndNode[6] = 0x96;
	 uuid.clockSeqAndNode[5] = 0x08;
	 uuid.clockSeqAndNode[4] = 0x00;
	 uuid.clockSeqAndNode[3] = 0x20;
	 uuid.clockSeqAndNode[2] = 0x0c;
	 uuid.clockSeqAndNode[1] = 0x9a;
	 uuid.clockSeqAndNode[0] = 0x66;*/

	task_t* user = prepare_static_userspace_task((uintptr_t) &_userpayload,
			(uintptr_t) &_userpayload_end, "SYSTEM DAEMON",
			"47b57610-925b-11e2-9e96-0800200c9a66");
	init_userspace_task(user);

	// 47b57610-925b-11e2-9e96-0800200c9a66
	/*uuid.timeLow = 0x47b57610;
	 uuid.timeMid = 0x925b;
	 uuid.timeHiAndVersion = 0x11e2;
	 uuid.clockSeqAndNode[7] = 0x9e;
	 uuid.clockSeqAndNode[6] = 0x96;
	 uuid.clockSeqAndNode[5] = 0x08;
	 uuid.clockSeqAndNode[4] = 0x00;
	 uuid.clockSeqAndNode[3] = 0x20;
	 uuid.clockSeqAndNode[2] = 0x0c;
	 uuid.clockSeqAndNode[1] = 0x9a;
	 uuid.clockSeqAndNode[0] = 0x66;*/

	task_t* sampleuser = prepare_static_userspace_task(
			(uintptr_t) &_samplepayload, (uintptr_t) &_samplepayload_end,
			"SAMPLE TRUSTLET", "7e58ce53-0ff2-4356-b1bd-cf81b708c6d5");
	init_userspace_task(sampleuser);

	task_t* rsatrustlet = prepare_static_userspace_task(
			(uintptr_t) &_rsapayload, (uintptr_t) &_rsapayload_end,
			"RSA TRUSTLET", "2b0c6603-ac6d-4817-a102-ebef1ae0f846");
	init_userspace_task(rsatrustlet);

	main_info("TASK UUID: %s", "47b57610-925b-11e2-9e96-0800200c9a66");
	main_info("TASK UUID: %s", "7e58ce53-0ff2-4356-b1bd-cf81b708c6d5");

	print_tasks();
	//DEBUG_STOP;

	main_info("%s PREPARING SECURE USERSPACE TASKS [DONE] %s", SEPERATOR,
			SEPERATOR);

	//aes_self_test(1);

	//DEBUG_STOP;
/*
	print_tasks();

	main_info("%s BOOT NONSECURE %s", SEPERATOR, SEPERATOR);

	main_info("%s PREPARE PAYLOAD %s", SEPERATOR, SEPERATOR);

	main_debug("KERNEL ...");

	 // prepare payload ...
	 uint32_t vpayload_end = (uint32_t) &payload_end;
	 uint32_t vpayload = (uint32_t) &payload;
	 uint32_t payload_size = vpayload_end - vpayload;

	 uintptr_t vpayload_ptr = map_phys_mem((uintptr_t) 0x70008000, payload_size,
	 AP_SVC_RW_USR_NO, 1, 1, 0);

	 if (vpayload_ptr == NULL ) {
	 kpanic();
	 }

	 uint8_t* src = (uint8_t*) vpayload;
	 uint8_t* dst = (uint8_t*) vpayload_ptr;

	 main_debug("Copying kernel form 0x%x to 0x%x (p 0x%x)", src, dst,
	 v_to_p(dst));

	 for (uint32_t i = 0; i < payload_size; i++) {
	 dst[i] = src[i];
	 }

	 main_debug("ATAGS ...");

	 uintptr_t vatag_ptr = map_phys_mem((uintptr_t) 0x70000000,
	 SMALL_PAGE_SIZE - 1, AP_SVC_RW_USR_NO, 1, 1, 0);

	 if (vatag_ptr == NULL ) {
	 kpanic();
	 }
	 vatag_ptr += 0x40;
	 dst = (uint8_t*) vatag_ptr;

	 atag_generate_nonsecure((uintptr_t) dst, 0, 0);
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
	 kpanic();
	 }

	 task->context.r[1] = hal_get_platform()->sys_id;
	 task->context.r[2] = 0x70000100;
	 task->context.pc = 0x70008000;

	 task_set_name(task, "NONSECURE_LINUX");

	 add_task(task);

	 set_nonsecure_task(task);
*/

	decision = 'n';

	while (decision != 'a' && decision != 'l' && decision != 't') {

		kprintf("\nSelect Guest (a ... Android, l ... Linux) or start Andix tester (t): ");

		decision = getchar();
	}

	task_t* task = NULL;

	if(decision == 'a') {
		kprintf("\nLoading Android ...\n");
		task = load_android();
	} else if (decision == 't') {
		kprintf("\nLoading Andix tester ...\n");
		task = load_tester();
	} else {
		kprintf("\nLoading Linux ...\n");
		task = load_linux();
	}

	if (task == NULL ) {
		kpanic();
	}

	main_info("%s BOOT NONSECURE [DONE] %s", SEPERATOR, SEPERATOR);

	kprintf("\nReady to start non-secure linux. Press any key to proceed.\n");

	getchar();

	get_current_task()->state = BLOCKED;

	print_tasks();

	__smc_1(SSC_TASK_SWITCH, (uint32_t) task);

	main_info("%s ENTERING MAIN MONITOR TASK [DONE] %s", SEPERATOR, SEPERATOR);
}
