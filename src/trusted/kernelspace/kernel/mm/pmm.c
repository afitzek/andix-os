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
 * pmm.c
 *
 *  Created on: Aug 25, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/pmm.h>
#include <mm/mm.h>
#include <hal.h>
#include <devices/tzmem_controller/tzmem_controller.h>

/**
 * This bitmap manages the physical RAM available
 * starting @ RAM_START
 *
 * 1 = free
 * 0 = allocated
 */
uint32_t* secure_phy_mem = NULL;

void* phys_start_addr = NULL;
void* phys_end_addr = NULL;
uint32_t pmm_bitmap_size;
list* physical_mem_list;

list* pmm_get_mem_list() {
	return (physical_mem_list);
}

/**
 * The first 2 MB of memory are used by boot code and stuff
 */
void pmm_init(uint32_t phys_start, uint32_t phys_end) {
	phys_start_addr = (void*) phys_start;
	phys_end_addr = (void*) phys_end;

	physical_mem_list = (list*) kmalloc(sizeof(list));
	physical_mem_list->next = physical_mem_list;
	physical_mem_list->prev = physical_mem_list;
	physical_mem_list->data = NULL;

	// (total / PAGE_SIZE * 8)
	pmm_bitmap_size = (phys_end - phys_start) / (4096 * 8);

	secure_phy_mem = (uint32_t*) kmalloc(pmm_bitmap_size);

	pmm_debug("Physical bitmap located @ 0x%x (%d B %d KB %d MB)",
			secure_phy_mem, pmm_bitmap_size, pmm_bitmap_size / 1024,
			pmm_bitmap_size / (1024 * 1024));

	pmm_debug("Physical start addr @ 0x%x", phys_start_addr);
	int i = 0;
	for (i = 0; i < pmm_bitmap_size / 4; i++) {
		secure_phy_mem[i] = 0xFFFFFFFF;
	}
}

void pmm_add_phys_mem_area(uint32_t pstart, uint32_t pend, uint32_t type) {
	phys_mem_area* area = (phys_mem_area*) kmalloc(sizeof(phys_mem_area));
	if (area == NULL ) {
		pmm_error("Failed to allocate memory for physical mem area!");
		return;
	}

	area->pstart = pstart;
	area->pend = pend;
	area->type = type;

	if (list_add(physical_mem_list, (void*) area) < 0) {
		pmm_error("Failed to allocate memory for physical mem area!");
		kfree((uintptr_t) area);
	}
}

void pmm_dump() {
	int i = 0;
	int j = 0;
	uint32_t free_pages = 0;
	uint32_t used_pages = 0;

	pmm_debug("Physical bitmap located @ 0x%x (%d B %d KB %d MB)",
			secure_phy_mem, pmm_bitmap_size, pmm_bitmap_size / 1024,
			pmm_bitmap_size / (1024 * 1024));

	pmm_debug("Physical start addr @ 0x%x", phys_start_addr);
	pmm_debug("Physical  end  addr @ 0x%x", phys_end_addr);

	// Mark free RAM
	for (i = 0; i < pmm_bitmap_size / 4; i++) {
		if (secure_phy_mem[i]) { // There is one bit set! (there is one page free)
			for (j = 0; j < 32; j++) {
				if (secure_phy_mem[i] & (1 << j)) { // Find the set bit!
					free_pages += 1;
				} else {
					used_pages += 1;
				}
			}
		} else {
			used_pages += 32;
		}

		if (secure_phy_mem[i] != 0xFFFFFFFF) {
			mmm_debug("   [%d]: 0x%x", i, secure_phy_mem[i]);
		}
	}

	pmm_debug("Pages used: %d", used_pages);
	pmm_debug("Pages free: %d", free_pages);

	pmm_info("Memory Area:    START   |     END   | TYPE");
	pmm_info("===============================================");
	list* pos;
	list* next;
	list_for_each_safe(pos, next, physical_mem_list)
	{
		phys_mem_area* area = (phys_mem_area*) pos->data;
		if (area) {
			pmm_info("Memory Area: 0x%x | 0x%x | %c", area->pstart, area->pend,
					(area->type == MEM_TYPE_SECURE) ? 'S' : 'N');
		}
	}
	pmm_info("===============================================");
}

