/*
 * hal.h
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef HAL_H_
#define HAL_H_

#include <devices/devices.h>
#include <drivers/drivers.h>
#include <common/list.h>
#include <common/string.h>
#include <common/atags.h>
#include <common.h>
#include <common/typedefs.h>
#include <kprintf.h>
#include <doc/hal_doc.h>

/**
 * \addtogroup hal
 *	\{
 */

/**
 *
 */
static inline uint8_t __raw_readb(const volatile void *addr)
{
	return (*(volatile uint8_t *)(addr));
}
static inline uint16_t __raw_readw(const volatile void *addr)
{
	return (*(volatile uint16_t *)(addr));
}
static inline uint32_t __raw_readl(const volatile void *addr)
{
	return (*(volatile uint32_t *)(addr));
}
static inline void __raw_writeb(uint8_t v, volatile void *addr)
{
	*(volatile uint8_t *)(addr) = v;
}
static inline void __raw_writew(uint16_t v, volatile void *addr)
{
	*(volatile uint16_t *)(addr) = v;
}
static inline void __raw_writel(uint32_t v, volatile void *addr)
{
	*(volatile uint32_t *)(addr) = v;
}

/**
 * Platform Structure
 */
struct hal_platform {
	uint32_t	 				sys_id;		/**< The system id */
	uint32_t					device_count;	/**< The count of the dev_map */
	const device_info_t* 		dev_map;	/**< The device map */
	uint32_t					platform_device_count;
	const platform_device_t* 	platform_device_map;
};

/**
 * Typedef of Platform Structure
 */
typedef struct hal_platform hal_platform_t;

/**
 * Initialize HAL system
 * @param sysid The system ID
 * @param atags The ATAGS pointer
 */
void hal_init(uint32_t sysid, atag_t* atags);

int32_t hal_add_device(platform_device_t* dev);

void hal_rem_device(platform_device_t *dev);

void hal_map_io_mem(platform_device_t *dev);

platform_device_t* hal_find_device(const char* name, uint32_t id);

platform_driver_t* hal_find_driver(const char* name);

hal_platform_t* hal_get_platform();

const char* hal_get_error_string(uint32_t error);

uint32_t hal_write(platform_device_t* dev, void* data, uint32_t size);
uint32_t hal_read(platform_device_t* dev, void* data, uint32_t nsize);
uint32_t hal_ioctl(platform_device_t* dev, uint32_t request, uintptr_t param, uint32_t psize);
uint32_t hal_probe(platform_device_t* dev);

/**
 * Register a HAL platform
 */
#define HAL_PLATFORM_REGISTER(_name)                            \
  __attribute__((section(".hal.platform.ptr")))                 \
  const hal_platform_t *const __hal_platform_##_name = &_name;

/* \} group */

#endif /* HAL_H_ */
