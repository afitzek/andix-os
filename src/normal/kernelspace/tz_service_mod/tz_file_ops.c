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

#include <tz_service_mod/andixtz.h>

static atomic_t tz_dev_available = ATOMIC_INIT(1);

int tz_driver_open(struct inode * inode, struct file * file) {
	if (!atomic_dec_and_test(&tz_dev_available)) {
		atomic_inc(&tz_dev_available);
		return (-EBUSY); /* already open */
	}
	return (0);
}

int tz_driver_release(struct inode * inode, struct file * file) {
	atomic_inc(&tz_dev_available); /* release the device */
	return (0);
}

long tz_driver_ioctl(struct file * file, unsigned int cmd, unsigned long arg) {
	// Entry Point for Userspace ...
	if (_IOC_TYPE(cmd) != ANDIXTZ_IOC_MAGIC)
		return (-ENOTTY);
	if (_IOC_NR(cmd) > ANDIX_IOC_MAXNR)
		return (-ENOTTY);

	switch (cmd) {
	case ANDIX_IOCTZ:
		CP15DMB;
		CP15DSB;
		CP15ISB;
		if (ctrl_mem) {
			printk(KERN_INFO "Calling SMC with %08X @ 0x%08X",
					ctrl_mem->op, (unsigned int)ctrl_mem);

			// Pending service call for TrustZone
			__smc_1(SMC_PROCESS_CMEM, 0);
		}
		break;
	case ANDIX_IOGMOFF:
		return (((uint32_t) ctrl_mem) & 0xFFF);
	default: /* redundant, as cmd was checked against MAXNR */
		return (-ENOTTY);
	}

	return (0);
}

int tz_driver_mmap(struct file *file, struct vm_area_struct *vma) {
	return (mmap_comm_mem(file, vma, ctrl_mem));
}
