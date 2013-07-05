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
 * simplerng.c
 *
 *  Created on: Jan 13, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <drivers/random/simplerng.h>
#include <mm/mm.h>

void srng_seed(srng_data_t* srng) {
	if (srng != NULL ) {
		srng->m_w = 521288629;
		srng->m_z = 62436069;
	}
}

void srng_seed_simple(srng_data_t* srng, uint32_t seed) {
	if (srng != NULL ) {
		if (seed != 0) {
			srng->m_w = seed;
		}
	}
}

void srng_seed_manual(srng_data_t* srng, uint32_t seed1, uint32_t seed2) {
	if (srng != NULL ) {
		if (seed1 != 0) {
			srng->m_w = seed1;
		}
		if (seed2 != 0) {
			srng->m_z = seed2;
		}
	}
}

uint32_t srng_get_random(srng_data_t* srng) {
	if (srng != NULL ) {
		srng->m_z = 36969 * (srng->m_z & 65535) + (srng->m_z >> 16);
		srng->m_w = 18000 * (srng->m_w & 65535) + (srng->m_w >> 16);
		return ((srng->m_z << 16) + srng->m_w);
	}
	return (0);
}

void srng_walk_seed_rounds(srng_data_t* srng) {
	if (srng != NULL ) {
		uint32_t i = 0;
		for(i = 0; i < ROUNDS_WARMUP; i++) {
			srng_get_random(srng);
		}
	}
}

int srng_probe(platform_device_t *dev) {
	if (!dev) {
		return HAL_E_OUT_OF_MEMORY;
	}

	srng_data_t* srng = (srng_data_t*) kmalloc(sizeof(srng_data_t));

	if (srng == NULL ) {
		hal_error("SRNG failed to allocate memory for structure!");
		return HAL_E_OUT_OF_MEMORY;
	}

	srng_seed(srng);

	dev->device_data = (uintptr_t)srng;

	return HAL_SUCCESS;
}

void srng_release(platform_device_t *dev) {
	if (dev) {
		if (dev->device_data != NULL ) {
			kfree(dev->device_data);
		}
	}
}

uint32_t srng_ioctl(platform_device_t *dev, uint32_t request, uintptr_t param,
		uint32_t psize) {
	if (!dev) {
		hal_error("device is null");
		return HAL_E_IOCTL_PARA_INVALID;
	}

	if (!(dev->device_data)) {
		hal_error("device data is null");
		return HAL_E_IOCTL_PARA_INVALID;
	}

	switch (request) {
	case SRNG_IO_SEED:
		srng_seed((srng_data_t*) dev->device_data);
		srng_walk_seed_rounds((srng_data_t*) dev->device_data);
		return HAL_SUCCESS;
	case SRNG_IO_SEEDSIMPLE:
		srng_seed_simple((srng_data_t*) dev->device_data, (uint32_t) param);
		srng_walk_seed_rounds((srng_data_t*) dev->device_data);
		return HAL_SUCCESS;
	case SRNG_IO_SEEDMANUAL:
		if (psize != 8) {
			return HAL_E_IOCTL_PARA_INVALID;
		}

		if (!is_valid_kernel_addr(param)) {
			return HAL_E_IOCTL_PARA_INVALID;
		}
		srng_seed_manual((srng_data_t*) dev->device_data, param[0], param[1]);
		srng_walk_seed_rounds((srng_data_t*) dev->device_data);
		return HAL_SUCCESS;
	case SRNG_IO_GETRAND:

		if (!is_valid_kernel_addr(param)) {
			return HAL_E_IOCTL_PARA_INVALID;
		}

		(*param) = srng_get_random((srng_data_t*) dev->device_data);
		return HAL_SUCCESS;
	default:
		return HAL_E_IOCTL_REQ_NOT_AVAIL;
	}
}

static const platform_driver_t srng_driver = { .probe = srng_probe,
		.release = srng_release, .write = NULL, .read = NULL,
		.ioctl = srng_ioctl,
		.driver = { .name = SRNG_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(srng_driver)
