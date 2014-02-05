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
 * mmu.h
 *
 *  Created on: Jan 11, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef MMU_H_
#define MMU_H_

/**
 * \addtogroup mmu
 *	\{
 */

/**
 * Gets the smallest boundary of virtual memory which is mapped
 * at addr.
 *
 * @param vtable Page directory to use
 * @param addr The virtual address
 * @param start The start of the memory block (out) 0 is invlalid
 * @param end The end of the memory block (out) 0 is invalid
 */
void vmm_get_virt_block_size(uintptr_t vtable, uint32_t addr, uintptr_t start, uintptr_t end);

/**
 * Unmap virtual address from kernel memory
 * @param vaddr the virtual address to unmap
 */
void unmap_kernel_memory(uint32_t vaddr);

/**
 * Unmap virtual address from page directory
 * @param vaddr the virtual address to unmap
 * @param vpd the virtual address to the page directory
 * @return the physical address if the page was mapped, 0 otherwise
 */
uint32_t unmap_memory_from_pd(uint32_t vaddr, uintptr_t vpd);

/**
 * Map memory to kernel
 * @param section the memory information
 */
int32_t map_kernel_memory(kernel_mem_info_t *section);

/**
 * Maps memory from start to end to kernel starting at vstart
 * Memory template is in section
 * @param start physical start address
 * @param end physical end address
 * @param vstart virtual start address
 * @param section the memory information
 */
int32_t map_kernel_sections(uint32_t start, uint32_t end, uint32_t vstart,
		kernel_mem_info_t *section);

/**
 * Dump page directory from vstart to vend from l1_tt
 * @param vstart The virtual start address
 * @param vend The virtual end address
 * @param l1_tt The first level page directory
 */
void dump_mmu(uint32_t vstart, uint32_t vend, uint32_t* l1_tt);

/**
 * Dump kernel page directory from vstart to vend
 * @param vstart The virtual start address
 * @param vend The virtual end address
 */
void dump_kernel_mmu(uint32_t vstart, uint32_t vend);

/**
 * Dump full kernel page directory
 */
void dump_mmu_full();

/**
 * Unmaps boot 1:1 mapping of kernel code
 */
void unmap_virtual_phys_kern();

/**
 * Maps memory section into page directory (vtable)
 * @param vtable The virtual page directory
 * @param section the memory information
 */
int32_t map_memory_v(uintptr_t vtable, kernel_mem_info_t *section);

/* \} group */

#endif /* MMU_H_ */
