/*
 * module.c
 *
 *  Created on: Nov 29, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_application_mod/andixtee.h>
#include <linux/gfp.h>
#include <linux/init.h>

uint32_t tz_device;
uint8_t initialized;
TZ_TEE_SPACE* tee_mem;

struct file_operations tz_fops = {
		.owner 			= THIS_MODULE,
		.unlocked_ioctl = tz_driver_ioctl,
		.open 			= tz_driver_open,
		.release 		= tz_driver_release,
		.mmap			= tz_driver_mmap,
};

struct cdev cdev;
int log_alloc;

int _init_tz_driver(void) {
	int res = 0;
	int pgcount = 0;
	int pgcount_alloc = 0;
	initialized = 0;
	res = alloc_chrdev_region(&tz_device, 0, 1, DEVICE_NAME);
	if(res != SUCCESS) {
		printk(KERN_ERR "Failed to allocate TrustZone device <%s>\n",
				DEVICE_NAME);
		return (res);
	}
	initialized = 1;

	printk(KERN_INFO "Allocated TrustZone device for andix OS (<%d>:<%d>)\n",
			MAJOR(tz_device), MINOR(tz_device));

	cdev_init(&cdev, &tz_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &tz_fops;

	res = cdev_add(&cdev, tz_device, 1);
	if(res != SUCCESS) {
		printk(KERN_ERR "Failed to add character device <%s> %d\n",
				DEVICE_NAME, tz_device);
		return (res);
	}
	initialized = 2;

	pgcount = sizeof(TZ_MAIN_COM) / PAGE_SIZE;
	log_alloc = 0;
	pgcount_alloc = ipow(2, log_alloc);  // 2 ^ 0

	while(pgcount_alloc < pgcount) {
		log_alloc++;
		pgcount_alloc = ipow(2, log_alloc);
	}

	//com_mem = kmalloc(sizeof(TZ_MAIN_COM), GFP_KERNEL);

	com_mem = (TZ_MAIN_COM*)__get_free_pages(GFP_KERNEL, log_alloc);

	if(com_mem == NULL) {
		printk(KERN_ERR "Failed to allocate com memory\n");
		return (1);
	}

	tee_mem->op = 0xDEADBEEF;

	printk(KERN_INFO "COM memory @ 0x%x p (0x%x)\n",
			(unsigned int)com_mem, virt_to_phys(com_mem));

	if(register_tee_mem_in_tz(com_mem) != 0) {
		free_pages((unsigned long)com_mem, (unsigned int)log_alloc);
		printk(KERN_ERR "Failed to register com memory\n");
	}

	mutex_init(&ctlr_mutex);

	return (SUCCESS);
}

void _exit_tz_driver(void) {
	printk(KERN_INFO "Exiting TrustZone driver for andix OS\n");

	if(com_mem != NULL) {
		printk(KERN_INFO "Freeing com memory TrustZone character device for andix OS (<%d>:<%d>)\n",
				MAJOR(tz_device), MINOR(tz_device));
		unregister_tee_mem_from_tz();
		free_pages((unsigned long)com_mem, (unsigned int)log_alloc);
	}

	if(initialized > 1) {
		printk(KERN_INFO "Removing TrustZone character device for andix OS (<%d>:<%d>)\n",
				MAJOR(tz_device), MINOR(tz_device));
		cdev_del(&cdev);
	}

	if(initialized > 0) {
		printk(KERN_INFO "Unregistering TrustZone device for andix OS (<%d>:<%d>)\n",
					MAJOR(tz_device), MINOR(tz_device));
		unregister_chrdev_region(tz_device, 1);
	}
}

/**
 * Module configurations
 */
module_init( _init_tz_driver);
module_exit( _exit_tz_driver);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
