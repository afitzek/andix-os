/*
 * imx_tzic.h
 *
 *  Created on: May 27, 2013
 *      Author: andy
 */

#ifndef IMX_TZIC_H_
#define IMX_TZIC_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

#define IMX53_TZIC_DRIVER_NAME "IMX53_TZIC"

struct imx_tzic_regs {
	uint32_t	intctrl;    // 000
	uint32_t	inttype;    // 004
	uint32_t	__space_0;
	uint32_t	priomask;   // 00C
	uint32_t	syncctrl;   // 010
	uint32_t	dsmint;     // 014
	uint32_t	__space_1[26];
	uint32_t	intsec[4];  // 080 - 08C
	uint32_t	__space_2[28];
	uint32_t	enset[4];   // 100 - 110
	uint32_t	__space_3[28];
	uint32_t	enclear[4]; // 180 - 190
	uint32_t	__space_4[28];
	uint32_t	srcset[4];  // 200 - 210
	uint32_t	__space_5[28];
	uint32_t	srcclear[4]; // 280 - 290
	uint32_t	__space_6[92];
	uint32_t	priority[32]; // 400 - 480
	uint32_t	__space_7[544];
	uint32_t	pending[4];   // D00 - D10
	uint32_t	__space_8[28];
	uint32_t	hipending[4]; // D80 - D90
	uint32_t	__space_9[28];
	uint32_t	wakeup_config[4]; // E00 - E10
	uint32_t	__space_10[60];
	uint32_t	swint;
};

typedef struct imx_tzic_regs imx_tzic_t;

/**
 * IOCTL handler for imx53 TZIC
 */
uint32_t imx_tzic_ioctl(platform_device_t *dev,
		uint32_t request, uintptr_t param, uint32_t psize);

static const platform_driver_t imx_tzic_driver;

#endif /* IMX_TZIC_H_ */
