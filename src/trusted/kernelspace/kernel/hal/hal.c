/*
 * hal.c
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <hal.h>
#include <kprintf.h>
#include <mm/mm.h>

/**
 * \defgroup hal HAL - Hardware Abstraction Layer
 *
 * Stuff for Hardware access
 */

/* \addtogroup hal
 * \{ */

list* hal_devices = NULL;/*{ .data = (uintptr_t) NULL,
 .next = &hal_devices, .prev = &hal_devices };*/

extern platform_driver_t * __hal_driver_table[];
extern platform_driver_t * __hal_driver_table_end;

extern hal_platform_t * __hal_platform_table[];
extern hal_platform_t * __hal_platform_table_end;

hal_platform_t * current_platform;

uint32_t hal_init_magic = 0;

void hal_init(uint32_t sysid, atag_t* atags) {
	hal_debug("HAL initializing ... from 0x%x - 0x%x", &__hal_platform_table[0],
			&__hal_platform_table_end);
	hal_debug("for System: 0x%x", sysid);
	current_platform = NULL;
	int idx = 0;
	int devidx = 0;

	hal_platform_t* platform = NULL;
	while (&__hal_platform_table[idx] < &__hal_platform_table_end) {
		hal_platform_t* check = __hal_platform_table[idx];
		hal_debug("checking platform @ 0x%x [%d]", check, check->sys_id);
		if (check->sys_id == sysid) {
			hal_debug("Platform @ 0x%x", check);
			platform = check;
		}
		idx++;
	}
	if (platform == 0) {
		hal_debug("HAL initializing [failed]");
		return;
	}

	current_platform = platform;

	hal_devices = (list*) kmalloc(sizeof(list));
	hal_devices->next = hal_devices;
	hal_devices->prev = hal_devices;
	hal_devices->data = NULL;

	for (devidx = 0; devidx < platform->device_count; devidx++) {
		const device_info_t* dev_info = &(platform->dev_map[devidx]);
		platform_driver_t* driver = hal_find_driver(dev_info->driver);
		/*hal_debug(
		 "Device %s.%d [%s:0x%x] @ 0x%x", dev_info->name, dev_info->id, dev_info->driver, driver, dev_info->base);*/
		if (driver == NULL ) {
			hal_debug("Driver %s not found ...", dev_info->driver);
		} else {
			platform_device_t* device = (platform_device_t*) kmalloc(
					sizeof(platform_device_t));
			device->driver = driver;
			device->id = dev_info->id;
			device->info = dev_info;
			device->size = dev_info->size;

			if (device->driver->probe) {
				if (device->driver->probe(device) != HAL_SUCCESS) {
					kfree((void*) device);
					hal_debug("Failed to add hal device!");
				} else {
					hal_add_device(device);
					hal_debug("Device %s.%d [%s:0x%x] @ 0x%x ready",
							dev_info->name, dev_info->id, dev_info->driver,
							driver, device);
				}
			} else {
				kfree((void*) device);
				hal_debug("Failed to add hal device!");
			}
		}
	}

	hal_init_magic = 0x96456281;

	timing_init();
}

hal_platform_t* hal_get_platform() {
	return (current_platform);
}

int32_t hal_add_device(platform_device_t* dev) {
	if (!dev) {
		return HAL_E_INVALID_DEV;
	}

	list_add(hal_devices, (uintptr_t) dev);
	return HAL_SUCCESS;
}

