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
 * random.h
 *
 *  Created on: Jan 14, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <common/typedefs.h>

/**
 * \addtogroup hal_random
 *	\{
 */

/**
 * IOCTL call to reseed device
 */
#define SRNG_IO_SEED		(0x01)

/**
 * IOCTL call to reseed device with x
 */
#define SRNG_IO_SEEDSIMPLE	(0x02)

/**
 * IOCTL call to reseed device with x and y
 */
#define SRNG_IO_SEEDMANUAL	(0x03)

/**
 * IOCTL call to get random number
 */
#define SRNG_IO_GETRAND		(0x04)

/**
 * Initializes random module
 * @return 0 on Success
 */
uint8_t random_init();

/**
 * Seeds the random source
 */
void random_seed();

/**
 * Seeds the random source with time information
 */
void random_seed_time();

/**
 * Seeds the random source with value
 * @param seed the seed value
 */
void random_seed_manual(uint64_t seed);

/**
 * Get random 8 bit value
 * @return the random byte
 */
uint8_t random_next8();

/**
 * Get random 16 bit value
 * @return the random value
 */
uint16_t random_next16();

/**
 * Get random 32 bit value
 * @return the random value
 */
uint32_t random_next32();

/**
 * Get random 64 bit value
 * @return the random value
 */
uint64_t random_next64();

/**
 * Get random 32 bit value with maximum value
 * @param max The maximum value
 * @return random value
 */
uint32_t random_max(uint32_t max);

/**
 * Get random 32 bit value with min and max range
 * @param min The min value
 * @param max The max value
 * @return the random value
 */
uint32_t random_min_max(uint32_t min, uint32_t max);

/**
 * Fill buffer with random values
 * @param buffer the buffer
 * @param len the length
 */
void random_fill(uint8_t *buffer, uint32_t len);

/* \} group */

#endif /* RANDOM_H_ */
