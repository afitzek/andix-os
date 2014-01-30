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
 * mm.h
 *
 *  Created on: Nov 18, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef MM_H_
#define MM_H_

#include <mm/vmm.h>
#include <mm/mmm.h>
#include <mm/pmm.h>
#include <mm/smm.h>
#include <mm/umm.h>
#include <mm/mmu.h>

/**
 * Translates physical to virtual address
 */
#define mapped_p_to_v(vaddr,paddr) (((uint32_t) vaddr & 0xFFFFF000) | (paddr & 0xFFF))

/**
 * Type of memory space
 */
typedef enum {
	USER_MEM,			/**< User memory*/
	MAPABLE_MEM,		/**< Mapable memory */
	KERNEL_CODE,		/**< Kernel code */
	KERNEL_DATA,		/**< Kernel data memory (.BSS and .DATA) */
	KERNEL_HEAP,		/**< Kernel heap memory */
	STACK_MEM,			/**< Stack memory */
} mem_space;

#define SECURE_MEMORY_SIZE	(0x10000000)

/**
 * Allocate memory in kernel heap
 * @param size the size of the memory in byte
 * @return Address of the reserved block
 */
uintptr_t kmalloc(uint32_t size);

/**
 * Allocate aligned memory in kernel heap
 * @param size the size of the memory in byte
 * @param align align the memory
 * @return Address of the reserved block
 */
uintptr_t kmalloc_align(uint32_t size, uint32_t align);

/**
 * Free allocated kernel memory
 * @param blk Heap address
 */
void kfree(uintptr_t blk);

/**
 * Get physical memory address for non secure virtual address
 * @param vaddr the virutal address
 * @return the physical address or NULL if not mapped
 */
uintptr_t virt_ns_to_phys(uintptr_t vaddr);

/**
 * Map a physical address into kernel memory
 *
 * @param paddr The physical address
 * @param size The size of memory to map
 * @param access The access flags AP_*
 * @param cache Cacheable
 * @param buffer Bufferable
 * @param nonsecure Non secure page
 * @return The virtual address of the mapped memory or NULL on error
 */
uintptr_t map_phys_mem(uintptr_t paddr, uint32_t size,
		uint8_t access, uint8_t cache, uint8_t buffer,
		uint8_t nonsecure);

/**
 * Maps IO memory
 * @param pstart The physical start address
 * @param size The memory size
 * @return The virtual address of NULL on error
 */
uintptr_t map_io_mem(uintptr_t pstart, uint32_t size);

/**
 * Unmap memory from the kernel space
 *
 * Works for io mem, phys mem and stacks
 * @param addr The virtual address
 * @param size The size of the memory to unmap
 */
void unmap_memory(uintptr_t addr, uint32_t size);

/**
 * Determines the mem space type
 * @param addr The mem addr
 * @return The mem space type
 */
mem_space mem_get_type(uintptr_t addr);

#endif /* MM_H_ */
