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
 * vmm.h
 *
 *  Created on: Sep 7, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef VMM_H_
#define VMM_H_

#include <kprintf.h>
#include <common/atags.h>
#include <mm/pmm.h>
#include <mm/mmm.h>
#include <cache.h>

/**
 * \addtogroup vmm
 *	\{
 */

#define AP_SVC_NO_USR_NO 0x0
#define AP_SVC_RW_USR_NO 0x1
#define AP_SVC_RW_USR_RO 0x2
#define AP_SVC_RW_USR_RW 0x3
#define AP_SVC_RO_USR_NO 0x5
#define AP_SVC_RO_USR_RO 0x7

#define AP_MASK			 0x3
#define AP2_MASK		 0x4
#define AP2_SHIFT		 2

#define EXEC_NON     	1
#define EXEC_OK   		0

#define SECTION_SHIFT_BUFFER    2
#define SECTION_SHIFT_CACHE     3
#define SECTION_SHIFT_NONEXEC   4
#define SECTION_SHIFT_SHARED    16
#define SECTION_SHIFT_DOMAIN    5
#define SECTION_SHIFT_AP2       15
#define SECTION_SHIFT_AP        10
#define SECTION_SHIFT_TEX       12
#define SECTION_SHIFT_IMP       9
#define SECTION_SHIFT_NONSECURE 19
#define SECTION_SHIFT_NG        17

#define COARSE_TABLE_SHIFT_DOMAIN 5
#define COARSE_TABLE_SHIFT_NONSECURE 3

#define SMALL_PAGE_SHIFT_SHARE	10
#define SMALL_PAGE_SHIFT_AP2	9
#define SMALL_PAGE_SHIFT_AP		4
#define SMALL_PAGE_SHIFT_TEX	6
#define SMALL_PAGE_SHIFT_BUFFER	2
#define SMALL_PAGE_SHIFT_CACHE	3
#define SMALL_PAGE_SHIFT_NONEXEC 0

#define LARGE_PAGE_SHIFT_NONEXEC 	15
#define LARGE_PAGE_SHIFT_TEX 		12
#define LARGE_PAGE_SHIFT_SHARE 		10
#define LARGE_PAGE_SHIFT_TEX 		12
#define LARGE_PAGE_SHIFT_AP2		9
#define LARGE_PAGE_SHIFT_AP			4
#define LARGE_PAGE_SHIFT_BUFFER		2
#define LARGE_PAGE_SHIFT_CACHE		3

/*#define PHYSICAL_AIPSTZ_1_END 	0x53FF4000

#define PHYSICAL_AIPSTZ_2_START 0x63F00000
#define PHYSICAL_AIPSTZ_2_END 	0x63FFFFFF

#define VIRTUAL_KERNEL_START	0xC0000000*/

#define VIRTUAL_AIPSTZ_1_START 0x83F00000
#define VIRTUAL_AIPSTZ_2_START 0x93F00000

#define ENABLEMMU 0x00000001
#define ENABLEDCACHE 0x00000004
#define ENABLEICACHE 0x00001000
#define CHANGEMMU 0x00000001
#define CHANGEDCACHE 0x00000004
#define CHANGEICACHE 0x00001000
#define COMM_MEM 0x6400000 // 100 MB

#define TEX_DEFAULT			0
#define TEX_MASK			7

#define DOMAIN_MASK			0xF

#define COARSE_PAGE_SIZE	1024

#define SECTION_TYPE		2
#define PAGE_TABLE_TYPE		1
#define L1_TYPE_MASK		3
#define L2_TYPE_MASK		3

#define L2_ADDR_MASK		0xFFFFFC00

#define SMALL_PAGE_TYPE 	2
#define LARGE_PAGE_TYPE		1

#define SMALL_PAGE_SIZE		0x1000
#define LARGE_PAGE_SIZE		0x10000
#define SECTION_SIZE		0x100000

#define SMALL_PAGE_MASK		(0xFFFFF000)
#define LARGE_PAGE_MASK		(0xFFFF0000)
#define SECTION_MASK		(0xFFF00000)

typedef enum {
	LARGE_PAGE,
	SMALL_PAGE,
	SECTION,
	//SUPERSECTION
} mem_type;

typedef struct {
	uint32_t 	vaddr;
	uint32_t 	paddr;
	uint8_t  	ap;
	uint8_t  	execute;
	uint8_t 	bufferable;
	uint8_t 	cacheable;
	uint8_t 	domain;
	uint8_t		nonsecure;
	uint8_t		shareable;
	uint8_t		tex;
	mem_type	type;
} kernel_mem_info_t;

#define KERNEL_HEAP_SIZE 0x6400000

#define M_FREE_MASK 1

#define M_LAST_BLOCK(block) (k_mem_block_t*) ((uint8_t*) block - block->prev_size - sizeof(k_mem_block_t))
#define M_NEXT_BLOCK(block) (k_mem_block_t*) ((uint8_t*) block + block->size + sizeof(k_mem_block_t))

#define M_IS_FREE(block) ((block->flags & M_FREE_MASK) == 0)

typedef struct k_mem_block {
	uint32_t prev_size;
	uint32_t flags;
	uint32_t size;
} k_mem_block_t;

extern void* heap_end;
extern k_mem_block_t* heap_start;


uint32_t* get_user_table();
uint32_t* get_kernel_table();
void set_user_table(uint32_t* table);
void set_kernel_table(uint32_t* table);
int mmu_translate(void *va, uint32_t *pa);
void* v_to_p(void* ptr);
uint32_t getKernelVirtualOffset();
void* p_to_v(void* ptr);
void controlSet(uint32_t value, uint32_t mask);
void flushTLB(void);
void init_vmm(struct atag* startTag);
void init_heap();
void dump_heap();
void prepareBestBlock(k_mem_block_t* best, uint32_t size);
void clean_user();
uint32_t map_atags(uintptr_t phys);
uint32_t needed_pages(uint8_t* ptr, uint32_t size);
uint32_t check_align(uint32_t align);

/**
 * The start address of the kernel code memory space
 * @return the start of kernel code mem
 */
uint32_t vmm_get_kernel_code_start_addr();

/**
 * The end address of the kernel code memory space
 * @return the end of kernel code mem
 */
uint32_t vmm_get_kernel_code_end_addr();

/**
 * Checks if given address is a kernel code memory address
 * @param vaddr The virtual address
 * @return 0 if not in kernel code space, 1 if in space
 */
uint8_t vmm_is_kernel_code_mem_addr(uintptr_t vaddr);

/**
 * The start address of the kernel data memory space
 * @return the start of kernel data mem
 */
uint32_t vmm_get_kernel_data_start_addr();

/**
 * The end address of the kernel data memory space
 * @return the end of kernel data mem
 */
uint32_t vmm_get_kernel_data_end_addr();

/**
 * Checks if given address is a kernel data memory address
 * @param vaddr The virtual address
 * @return 0 if not in kernel data space, 1 if in space
 */
uint8_t vmm_is_kernel_data_mem_addr(uintptr_t vaddr);

/**
 * The start address of the kernel heap memory space
 * @return the start of kernel heap mem
 */
uint32_t vmm_get_kernel_heap_start_addr();

/**
 * The end address of the kernel heap memory space
 * @return the end of kernel heap mem
 */
uint32_t vmm_get_kernel_heap_end_addr();

/**
 * Checks if given address is a kernel heap memory address
 * @param vaddr The virtual address
 * @return 0 if not in kernel heap space, 1 if in space
 */
uint8_t vmm_is_kernel_heap_mem_addr(uintptr_t vaddr);

/* \} group */

#endif /* VMM_H_ */
