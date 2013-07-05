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
 * mmm.h
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SMM_H_
#define SMM_H_

#include <common/typedefs.h>
#include <kprintf.h>

/**
 * \addtogroup smm
 *	\{
 */
/**
 * Initialize stack memory manager
 * @param virt_start The virtual start address
 * @param virt_end The virtual end address
 */
void smm_init(uint32_t virt_start, uint32_t virt_end);

/**
 * Dump Stack memory manager information
 */
void smm_dump();

/**
 * Allocate stack memory page
 */
void* smm_allocate_page(void);

/**
 * Allocate stack memory pages
 * @param numpages count of pages
 */
void* smm_allocate_pages(uint32_t numpages);

/**
 * Free stack memory page
 * @param page The page address
 */
void smm_free_page(void* page);

/**
 * The start address of the kernel stack space
 * @return the start of stack mem
 */
uint32_t smm_get_start_addr();

/**
 * The end address of the stack memory space
 * @return the end of stack mem
 */
uint32_t smm_get_end_addr();

/**
 * Checks if given address is a stack memory address
 * @param vaddr The virtual address
 * @return 0 if not in stack space, 1 if in space
 */
uint8_t smm_is_mem_addr(uintptr_t vaddr);

/**
 * Maps stack memory of size
 * @param size the stack size
 */
uintptr_t map_stack_mem(uint32_t size);

/* \} group */

#endif /* MMM_H_ */
