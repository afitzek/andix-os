/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
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
