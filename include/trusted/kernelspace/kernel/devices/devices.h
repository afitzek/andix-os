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

#define DEVICENAMESIZE 50

struct device_info {
	const char		name[DEVICENAMESIZE];
	const char		driver[DRIVERNAMESIZE];
	uintptr_t		base;
	uint32_t		size;
	uint32_t		id;
	uint8_t			flags;
	//uintptr_t		data;
};

typedef struct device_info device_info_t;

struct platform_device {
	const device_info_t*		info;
	uint32_t					id;
	uintptr_t					vbase;
	uint32_t					size;
	const platform_driver_t*	driver;
	uintptr_t					device_data;
};

#define FLAG_DO_INIT		(1 << 0)
#define FLAG_DO_INIT_POS	(0)

#define SERIAL_DEVICE		"serial"
#define TIMER_DEVICE		"timer"
#define ETHERNET_DEVICE		"eth"
#define RANDOM_DEVICE		"random"

#endif /* DEVICES_H_ */
