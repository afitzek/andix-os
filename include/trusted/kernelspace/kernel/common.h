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
 * common.h
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <common/list.h>
#include <common/atags.h>
#include <common/time.h>
#include <common/mutex.h>
#include <common/string.h>
#include <common/typedefs.h>
#include <common/debug.h>
#include <common/queue.h>

uint32_t getCPSR();
void *memset(void *s, int c, unsigned int n);
void *memmove(void *d, const void *s, unsigned int n);
char* getModeString(uint32_t cpsr);

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define roundup(x, y)		((((x) + ((y) - 1)) / (y)) * (y))
uint8_t is_kernel_addr(uint32_t value);
uint8_t is_valid_addr(void* ptr);
uint8_t is_valid_user_addr(void* ptr);
uint8_t is_valid_kernel_addr(void* ptr);
uint8_t is_valid_nonsecure_ram_addr(void* ptr);

#endif /* COMMON_H_ */
