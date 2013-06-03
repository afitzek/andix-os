/*
 * imx_wdog.h
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#ifndef IMX_WDOG_H_
#define IMX_WDOG_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

#define IMX53_WDOG_DRIVER_NAME "IMX53_WDOG"

struct imx_wdog_regs {
	uint16_t	wcr;
	uint16_t	wsr;
	uint16_t	wrsr;
	uint16_t	wicr;
	uint16_t	wmcr;
};

typedef struct imx_wdog_regs imx_wdog_t;

static const platform_driver_t imx_wdog_driver;

#endif /* IMX_WDOG_H_ */
