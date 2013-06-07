/*
 * csu.c
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#include <devices/csu/csu.h>
#include <kprintf.h>

platform_device_t* csu_dev;

/**
 * initialize csu device
 */
void csu_init() {
	csu_dev = NULL;
	csu_dev = hal_find_device(CSU_DEVICE, 0);
}

/**
 * set access permission for device id
 *
 * @param devID The device id
 * @param ap The access permission (see CSU_AP_*)
 *
 * @return 0 on success other on error
 */
int32_t csu_set_device_permission(int32_t devID, int32_t ap) {
	csu_ioctl_param_t param;
	uint32_t hal_res = 0;

	if (csu_dev == NULL ) {
		hal_error("CSU not available for this platform!");
		return (-1);
	}

	if (ap < __CSU_AP_MIN || ap > __CSU_AP_MAX) {
		hal_error("Invalid Access Permission for CSU!");
		return (-1);
	}

	param.access_permission = ap;
	param.device_id = devID;

	hal_res = hal_ioctl(csu_dev, CSU_IOCTL_SET_AP_DEVICE, (uintptr_t)&param,
			sizeof(csu_ioctl_param_t));

	if(hal_res != HAL_SUCCESS) {
		hal_error("Failed to set access permission for device: %s (%d)",
				hal_get_error_string(hal_res), hal_res);
		return (-1);
	}

	return (0);
}
