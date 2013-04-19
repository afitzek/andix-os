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

// Physical memory manager

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

#endif /* PMM_H_ */
