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
 * Get physical memory address
 * @param vaddr the virutal address
 * @return the physical address or NULL if not mapped
 */
uintptr_t virt_to_phys(uintptr_t vaddr);

/**
 * Map a physical address into kernel memory
 *
 * @param paddr The physical address
 * @param size The size of memory to map
 * @param access The access flags AP_*
 * @param cache Cacheable
 * @param buffer Bufferable
 * @return The virtual address of the mapped memory or NULL on error
 */
uintptr_t map_phys_mem(uintptr_t paddr, uint32_t size, uint8_t access, uint8_t cache, uint8_t buffer);

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
