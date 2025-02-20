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
 * random.c
 *
 *  Created on: Jan 14, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <devices/random/random.h>
#include <hal.h>

platform_device_t* rand_device;

uint8_t random_init() {
	rand_device = NULL;
	rand_device = hal_find_device(RANDOM_DEVICE, 0);
	if (rand_device) {
		random_seed_time();
		return (0);
	}
	return (1);
}

void random_seed() {
	if (rand_device) {
		hal_ioctl(rand_device, SRNG_IO_SEED, 0, 0);
	}
}

void random_seed_time() {
	if (rand_device) {
		clock_t value = clock();
		hal_ioctl(rand_device, SRNG_IO_SEEDSIMPLE, (uintptr_t)value, 0);
	}
}

void random_seed_manual(uint64_t seed) {
	uint32_t mseed[2];
	if (rand_device) {
		mseed[0] = (uint32_t) seed;
		mseed[1] = (uint32_t) (seed >> 32);
		hal_ioctl(rand_device, SRNG_IO_SEEDMANUAL, mseed, sizeof(mseed));
	}
}

uint8_t random_next8() {
	if (rand_device) {
		uint32_t result = 1;
		hal_ioctl(rand_device, SRNG_IO_GETRAND, &result, 0);
		return ((uint8_t) result & 0xFF);
	}
	return (0);
}

uint16_t random_next16() {
	if (rand_device) {
		uint32_t result = 1;
		hal_ioctl(rand_device, SRNG_IO_GETRAND, &result, 0);
		return ((uint16_t) result & 0xFFFF);
	}
	return (0);
}

uint32_t random_next32() {
	if (rand_device) {
		uint32_t result = 1;
		hal_ioctl(rand_device, SRNG_IO_GETRAND, &result, 0);
		return (result);
	}
	return (0);
}

uint64_t random_next64() {
	if (rand_device) {
		uint64_t result1 = 1;
		uint64_t result2 = 1;
		uint64_t result = 1;
		hal_ioctl(rand_device, SRNG_IO_GETRAND, (uintptr_t)&result1, 0);
		hal_ioctl(rand_device, SRNG_IO_GETRAND, (uintptr_t)&result2, 0);
		result = (result1 << 32);
		result = result | result2;
		return (result);
	}
	return (0);
}

uint32_t random_max(uint32_t max) {
	uint32_t rand = random_next32() % max;
	return (rand);
}

uint32_t random_min_max(uint32_t min, uint32_t max) {
	if(min > max) {
		return (min);
	}

	uint32_t diff = max - min;

	return (random_max(diff) + min);
}

void random_fill(uint8_t *buffer, uint32_t len) {

	if(!is_valid_addr(buffer)) {
		return;
	}

	uint32_t idx = 0;
	for(idx = 0; idx < len; idx++) {
		buffer[idx] = random_next8();
	}
}
