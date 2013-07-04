/*
 * tzmem_controller.c
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#include <common.h>
#include <devices/devices.h>
#include <devices/tzmem_controller/tzmem_controller.h>
#include <kprintf.h>

platform_device_t* tzmem_dev;

int32_t tzmem_init() {
	tzmem_dev = NULL;
	tzmem_dev = hal_find_device(TZMEM_CTRL_DEVICE, 0);

	if (tzmem_dev != NULL ) {
		hal_error("TZMEM Controller ready!");
		return (HAL_SUCCESS);
	}
	hal_error("NO TZMEM Controller available!");
	return (HAL_E_INVALID_DEV);
}

int32_t tzmem_lock(uintptr_t paddr, uint32_t size) {
	tz_mem_ioparam_t param;
	if (tzmem_dev == NULL ) {
		hal_error("NO TZMEM Controller available!");
		return (HAL_E_INVALID_DEV);
	}

	param.physical_addr = paddr;
	param.size = size;

	return (hal_ioctl(tzmem_dev, TZ_MEM_IOCTL_LOCK_MEMORY, &param,
			sizeof(tz_mem_ioparam_t)));
}

int32_t tzmem_unlock(uintptr_t* paddr, uint32_t size) {
	tz_mem_ioparam_t param;
	if (tzmem_dev == NULL ) {
		hal_error("NO TZMEM Controller available!");
		return (HAL_E_INVALID_DEV);
	}

	param.physical_addr = paddr;
	param.size = size;

	return (hal_ioctl(tzmem_dev, TZ_MEM_IOCTL_UNLOCK_MEMORY, &param,
			sizeof(tz_mem_ioparam_t)));
}

void tzmem_dump() {
	tz_mem_ioparam_t param;
	if (tzmem_dev == NULL ) {
		hal_error("NO TZMEM Controller available!");
		return;
	}

	param.physical_addr = 0;
	param.size = 0;

	hal_ioctl(tzmem_dev, TZ_MEM_IOCTL_DUMP, &param, sizeof(tz_mem_ioparam_t));
}
