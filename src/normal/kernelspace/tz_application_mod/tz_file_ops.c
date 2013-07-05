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
 * tz_file_ops.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_application_mod/andixtee.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/cred.h>
#include <linux/uaccess.h>
#include <andix_tz_mod.h>
#include <tz_application_mod/tee_logic.h>
#include <linux/io.h>

//static atomic_t tz_dev_available = ATOMIC_INIT(1);

int tz_driver_open(struct inode * inode, struct file * file) {
	/*if (!atomic_dec_and_test(&tz_dev_available)) {
		atomic_inc(&tz_dev_available);
		return (-EBUSY); // already open
	}*/
	return (0);
}

int tz_driver_release(struct inode * inode, struct file * file) {
	//atomic_inc(&tz_dev_available); // release the device
	// release all contexts with
	tee_context* ctx = tee_context_find_by_pid(current->pid);
	while(ctx != NULL) {
		// TODO: CALL TZ to release ctx
		tee_context_free(ctx);
		ctx = tee_context_find_by_pid(current->pid);
	}
	return (0);
}

int tz_process_tee_mem(TZ_TEE_SPACE* userspace) {

	int result = 0;

	//if(get_shared_tee_mem() == NULL) {
	//	printk(KERN_ERR "Failed to get TEE memory\n");
	//	return (-1);
	//}

	//TODO: get mutex
	//push_tee_to_com(userspace);

	// CALL Monitor with TEE mem
	result = tee_process(userspace);

	//push_tee_to_userspace(userspace);
	//TODO: release mutex

	return (result);
}

long tz_driver_ioctl(struct file * file, unsigned int cmd, unsigned long arg) {

	int verify_arg = 0;
	void* tz_vaddr = NULL;
	uint32_t tz_value = 0;
	unsigned long tz_paddr = 0xb0000000;

	// Entry Point for Userspace ...
	if (_IOC_TYPE(cmd) != ANDIXTZ_IOC_MAGIC)
		return (-ENOTTY);
	if (_IOC_NR(cmd) > ANDIX_IOC_MAXNR)
		return (-ENOTTY);

	//printk(KERN_INFO "TZ IOCTL CALL\n");

	switch (cmd) {
	// Use copy_(to/from)_user to process TEE and CTRL Requests
	// Process TEE Request
	case ANDIX_CTRL_POLL:
		//printk(KERN_INFO "TZ CTRL POLL\n");
		if (!check_process_is_root()) {
			// Only root process can poll ctrl
			return (-EACCES);
		}

		verify_arg = access_ok(VERIFY_WRITE, arg, sizeof(TZ_CTLR_SPACE));
		if (verify_arg == 0) {
			return (-EINVAL);
		}

		return (poll_ctrl_task_to_ns((TZ_CTLR_SPACE*) arg));
		break;
	case ANDIX_CTRL_PUSH:

		//printk(KERN_INFO "TZ CTRL PUSH\n");

		if (!check_process_is_root()) {
			// Only root process can push ctrl
			return (-EACCES);
		}

		verify_arg = access_ok(VERIFY_READ, arg, sizeof(TZ_CTLR_SPACE));

		if (verify_arg == 0) {
			return (-EINVAL);
		}

		return (push_ctrl_task_from_ns((TZ_CTLR_SPACE*) arg));
		break;
	case ANDIX_TEE_PUSH:

		//printk(KERN_INFO "TZ TEE PUSH\n");

		verify_arg = access_ok(VERIFY_WRITE, arg, sizeof(TZ_TEE_SPACE));

		if (verify_arg == 0) {
			printk(KERN_ERR "TZ_TEE_SPACE not valid "
					"memory for writing! (0x%x)\n", (unsigned int)arg);
			return (-EINVAL);
		}

		return (tz_process_tee_mem((TZ_TEE_SPACE*) arg));
		break;
	case ANDIX_TZ_TEST:
		tz_vaddr = ioremap(tz_paddr, 4);

		if(tz_vaddr != NULL) {

			tz_value = ioread32(tz_vaddr);

			printk(KERN_INFO "TZ MEM VALUE: 0x%x\n", tz_value);

			iounmap(tz_vaddr);
		} else {
			printk(KERN_ERR "TZ mapping failed\n");
		}
		break;
	default: /* redundant, as cmd was checked against MAXNR */
		return (-ENOTTY);
	}

	return (0);
}

/*int tz_driver_mmap(struct file *file, struct vm_area_struct *vma) {
	return (mmap_comm_mem(file, vma, tee_mem));
}*/
