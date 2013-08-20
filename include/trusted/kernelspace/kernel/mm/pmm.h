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
 * pmm.h
 *
 *  Created on: Aug 25, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef PMM_H_
#define PMM_H_

#include <kprintf.h>
#include <common/list.h>

/**
 * \addtogroup pmm
 *	\{
 */

#define MEM_TYPE_SECURE 	(1)
#define MEM_TYPE_UNSECURE 	(0)

/**
 * Physical memory area
 */
typedef struct {
	uint32_t pstart;	/**< The start address */
	uint32_t pend;		/**< The end addess */
	uint32_t type;		/**< The memory type */
} phys_mem_area;

/**
 * Initializes physical memory manager
 * @param phys_start The physical start address
 * @param phys_end The physical end address
 */
void pmm_init(uint32_t phys_start, uint32_t phys_end);

/**
 * Gets a list with physical memory regions available
 * @return
 */
list* pmm_get_mem_list();

/**
 * Adds an unsecure phys mem area
 * @param pstart The physical start address
 * @param pend The physical end address
 * @param type The mem type
 */
void pmm_add_phys_mem_area(uint32_t pstart, uint32_t pend, uint32_t type);

/**
 * Dump physical memory manager information
 */
void pmm_dump();

/**
 * Allocate physical memory page
 */
void* pmm_allocate_page(void);

/**
 * Allocate physical aligned pages
 * @param numpages count of pages
 * @param align alignment
 */
void* pmm_allocate_aligned_pages(uint32_t numpages, uint32_t align);

/**
 * Allocate physical pages
 * @param numpages count of pages
 */
void* pmm_allocate_pages(uint32_t numpages);

/**
 * Free physical memory page
 * @param page address of page
 */
void pmm_free_page(void* page);

/**
 * Activate HW protection for secure memory!
 *
 * @return 0 on success -1 on failure
 */
int32_t pmm_protect_secure_mem();

/**
 * Stops the system if addr is in protected memory region
 *
 * @param addr The physical memory address
 */
void pmm_stop_on_protected_mem(uint32_t addr);

/* \} group */

#endif /* PMM_H_ */
