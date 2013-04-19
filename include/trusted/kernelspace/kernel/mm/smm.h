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

// Stack memory manager

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


uintptr_t map_stack_mem(uint32_t size);

#endif /* MMM_H_ */
