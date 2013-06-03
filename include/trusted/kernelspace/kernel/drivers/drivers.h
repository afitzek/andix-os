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
