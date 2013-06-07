/*
 * imx_csu.c
 *
 *  Created on: Jun 2, 2013
 *      Author: andy
 */

#include <drivers/csu/imx_csu.h>

int imx_csu_probe(platform_device_t *dev) {
	hal_map_io_mem(dev);

	dev->device_data = dev->vbase;

	uintptr_t csu = dev->device_data;

	// We need to unlock everything. Do more reverse engineering
	// to find precise register counts

	while (csu < (uint32_t*) ((uint32_t) dev->vbase + 0x80)) {
		(*csu) = 0x00FF00FF;
		csu++;
	}

	return HAL_SUCCESS;
}

uint32_t imx_csu_ioctl(platform_device_t *dev,
		uint32_t request, uintptr_t param, uint32_t psize) {
	// we don't know which device is which so we support nothing...
	return HAL_E_IOCTL_REQ_NOT_AVAIL;
}

static const platform_driver_t imx_csu_driver = { .probe = imx_csu_probe,
		.release = NULL, .write = NULL,
		.read = NULL, .ioctl = NULL, .driver = { .name =
				IMX53_CSU_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_csu_driver)
