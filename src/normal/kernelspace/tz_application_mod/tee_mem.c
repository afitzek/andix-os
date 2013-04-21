/*
 * tee_mem.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_mem.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/cred.h>
#include <linux/uaccess.h>
#include <tz_application_mod/andixtee.h>

TZ_TEE_SPACE* shared_tee_mem = NULL;
int tee_init = 0;
TZ_TEE_SPACE teespace;
struct mutex tee_mutex;

int push_tee_to_userspace(TZ_TEE_SPACE *userspace) {
	int result = 0;

	// check if ctrl system is initialized
	if (tee_init == 1) {
		mutex_lock(&tee_mutex);
		// check if ctrl system is initialized
		if (tee_init == 1) {
			result = 1;
			if(copy_to_user(userspace, shared_tee_mem, sizeof(TZ_TEE_SPACE))) {
				// copy failed!
				printk(KERN_INFO "Failed to copy tee request to userspace\n");
				return (-1);
			}
		}
		mutex_unlock(&tee_mutex);
	}

	return (result);
}

int push_tee_to_com(TZ_TEE_SPACE *userspace) {
	int result = 0;

	// check if tee system is initialized
	if (tee_init == 1) {
		mutex_lock(&tee_mutex);
		// check if tee system is initialized
		if (tee_init == 1) {
			result = 1;
			if(copy_from_user(shared_tee_mem,
					userspace, sizeof(TZ_TEE_SPACE))) {
				// copy failed!
				printk(KERN_INFO "Failed to copy tee request to kernel\n");
				return (-1);
			}
		}
		mutex_unlock(&tee_mutex);
	}

	return (result);
}

int register_tee_mem_in_tz(TZ_TEE_SPACE *com) {
	uint32_t paddr = virt_to_phys((void*) com);
	uint32_t res = 0;
	printk(KERN_INFO "Registering tee memory virtual 0x%x physical 0x%x",
			(unsigned int) com, paddr);
	res = __smc_1(SMC_REGISTER_TMEM, paddr);
	return ((int) res);
}

int unregister_tee_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering tee memory");
	res = __smc_1(SMC_UNREGISTER_TMEM, 0);
	return ((int) res);
}

void cleanup_tee() {
	if (tee_init == 1 && shared_tee_mem != NULL) {
		mutex_lock(&tee_mutex);
		unregister_tee_mem_from_tz();
		kfree(shared_tee_mem);
		shared_tee_mem = NULL;
		tee_init = 0;
		mutex_unlock(&tee_mutex);
		mutex_destroy(&tee_mutex);
	}
}

int initialize_tee() {
	// Allocate memory
	shared_tee_mem = (TZ_TEE_SPACE*) kmalloc(sizeof(TZ_TEE_SPACE), GFP_KERNEL);

	if (shared_tee_mem == NULL) {
		printk(KERN_ERR "Failed to allocate tee memory\n");
		return (-1);
	}

	printk(KERN_INFO "TEE memory @ 0x%x p (0x%x)\n",
			(unsigned int)shared_tee_mem, virt_to_phys(shared_tee_mem));

	if (register_tee_mem_in_tz(shared_tee_mem) != 0) {
		kfree(shared_tee_mem);
		printk(KERN_ERR "Failed to register tee memory in tz\n");
		return (-1);
	}

	mutex_init(&tee_mutex);

	tee_init = 1;

	return (0);
}
