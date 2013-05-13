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
#include <linux/kernel.h>
#include <linux/cred.h>
#include <linux/uaccess.h>
#include <tz_application_mod/andixtee.h>
#include <asm-generic/cacheflush.h>

struct page * shared_pages = NULL;
TZ_TEE_SPACE* shared_tee_mem = NULL;
int tee_init_state = 0;
TZ_TEE_SPACE teespace;
struct mutex tee_mutex;

int push_tee_to_userspace(TZ_TEE_SPACE *userspace) {
	int result = 0;

	// check if ctrl system is initialized
	if (tee_init_state == 1) {
		mutex_lock(&tee_mutex);
		// check if ctrl system is initialized
		if (tee_init_state == 1) {
			result = 1;
			if (copy_to_user(userspace, shared_tee_mem, sizeof(TZ_TEE_SPACE))) {
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
	if (tee_init_state == 1) {
		mutex_lock(&tee_mutex);
		// check if tee system is initialized
		if (tee_init_state == 1) {
			result = 1;
			if (copy_from_user(shared_tee_mem, userspace,
					sizeof(TZ_TEE_SPACE))) {
				// copy failed!
				printk(KERN_INFO "Failed to copy tee request to kernel\n");
				return (-1);
			}
		}
		mutex_unlock(&tee_mutex);
	}

	return (result);
}

int push_tee(TZ_TEE_SPACE* kspace) {
	int result = 0;

	// check if tee system is initialized
	if (tee_init_state == 1) {
		mutex_lock(&tee_mutex);
		// check if tee system is initialized
		if (tee_init_state == 1) {
			result = 1;
			memcpy(shared_tee_mem, kspace, sizeof(TZ_TEE_SPACE));
		}
		mutex_unlock(&tee_mutex);
	}

	return (result);
}

int poll_tee(TZ_TEE_SPACE* kspace) {
	int result = 0;

	// check if tee system is initialized
	if (tee_init_state == 1) {
		mutex_lock(&tee_mutex);
		// check if tee system is initialized
		if (tee_init_state == 1) {
			result = 1;
			memcpy(kspace, shared_tee_mem, sizeof(TZ_TEE_SPACE));
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

TZ_TEE_SPACE *get_shared_tee_mem() {
	if (tee_init_state == 1 && shared_tee_mem != NULL ) {
		return (shared_tee_mem);
	} else {
		return (NULL );
	}
}

int unregister_tee_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering tee memory");
	res = __smc_1(SMC_UNREGISTER_TMEM, 0);
	return ((int) res);
}

void cleanup_tee() {
	int pages = needed_log_pages(sizeof(TZ_TEE_SPACE));
	if (tee_init_state == 1 && shared_tee_mem != NULL ) {
		mutex_lock(&tee_mutex);
		unregister_tee_mem_from_tz();
		//kfree(shared_tee_mem);
		__free_pages(shared_pages, pages);
		shared_tee_mem = NULL;
		shared_pages = NULL;
		tee_init_state = 0;
		mutex_unlock(&tee_mutex);
		mutex_destroy(&tee_mutex);
	}
}

/*
void tee_flush_cache() {
	if (tee_init_state == 1) {
		flush_dcache_page(shared_pages);
	}
}

uint32_t cl_size;

void tee_invalidate(void* memory, uint32_t size) {
	uint32_t start = (uint32_t) memory;
	uint32_t end = start + size;
	start = round_down(start, cl_size);
	end = round_up(end, cl_size);
	while (start < end) {
		asm ("mcr p15, 0, %0, c7, c6, 1" : : "r" (start)
				: "memory");
		start += cl_size;
	}
	CP15DSB;
	CP15ISB;
}
*/

int initialize_tee() {
	// Allocate memory
	//shared_tee_mem = (TZ_TEE_SPACE*) kmalloc(sizeof(TZ_TEE_SPACE),
	//		GFP_KERNEL);

	//uint32_t ctr;

	//asm volatile("mrc p15, 0, %0, c0, c0, 1" : "=r" (ctr));
	//cl_size = 4 << ((ctr >> 16) & 0xf);

	int pages = needed_log_pages(sizeof(TZ_TEE_SPACE));

	shared_pages = alloc_pages(GFP_KERNEL | GFP_DMA, pages);
	if (shared_pages == NULL ) {
		printk(KERN_ERR "Failed to allocate tee memory\n");
		return (-1);
	}
	shared_tee_mem = (TZ_TEE_SPACE*) page_address(shared_pages);

	printk(KERN_INFO "TEE memory @ 0x%x p (0x%x)\n",
			(unsigned int) shared_tee_mem, virt_to_phys(shared_tee_mem));

	if (register_tee_mem_in_tz(shared_tee_mem) != 0) {
		//kfree(shared_tee_mem);
		__free_pages(shared_pages, pages);
		shared_tee_mem = NULL;
		shared_pages = NULL;
		printk(KERN_ERR "Failed to register tee memory in tz\n");
		return (-1);
	}

	mutex_init(&tee_mutex);

	tee_init_state = 1;

	return (0);
}
