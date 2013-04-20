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
