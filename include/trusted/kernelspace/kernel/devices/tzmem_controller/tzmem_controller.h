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
 * tzmem_controller.h
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#ifndef TZMEM_CONTROLLER_H_
#define TZMEM_CONTROLLER_H_

#include <common.h>

/**
 * \addtogroup tzmem_ctrl
 *	\{
 */

/**
 * Lock memory region from nonsecure access
 */
#define TZ_MEM_IOCTL_LOCK_MEMORY (0x1)

/**
 * Unlock memory region from nonsecure access
 */
#define TZ_MEM_IOCTL_UNLOCK_MEMORY (0x2)

/**
 * Dump info about tz memcontroller
 */
#define TZ_MEM_IOCTL_DUMP (0x3)

/**
 * ioctl parameter data structure for tz mem controller
 */
struct tz_mem_ioparam {
	void* physical_addr;
	uint32_t size;
};

typedef struct tz_mem_ioparam tz_mem_ioparam_t;

/**
 * Initialize the tz memory system
 *
 * @return HAL_*
 */
int32_t tzmem_init();

/**
 * Lock physical memory from nonsecure world
 *
 * @param paddr The physical addr
 * @param size The size of the memory area
 * @return HAL_*
 */
int32_t tzmem_lock(uintptr_t paddr, uint32_t size);

/**
 * Lock physical memory from nonsecure world
 *
 * @param paddr The physical addr
 * @param size The size of the memory area
 * @return HAL_*
 */
int32_t tzmem_unlock(uintptr_t* paddr, uint32_t size);

/**
 * Dump information about tz memory controller
 */
void tzmem_dump();

/** \} */

#endif /* TZMEM_CONTROLLER_H_ */