int32_t pmm_protect_secure_mem() {
	uint32_t hal_res = 0;
	list* pos;
	list* next;
	list_for_each_safe(pos, next, physical_mem_list)
	{
		phys_mem_area* area = (phys_mem_area*) pos->data;
		if (area) {
			if(area->type == MEM_TYPE_SECURE) {
				pmm_info("Protecting: 0x%x | 0x%x | S",
						area->pstart, area->pend);
				hal_res = tzmem_lock((uintptr_t)area->pstart,
						area->pend - area->pstart);

				if(hal_res != HAL_SUCCESS) {
					pmm_error("Failed to lockdown secure memory area! %s (%d)",
							hal_get_error_string(hal_res), hal_res);
					return (-1);
				}

				pmm_info("Protected OK: 0x%x | 0x%x | S",
						area->pstart, area->pend);
			}

		}
	}
	return (0);
}

void* pmm_allocate_page(void) {
	int i = 0;
	int j = 0;

	// Mark free RAM
	for (i = 0; i < pmm_bitmap_size / 4; i++) {
		if (secure_phy_mem[i]) { // There is one bit set!  (there is one page free)
			for (j = 0; j < 32; j++) {
				if (secure_phy_mem[i] & (1 << j)) { // Find the set bit!
					// Found free page!
					secure_phy_mem[i] = secure_phy_mem[i] & ~(1 << j); // Mark as used!
					return ((uint8_t*) phys_start_addr + (i * 32 + j) * 4096);
				}
			}
		}
	}

	return (NULL );
}

void* pmm_allocate_aligned_pages(uint32_t numpages, uint32_t align) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < pmm_bitmap_size / 4; i++) {
		if (secure_phy_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (secure_phy_mem[i] & (1 << j)) { // Find the set bit!
					if (k == 0) {
						uint32_t saddr = (uint32_t) (((uint8_t*) phys_start_addr
								+ (i * 32 + j) * 4096));
						//pmm_debug("saddr: 0x%x", saddr);
						uint32_t padding = (uint32_t) (align
								- ((uint32_t) saddr & (align - 1)));
						//pmm_debug("padding: 0x%x", padding);
						if (padding != align) {
							// alignment mismatch!
							continue;
						}
						starti = i;
						startj = j;
						//mmm_debug("ALOC: start %d %d", i, j);
					} else if (k == numpages - 1) {

						//mmm_debug("ALOC: found %d %d", i, j);
						// found free stuff ...
						for (ti = starti; ti <= i; ti++) {
							int from = 0;
							int to = 32;

							if (ti == starti) {
								from = startj;
							}

							if (ti == i) {
								to = j;
							}
							//mmm_debug("ALOC: LOCK %d %d", i, j);
							for (tj = from; tj <= to; tj++) {
								secure_phy_mem[ti] = secure_phy_mem[ti]
										& ~(1 << tj);
							}
						}
						return ((uint8_t*) phys_start_addr
								+ (starti * 32 + startj) * 4096);
					}
					k++;
				} else {
					k = 0;
				}
			}
		} else {
			k = 0;
		}
	}

	return (NULL );
}

void* pmm_allocate_pages(uint32_t numpages) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < pmm_bitmap_size / 4; i++) {
		if (secure_phy_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (secure_phy_mem[i] & (1 << j)) { // Find the set bit!
					if (k == 0) {
						starti = i;
						startj = j;
						//mmm_debug("ALOC: start %d %d", i, j);
					}
					if (k == numpages - 1) {

						//mmm_debug("ALOC: found %d %d", i, j);
						// found free stuff ...
						for (ti = starti; ti <= i; ti++) {
							int from = 0;
							int to = 32;

							if (ti == starti) {
								from = startj;
							}

							if (ti == i) {
								to = j;
							}
							//mmm_debug("ALOC: LOCK %d %d", i, j);
							for (tj = from; tj <= to; tj++) {
								secure_phy_mem[ti] = secure_phy_mem[ti]
										& ~(1 << tj);
							}
						}
						return ((uint8_t*) phys_start_addr
								+ (starti * 32 + startj) * 4096);
					}
					k++;
				} else {
					k = 0;
				}
			}
		} else {
			k = 0;
		}
	}

	return (NULL );
}

void pmm_free_page(void* page) {
	int offset = ((uint8_t*) page - (uint8_t*) phys_start_addr) / 4096;
	int i = offset / 32;
	int j = offset - (i * 32);
	secure_phy_mem[i] = secure_phy_mem[i] | (1 << j); // Mark as free!
}
