/*
 * mmm.h
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef MMM_H_
#define MMM_H_

#include <common/typedefs.h>
#include <kprintf.h>

// Mapable memory manager

/**
 * Initialize mapable memory
 * @param virt_start The virtual start address of mapable memory
 * @param virt_end The virtual end address of mapable memory
 */
void mmm_init(uint32_t virt_start, uint32_t virt_end);

/**
 * Dump mapable memory information
 */
void mmm_dump();

/**
 * Allocate a mapable memory page
 */
void* mmm_allocate_page(void);

/**
 * Allocate mapable memory pages
 * @param numpages Page count
 */
void* mmm_allocate_pages(uint32_t numpages);

/**
 * Free allocated mapable memory page
 * @param page The page address
 */
void mmm_free_page(void* page);

/**
 * The start address of the mapable memory space
 * @return the start of mmm mem
 */
uint32_t mmm_get_start_addr();

/**
 * The end address of the mapable memory space
 * @return the end of mmm mem
 */
uint32_t mmm_get_end_addr();

/**
 * Checks if given address is a mapable memory address
 * @param vaddr The virtual address
 * @return 0 if not in mmm space, -1 if in space but not mapped, 1 if in space and mapped
 */
uint8_t mmm_is_mem_addr(uintptr_t vaddr);

#endif /* MMM_H_ */
