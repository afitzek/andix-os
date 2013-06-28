/*
 * imx_csu.h
 *
 *  Created on: Jun 2, 2013
 *      Author: andy
 */

#ifndef IMX_CSU_H_
#define IMX_CSU_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

/**
 * \addtogroup imx53_csu
 *	\{
 */

/**
 * IMX53 CSU Driver name
 */
#define IMX53_CSU_DRIVER_NAME "IMX53_CSU"

/**
 * imx53 csu probe handler
 * @param dev the device instance
 * @return HAL_*
 */
int imx_csu_probe(platform_device_t *dev);

/**
 * imx53 csu ioctl handler
 * @param dev the device instance
 * @param request the request to process
 * @param param the request parameter
 * @param psize the size of the request parameter
 * @return HAL_*
 */
uint32_t imx_csu_ioctl(platform_device_t *dev,
		uint32_t request, uintptr_t param, uint32_t psize);

/**
 * The imx53 CSU driver
 */
static const platform_driver_t imx_csu_driver;

/** \} */

#endif /* IMX_CSU_H_ */
