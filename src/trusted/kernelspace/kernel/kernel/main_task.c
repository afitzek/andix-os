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
#include <scheduler.h>
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

	struct thread_t *service = create_kernel_thread(SVC_MODE, SECURE);

	if (service == NULL ) {
		main_error("Failed to create service task");
		kpanic();
	}

	thread_set_entry(service, service_entry);
	sched_add_thread(service);
	service_pre(service);
	yield();

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

	struct thread_t* tee = create_kernel_thread(SVC_MODE, SECURE);

	if (tee == NULL ) {
		main_error("Failed to create process task");
		kpanic();
	}

	thread_set_entry(tee, tee_task_entry);
	sched_add_thread(tee);
	yield();

	main_info("%s STARTING TEE TASKS [DONE] %s", SEPERATOR, SEPERATOR);

	// ========================================================================

	main_info("%s PREPARING SECURE USERSPACE TASKS %s", SEPERATOR, SEPERATOR);

	struct user_process_t* sampleuser = create_static_userspace_trustlet(
			(uintptr_t) &_samplepayload, (uintptr_t) &_samplepayload_end,
			"SAMPLE TRUSTLET", "7e58ce53-0ff2-4356-b1bd-cf81b708c6d5");
	yield();

	struct user_process_t* rsatrustlet = create_static_userspace_trustlet(
			(uintptr_t) &_rsapayload, (uintptr_t) &_rsapayload_end,
			"RSA TRUSTLET", "2b0c6603-ac6d-4817-a102-ebef1ae0f846");
	yield();

	main_info("TASK UUID: %s", "47b57610-925b-11e2-9e96-0800200c9a66");
	main_info("TASK UUID: %s", "7e58ce53-0ff2-4356-b1bd-cf81b708c6d5");

	print_threads();
	//DEBUG_STOP;

	main_info("%s PREPARING SECURE USERSPACE TASKS [DONE] %s", SEPERATOR,
			SEPERATOR);

	//aes_self_test(1);

	//DEBUG_STOP;

	decision = 'n';

	while (decision != 'a' && decision != 'l' && decision != 't') {

		kprintf("\nSelect Guest (a ... Android, l ... Linux) or start Andix tester (t): ");

		decision = getchar();
	}

	struct thread_t* mainthread = NULL;

	if(decision == 'a') {
		kprintf("\nLoading Android ...\n");
		mainthread = load_android();
	} else if (decision == 't') {
		kprintf("\nLoading Andix tester ...\n");
		mainthread = load_tester();
	} else {
		kprintf("\nLoading Linux ...\n");
		mainthread = load_linux();
	}

	if (mainthread == NULL ) {
		kpanic();
	}

	main_info("%s BOOT NONSECURE [DONE] %s", SEPERATOR, SEPERATOR);

	kprintf("\nReady to start non-secure linux. Press any key to proceed.\n");

	getchar();

	get_current_thread()->state = BLOCKED;

	print_threads();

	yield();

	main_info("%s ENTERING MAIN MONITOR TASK [DONE] %s", SEPERATOR, SEPERATOR);
}
