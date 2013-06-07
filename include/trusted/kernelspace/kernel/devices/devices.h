/*
 * devices.h
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include <common/typedefs.h>
#include <drivers/drivers.h>

/**
 * \addtogroup dev
 *	\{
 */

/**
 * Maximum device name size
 */
#define DEVICENAMESIZE 50

/**
 * Device information structure
 */
struct device_info {
	const char		name[DEVICENAMESIZE];
	const char		driver[DRIVERNAMESIZE];
	uintptr_t		base;
	uint32_t		size;
	uint32_t		id;
	uint8_t			flags;
	uintptr_t		data;
	uint32_t		datasize;
};

/**
 * device information type
 */
typedef struct device_info device_info_t;

/**
 * Platform device
 */
struct platform_device {
	const device_info_t*		info;
	uint32_t					id;
	uintptr_t					vbase;
	uint32_t					size;
	const platform_driver_t*	driver;
	uintptr_t					device_data;
};

/**
 * Device flag do initialization
 */
#define FLAG_DO_INIT		(1 << 0)

/**
 * Device flag do initialization position
 */
#define FLAG_DO_INIT_POS	(0)

/**
 * Serial device name
 */
#define SERIAL_DEVICE				"serial"

/**
 * Timer device name
 */
#define TIMER_DEVICE				"timer"

/**
 * Ethernet device name
 */
#define ETHERNET_DEVICE				"eth"

/**
 * Random device name
 */
#define RANDOM_DEVICE				"random"

/**
 * Interrupt control device
 */
#define INTERRUPT_CTRL_DEVICE		"interrupt_ctrl"

/**
 * CSU device name
 */
#define CSU_DEVICE					"csu"

/**
 * Watchdog device name
 */
#define WATCHDOG_DEVICE				"wdog"

/**
 * TrustZone Memory Controller
 */
#define TZMEM_CTRL_DEVICE			"tzmem_ctrl"

/** \} */


#endif /* DEVICES_H_ */
