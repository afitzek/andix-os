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

/**
 * TZIC Driver name
 */
#define IMX53_TZIC_DRIVER_NAME "IMX53_TZIC"

/**
 * TIZC register map
 */
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

/**
 * Disable TZIC
 *
 * Don't use this unless you know exactly what you are doing!
 */
void imx_tzic_disable(imx_tzic_t* tzic);

/**
 * Enable TZIC
 *
 * Don't use this unless you know exactly what you are doing!
 */
void imx_tzic_enable(imx_tzic_t* tzic);

/**
 * Probe tzic device
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param dev the platform_device
 */
int imx_tzic_probe(platform_device_t *dev);

/**
 * imx53 probe irq secure
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error, 0 irq insecure, 1 irq is secure
 */
int imx_tzic_probe_secure(imx_tzic_t* tzic, int irq);

/**
 * imx53 probe irq enabled
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error, 0 irq disabled, 1 irq is enabled
 */
int imx_tzic_probe_enable(imx_tzic_t* tzic, int irq);

/**
 * imx53 set secure irq
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error
 */
int imx_tzic_set_secure(imx_tzic_t* tzic, int irq);

/**
 * imx53 unset secure irq
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error
 */
int imx_tzic_unset_secure(imx_tzic_t* tzic, int irq);

/**
 * imx53 set enable for irq
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error
 */
int imx_tzic_set_enable(imx_tzic_t* tzic, int irq);

/**
 * imx53 unset enable for irq
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error
 */
int imx_tzic_unset_enable(imx_tzic_t* tzic, int irq);


/**
 * imx53 clear pending irq
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @param irq the irq number
 * @return -1 on error
 */
int imx_tzic_clear_irq(imx_tzic_t* tzic, int irq);

/**
 * imx53 get pending requests
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @return irq, -1 none pending
 */
int imx_tzic_get_pending(imx_tzic_t* tzic);

/**
 * imx53 process all pending requests
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param tzic the tzic device data
 * @return 0 on success
 */
int imx_tzic_do_pending(imx_tzic_t* tzic);

/**
 * imx53 software interrupt generation
 *
 * Don't use this unless you know exactly what you are doing!
 *
 * @param irq the irq parameter
 * @param tzic the tzic device data
 */
void imx_tzic_swint(int irq, imx_tzic_t* tzic);

/**
 * IMX TZIC driver instance
 */
static const platform_driver_t imx_tzic_driver;

#endif /* IMX_TZIC_H_ */
