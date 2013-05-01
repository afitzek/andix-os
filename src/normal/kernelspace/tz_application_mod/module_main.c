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
#include <tz_application_mod/tee_logic.h>

uint32_t tz_device;
uint8_t initialized;
TZ_TEE_SPACE* tee_mem;

struct file_operations tz_fops = { .owner = THIS_MODULE, .unlocked_ioctl =
		tz_driver_ioctl, .open = tz_driver_open, .release = tz_driver_release, };

struct cdev cdev;

int _init_tz_driver(void) {
	int res = 0;
	initialized = 0;
	res = alloc_chrdev_region(&tz_device, 0, 1, DEVICE_NAME);
	if (res != SUCCESS) {
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
	if (res != SUCCESS) {
		printk(KERN_ERR "Failed to add character device <%s> %d\n", DEVICE_NAME,
				tz_device);
		return (res);
	}
	initialized = 2;

	if (initialize_ctrl() != 0) {
		printk(KERN_ERR "Failed to initialize ctrl module\n");
		return (-1);
	}

	if (initialize_tee() != 0) {
		printk(KERN_ERR "Failed to initialize tee module\n");
		return (-1);
	}

	if (tee_init() != 0) {
		printk(KERN_ERR "Failed to initialize tee logic\n");
		return (-1);
	}

	return (SUCCESS);
}

void _exit_tz_driver(void) {
	printk(KERN_INFO "Exiting TrustZone driver for andix OS\n");

	cleanup_tee();
	cleanup_ctrl();

	if (initialized > 1) {
		printk(
				KERN_INFO "Removing TrustZone character device for andix OS (<%d>:<%d>)\n",
				MAJOR(tz_device), MINOR(tz_device));
		cdev_del(&cdev);
	}

	if (initialized > 0) {
		printk(
				KERN_INFO "Unregistering TrustZone device for andix OS (<%d>:<%d>)\n",
				MAJOR(tz_device), MINOR(tz_device));
		unregister_chrdev_region(tz_device, 1);
	}
}

/**
 * Module configurations
 */
module_init(_init_tz_driver)
;
module_exit(_exit_tz_driver)
;

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
