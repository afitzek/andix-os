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

#define IMX53_CSU_DRIVER_NAME "IMX53_CSU"

int imx_csu_probe(platform_device_t *dev);

static const platform_driver_t imx_csu_driver;

#endif /* IMX_CSU_H_ */
