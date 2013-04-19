/*
 * ns_block.h
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef NS_BLOCK_H_
#define NS_BLOCK_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

#define ANDIX_NS_BLOCK_DRIVER_NAME "ANDIX_NS_BLOCK"

// IOCTLS

#define NS_BLK_SETUP		(0x01)
#define NS_BLK_READBLK		(0x02)
#define NS_BLK_WRITEBLK		(0x03)
#define NS_BLK_CREATE		(0x04)

#define BLOCK_SIZE			(1024)

typedef struct {
	uint8_t		blk_data[BLOCK_SIZE];
	uint32_t	offset;
} ns_block_blk;

typedef struct {
	uint8_t		device_name[32];
} ns_block_setup;

typedef struct {
	void* 		nsfd;
} ns_block_dev;

static const platform_driver_t andix_ns_block_driver;

int andix_ns_block_probe(platform_device_t *dev);
void andix_ns_block_release(platform_device_t *dev);
uint32_t andix_ns_block_ioctl(platform_device_t *dev, uint32_t request, uintptr_t param, uint32_t psize);

#endif /* NS_BLOCK_H_ */
