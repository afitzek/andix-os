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
 * umm.h
 *
 *  Created on: Jan 5, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef UMM_H_
#define UMM_H_

/**
 * \addtogroup umm
 *	\{
 */

#include <task/user_process.h>

#define USER_START_MAPPED_MEM 0x70001000

/**
 * Create a new page directory
 */
uintptr_t create_page_directory(uintptr_t* ptd);

/**
 * Maps initail user stack to virtual page directory
 * @param vptd The virtual page directory
 */
void map_initial_user_stack(uintptr_t vptd);

/**
 * Unmaps memory from task
 * @param vaddr The virutal address
 * @param size The size
 * @param proc
 */
void unmap_mem_from_task(uint8_t* vaddr, uint32_t size, struct user_process_t *proc);

/**
 * Map memory to task
 * @param paddr The physical address
 * @param size The size
 * @param proc
 */
uint8_t* map_mem_to_task(uint8_t* paddr, uint32_t size, struct user_process_t *proc);

/**
 * Frees a mapped page from task
 * @param page Address on the page
 * @param proc
 */
void free_task_mapped_page(void* page, struct user_process_t *proc);

/**
 * Allocates mapped pages in the task vm
 * @param numpages The number of pages
 * @param proc
 */
void* allocate_task_mapped_page_frames(uint32_t numpages, struct user_process_t *proc);

/**
 * Maps memory to userspace vptd
 * @param vptd The virtual page directory
 * @param section The memory info section
 */
void map_user_memory(uintptr_t vptd, kernel_mem_info_t *section);

/**
 * Allocates mapped memory to task
 * @param size The size of the mapped memory
 * @param proc
 */
uint8_t* allocate_map_mem_to_task(uint32_t size, struct user_process_t *proc);

/**
 * Fress memory from task
 * @param vaddr The virtual address
 * @param size The size to free
 * @param proc
 */
void free_mem_from_task(uint8_t* vaddr, uint32_t size, struct user_process_t *proc);

/**
 * Sets the TPIDRURO, User Read-Only Thread ID Register
 * @param p
 */
void set_user_thread_reg(uint32_t p);

/**
 * Reads the TPIDRURO, User Read-Only Thread ID Register
 * @return
 */
uint32_t get_user_thread_reg(void);

/* \} group */

#endif /* UMM_H_ */
