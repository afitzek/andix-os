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
 * drivers.h
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

#include <common/typedefs.h>
#include <ioctl.h>

#define DRIVERNAMESIZE	50

#define HAL_SUCCESS					(0)
#define HAL_E_INVALID_DEV			(-1)
#define HAL_E_FUNC_NOT_AVAIL		(-2)
#define HAL_E_IOCTL_REQ_NOT_AVAIL	(-3)
#define HAL_E_IOCTL_PARA_INVALID	(-4)
#define HAL_E_OUT_OF_MEMORY			(-5)
#define HAL_E_DEV_ERROR				(-6)
#define HAL_E_OUT_OF_RESOURCES		(-7)

#define __REG(x)     (*((volatile uint32_t *)(x)))

struct platform_device;

typedef struct platform_device platform_device_t;

typedef struct {
	char		name[DRIVERNAMESIZE];
	uintptr_t	driver_data;
} device_driver_t;

typedef struct {
	int (*probe)(platform_device_t *);
	void (*release)(platform_device_t *);
	uint32_t (*write)(platform_device_t *, void* data, uint32_t size);
	uint32_t (*read)(platform_device_t *, void* data, uint32_t nsize);
	uint32_t (*ioctl)(platform_device_t *, uint32_t request, uintptr_t param, uint32_t psize);
	device_driver_t driver;
	uint32_t idcount;
} platform_driver_t;

#define HAL_DRIVER_REGISTER(_name)                            \
  __attribute__((section(".hal.platform.driver")))                 \
  const platform_driver_t *const __platform_driver_##_name = &_name;

#endif /* DRIVERS_H_ */
