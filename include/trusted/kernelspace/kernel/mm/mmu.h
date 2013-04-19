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
 * Gets the smallest boundary of virtual memory which is mapped
 * at addr.
 *
 * @param vtable Page directory to use
 * @param addr The virtual address
 * @param start The start of the memory block (out) 0 is invlalid
 * @param end The end of the memory block (out) 0 is invalid
 */
void vmm_get_virt_block_size(uintptr_t vtable, uint32_t addr, uintptr_t start, uintptr_t end);

void unmap_kernel_memory(uint32_t vaddr);
int32_t map_kernel_memory(kernel_mem_info_t *section);
int32_t map_kernel_sections(uint32_t start, uint32_t end, uint32_t vstart,
		kernel_mem_info_t *section);
void dump_mmu(uint32_t vstart, uint32_t vend, uint32_t* l1_tt);
void dump_kernel_mmu(uint32_t vstart, uint32_t vend);
void dump_mmu_full();
void unmap_virtual_phys_kern();
int32_t map_memory_v(uintptr_t vtable, kernel_mem_info_t *section);

#endif /* MMU_H_ */
