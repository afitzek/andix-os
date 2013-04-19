/*
 * simplerng.h
 *
 *  Created on: Jan 13, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SIMPLERNG_H_
#define SIMPLERNG_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>
#include <devices/random/random.h>

#define ROUNDS_WARMUP		(0x176)

#define SRNG_DRIVER_NAME "SRNG"

typedef struct {
	uint32_t	m_w;
	uint32_t	m_z;
} srng_data_t;

/*
m_z = 36969 * (m_z & 65535) + (m_z >> 16);
m_w = 18000 * (m_w & 65535) + (m_w >> 16);
return (m_z << 16) + m_w;
*/

int srng_probe(platform_device_t *dev);
void srng_release(platform_device_t *dev);
uint32_t srng_ioctl(platform_device_t *dev, uint32_t request, uintptr_t param, uint32_t psize);
void init_srng(platform_device_t *dev);

#endif /* KISS_PRNG_H_ */
