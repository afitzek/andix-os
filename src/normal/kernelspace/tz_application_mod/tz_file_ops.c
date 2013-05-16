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

	// Entry Point for Userspace ...
	if (_IOC_TYPE(cmd) != ANDIXTZ_IOC_MAGIC)
		return (-ENOTTY);
	if (_IOC_NR(cmd) > ANDIX_IOC_MAXNR)
		return (-ENOTTY);

	//printk(KERN_INFO "TZ IOCTL CALL\n");

	switch (cmd) {
	// TODO remove userspace maping of com memory!
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

		printk(KERN_INFO "TZ CTRL PUSH\n");

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

		printk(KERN_INFO "TZ TEE PUSH\n");

		verify_arg = access_ok(VERIFY_WRITE, arg, sizeof(TZ_TEE_SPACE));

		if (verify_arg == 0) {
			printk(KERN_ERR "TZ_TEE_SPACE not valid "
					"memory for writing! (0x%x)\n", arg);
			return (-EINVAL);
		}

		return (tz_process_tee_mem((TZ_TEE_SPACE*) arg));
		break;
	default: /* redundant, as cmd was checked against MAXNR */
		return (-ENOTTY);
	}

	return (0);
}

/*int tz_driver_mmap(struct file *file, struct vm_area_struct *vma) {
	return (mmap_comm_mem(file, vma, tee_mem));
}*/
