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
 * ctrl_mem.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <tz_application_mod/ctrl_mem.h>
#include <tz_application_mod/andixtee.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/cred.h>
#include <linux/uaccess.h>

TZ_CTLR_SPACE* shared_ctrl_mem = NULL;
int ctrl_init = 0;
TZ_CTLR_SPACE* ctrlspace;
int pending = CTRL_IDLE;
struct mutex ctrl_mutex;

/**
 * Polls the ctlr_space from com mem to userspace
 *
 * CTRL_PENDING_FROM_S -> CTRL_PENDING_IN_NS
 */
int poll_ctrl_task_to_ns(TZ_CTLR_SPACE* userspace) {

	int result = 0;

	// check if ctrl system is initialized
	if (ctrl_init == 1) {
		mutex_lock(&ctrl_mutex);
		// check if ctrl system is initialized
		if (ctrl_init == 1) {

			if (pending == CTRL_PENDING_FROM_S) {
				printk(KERN_INFO "GOT CTRL SPACE to non secure daemon ....");
				result = 1;
				if (copy_to_user(userspace, ctrlspace,
						sizeof(TZ_CTLR_SPACE))) {
					printk(KERN_INFO "Failed to copy ctrl request to userspace\n");
					return (-1);
				}
				pending = CTRL_PENDING_IN_NS;
			}
		}
		mutex_unlock(&ctrl_mutex);
	}

	return result;
}

/**
 * Pushes the ctlr_space from userspace to comm mem
 *
 * CTRL_PENDING_IN_NS -> CTRL_RESPONSE_FROM_NS
 */
int push_ctrl_task_from_ns(TZ_CTLR_SPACE* userspace) {

	int result = 0;

	// check if ctrl system is initialized
	if (ctrl_init == 1) {
		mutex_lock(&ctrl_mutex);
		// check if ctrl system is initialized
		if (ctrl_init == 1) {

			if (pending == CTRL_PENDING_IN_NS) {
				printk(KERN_INFO "PUSH CTRL SPACE response from ns ...");
				result = 1;
				if (copy_from_user(ctrlspace, userspace,
						sizeof(TZ_CTLR_SPACE))) {
					printk(KERN_INFO "Failed to copy ctrl request to kernel\n");
					return (-1);
				}
				pending = CTRL_RESPONSE_FROM_NS;
			}
		}
		mutex_unlock(&ctrl_mutex);
	}

	return result;
}

/**
 * Pushes the ctlr_space from com mem to ctrlspace
 *
 * CTRL_IDLE -> CTRL_PENDING_FROM_S
 */
int push_ctrl_task_from_s(TZ_CTLR_SPACE* space) {
	int result = 0;

	// check if ctrl system is initialized
	if (ctrl_init == 1) {
		mutex_lock(&ctrl_mutex);
		// check if ctrl system is initialized
		if (ctrl_init == 1) {
			if (pending == CTRL_IDLE) {
				result = 1;
				printk(KERN_INFO "SET CTRL SPACE ....");
				//memcpy(&ctrlspace, shared_ctrl_mem, sizeof(TZ_CTLR_SPACE));
				ctrlspace = space;
				pending = CTRL_PENDING_FROM_S;
			}
		}
		mutex_unlock(&ctrl_mutex);
	}

	return result;
}

/**
 * Polls the ctlr_space from ctrl_space to com mem
 *
 * CTRL_RESPONSE_FROM_NS -> CTRL_IDLE
 */
int poll_ctrl_task_to_s() {

	int result = 0;

	// check if ctrl system is initialized
	if (ctrl_init == 1) {
		mutex_lock(&ctrl_mutex);
		// check if ctrl system is initialized
		if (ctrl_init == 1) {
			if (pending == CTRL_RESPONSE_FROM_NS) {
				printk(KERN_INFO "POLL FOR SECURE OK ...");
				result = 1;
				//memcpy(shared_ctrl_mem, &ctrlspace, sizeof(TZ_CTLR_SPACE));
				pending = CTRL_IDLE;
			}
		}
		mutex_unlock(&ctrl_mutex);
	}

	return result;
}

int check_process_is_root() {
	uid_t euid = current_euid();
	if (euid == 0) {
		return (1);
	}
	return (0);
}

int register_ctrl_mem_in_tz(TZ_CTLR_SPACE *com) {
	uint32_t paddr = virt_to_phys((void*) com);
	uint32_t res = 0;
	printk(KERN_INFO "Registering ctrl memory virtual 0x%x physical 0x%x",
			(unsigned int) com, paddr);
	res = __smc_1(SMC_REGISTER_CMEM, paddr);
	return ((int) res);
}

int unregister_ctrl_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering ctrl memory");
	res = __smc_1(SMC_UNREGISTER_CMEM, 0);
	return ((int) res);
}

void cleanup_ctrl() {
	if (ctrl_init == 1 && shared_ctrl_mem != NULL ) {
		mutex_lock(&ctrl_mutex);
		unregister_ctrl_mem_from_tz();
		kfree(shared_ctrl_mem);
		shared_ctrl_mem = NULL;
		ctrl_init = 0;
		mutex_unlock(&ctrl_mutex);
		mutex_destroy(&ctrl_mutex);
	}
}

int initialize_ctrl() {
	// Allocate memory
	/*shared_ctrl_mem = (TZ_CTLR_SPACE*) kmalloc(sizeof(TZ_CTLR_SPACE),
			GFP_KERNEL);

	if (shared_ctrl_mem == NULL ) {
		printk(KERN_ERR "Failed to allocate ctrl memory\n");
		return (-1);
	}

	printk(KERN_INFO "CTRL memory @ 0x%x p (0x%x)\n",
			(unsigned int)shared_ctrl_mem, virt_to_phys(shared_ctrl_mem));

	if (register_ctrl_mem_in_tz(shared_ctrl_mem) != 0) {
		kfree(shared_ctrl_mem);
		printk(KERN_ERR "Failed to register ctrl memory in tz\n");
		return (-1);
	}
	*/
	mutex_init(&ctrl_mutex);

	ctrl_init = 1;

	return (0);
}