void hal_rem_device(platform_device_t *dev) {
	list* entry = list_find_data(hal_devices, (uintptr_t) dev);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

void hal_map_io_mem(platform_device_t *dev) {

	dev->vbase = (uintptr_t) map_io_mem(dev->info->base, dev->size);
	//hal_debug(
	//		"Mapped [%d frames] IO 0x%x .. 0x%x -> 0x%x .. 0x%x", frames, section_description.paddr, section_description.paddr + dev->size, section_description.vaddr, section_description.vaddr + dev->size);
}

void hal_unmap_io_mem(platform_device_t* dev) {
	//TODO
}

platform_device_t* hal_find_device(const char* name, uint32_t id) {
	list* pos;
	list* next;
	platform_device_t* device = NULL;
	platform_device_t* device_res = NULL;

	if (hal_init_magic != 0x96456281) {
		return (NULL );
	}

	if (hal_devices == NULL ) {
		return (NULL );
	}
	hal_debug("Searching @ %s %d", name, id);
	list_for_each_safe(pos, next, hal_devices)
	{
		if (pos->data != NULL ) {
			device = (platform_device_t*) pos->data;
			if (strncmp(device->info->name, name, strlen(name)) == 0
					&& device->id == id) {
				device_res = device;
				break;
			}
		}
	}

	return (device_res);
}

uint32_t hal_write(platform_device_t* dev, void* data, uint32_t size) {
	if (dev == NULL || dev->driver == NULL ) {
		return HAL_E_INVALID_DEV;
	}

	if (dev->driver->write) {
		return (dev->driver->write(dev, data, size));
	}
	return HAL_E_FUNC_NOT_AVAIL;
}

uint32_t hal_read(platform_device_t* dev, void* data, uint32_t nsize) {
	if (dev == NULL || dev->driver == NULL ) {
		return HAL_E_INVALID_DEV;
	}

	if (dev->driver->read) {
		return (dev->driver->read(dev, data, nsize));
	}
	return HAL_E_FUNC_NOT_AVAIL;
}

uint32_t hal_probe(platform_device_t* dev) {
	if (dev == NULL || dev->driver == NULL ) {
		return HAL_E_INVALID_DEV;
	}

	if (dev->driver->probe) {
		return (dev->driver->probe(dev));
	}
	return HAL_E_FUNC_NOT_AVAIL;
}

void hal_release(platform_device_t* dev) {
	if (dev == NULL || dev->driver == NULL ) {
		return;
	}

	if (dev->driver->release) {
		dev->driver->release(dev);
	}
	return;
}

uint32_t hal_ioctl(platform_device_t* dev, uint32_t request, uintptr_t param,
		uint32_t psize) {
	if (dev == NULL || dev->driver == NULL ) {
		return HAL_E_INVALID_DEV;
	}

	if (dev->driver->ioctl) {
		return (dev->driver->ioctl(dev, request, param, psize));
	}
	return HAL_E_FUNC_NOT_AVAIL;
}

const char* hal_get_error_string(uint32_t error) {
	switch (error) {
	case HAL_SUCCESS:
		return ("HAL_SUCCESS");
	case HAL_E_DEV_ERROR:
		return ("HAL_E_DEV_ERROR");
	case HAL_E_FUNC_NOT_AVAIL:
		return ("HAL_E_FUNC_NOT_AVAIL");
	case HAL_E_INVALID_DEV:
		return ("HAL_E_INVALID_DEV");
	case HAL_E_IOCTL_PARA_INVALID:
		return ("HAL_E_IOCTL_PARA_INVALID");
	case HAL_E_IOCTL_REQ_NOT_AVAIL:
		return ("HAL_E_IOCTL_REQ_NOT_AVAIL");
	case HAL_E_OUT_OF_MEMORY:
		return ("HAL_E_OUT_OF_MEMORY");
	case HAL_E_OUT_OF_RESOURCES:
		return ("HAL_E_OUT_OF_RESOURCES");
	default:
		return ("UNKNOWN");
	}
}

void hal_set_device_id(platform_device_t* dev) {
	uint32_t max = 0;
	list* pos;
	list* next;
	platform_device_t* device;
	list_for_each_safe(pos, next, hal_devices)
	{
		if (pos->data != NULL ) {
			device = (platform_device_t*) pos->data;
			if (device->driver == dev->driver) {
				if (device->id > max) {
					max = device->id;
				}
			}
		}
	}

	max++;
	dev->id = max;
}

platform_driver_t* hal_find_driver(const char* name) {
	//platform_driver_t** ptr = (platform_driver_t**)__hal_driver_table;
	int idx = 0;
	while (&__hal_driver_table[idx] < &__hal_driver_table_end) {
		platform_driver_t* driver = __hal_driver_table[idx];
		if (strncmp(driver->driver.name, name, strlen(name)) == 0) {
			return (driver);
		}
		idx++;
	}
	return (NULL );
}

/* \} group */
