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
 * Raw read byte from address
 * @param addr the memory address to read from
 * @return byte read from address
 */
static inline uint8_t __raw_readb(const volatile void *addr) {
	return (*(volatile uint8_t *) (addr));
}

/**
 * Raw read word from address
 * @param addr the memory address to read from
 * @return word read from address
 */
static inline uint16_t __raw_readw(const volatile void *addr) {
	return (*(volatile uint16_t *) (addr));
}

/**
 * Raw read 32 bit integer from address
 * @param addr the memory address to read from
 * @return 32 bit integer read from address
 */
static inline uint32_t __raw_readl(const volatile void *addr) {
	return (*(volatile uint32_t *) (addr));
}

/**
 * Raw write byte to address
 * @param v the byte value to write
 * @param addr the memory address to write to
 */
static inline void __raw_writeb(uint8_t v, volatile void *addr) {
	*(volatile uint8_t *) (addr) = v;
}

/**
 * Raw write word to address
 * @param v the word value to write
 * @param addr the memory address to write to
 */
static inline void __raw_writew(uint16_t v, volatile void *addr) {
	*(volatile uint16_t *) (addr) = v;
}

/**
 * Raw write 32 bit integer to address
 * @param v the 32 bit value to write
 * @param addr the memory address to write to
 */
static inline void __raw_writel(uint32_t v, volatile void *addr) {
	*(volatile uint32_t *) (addr) = v;
}

typedef void (*early_io_init)(void);
typedef void (*early_io_putc)(uint8_t c);
typedef uint8_t (*early_io_getc)(void);

/**
 * Platform Structure
 */
struct hal_platform {
	uint32_t sys_id; /**< The system id */
	uint32_t device_count; /**< The count of the dev_map */
	const device_info_t* dev_map; /**< The device map */
	uint32_t platform_device_count;
	const platform_device_t* platform_device_map;
	early_io_init __early_io_init;
	early_io_putc __early_io_putc;
	early_io_getc __early_io_getc;
};

/**
 * Typedef of Platform Structure
 */
typedef struct hal_platform hal_platform_t;

struct board_info {
	uint32_t sys_id;
	uint32_t phys_target;
	uint32_t early_io;
};

typedef struct board_info board_info_t;

/**
 * Initialize HAL system
 * @param sysid The system ID
 * @param atags The ATAGS pointer
 */
void hal_init(uint32_t sysid, atag_t* atags);

/**
 * Add a platform device to the hal pool
 * @param dev the platform device
 * @return HAL_*
 */
int32_t hal_add_device(platform_device_t* dev);

/**
 * Removes a device from the hal pool
 * @param dev the platform device
 */
void hal_rem_device(platform_device_t *dev);

/**
 * Map the physical memory of a device to the kernel
 * @param dev the platform device
 */
void hal_map_io_mem(platform_device_t *dev);

/**
 * Find a platform device
 * @param name The device name
 * @param id The device index by type
 */
platform_device_t* hal_find_device(const char* name, uint32_t id);

/**
 * Find a hal driver by given name
 * @param name the driver name
 */
platform_driver_t* hal_find_driver(const char* name);

/**
 * Get the configured platform
 * @return the active platform
 */
hal_platform_t* hal_get_platform();

/**
 * Translate a HAL_* error to a readable string
 * @param the error number
 * @return human readable string
 */
const char* hal_get_error_string(uint32_t error);

/**
 * Execute write operation on the device
 *
 * @param dev a platform device
 * @param data the data to be written
 * @param size the size to be written
 * @return HAL_* constants
 */
uint32_t hal_write(platform_device_t* dev, void* data, uint32_t size);

/**
 * Execute read operation on the device
 *
 * @param dev a platform device
 * @param data the data buffer to read to
 * @param nsize the number of byte to read
 * @return HAL_* constants
 */
uint32_t hal_read(platform_device_t* dev, void* data, uint32_t nsize);

/**
 * Execute ioctl operation on the device
 *
 * @param dev a platform device
 * @param request the request
 * @param param the request parameter
 * @param psize the size of the request parameter
 * @return HAL_* constants
 */
uint32_t hal_ioctl(platform_device_t* dev, uint32_t request, uintptr_t param,
		uint32_t psize);

/**
 * Execute probe operation on the device
 * @param dev a platform device
 * @return HAL_* constants
 */
uint32_t hal_probe(platform_device_t* dev);

/**
 * Register a HAL platform
 */
#define HAL_PLATFORM_REGISTER(_name)                            \
  __attribute__((section(".hal.platform.ptr")))                 \
  const hal_platform_t *const __hal_platform_##_name = &_name;


/* \} group */

#endif /* HAL_H_ */
