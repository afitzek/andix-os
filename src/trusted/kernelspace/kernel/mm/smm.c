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
 * mmm.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/mm.h>

/**
 * This bitmap manages the virtual stack spaces
 * starting @ RAM_START
 *
 * 1 = free
 * 0 = allocated
 */
static uint32_t* stack_map_mem = NULL;

static void* stack_start_addr = NULL;
static void* stack_end_addr = NULL;
static uint32_t smm_bitmap_size;

/**
 *
 */
void smm_init(uint32_t virt_start, uint32_t virt_end) {
	stack_start_addr = (void*) virt_start;
	stack_end_addr = (void*) virt_end;

	// (total / PAGE_SIZE * 8)
	smm_bitmap_size = (virt_end - virt_start) / (4096 * 8);

	stack_map_mem = kmalloc(smm_bitmap_size);

	mmm_debug(
			"Stack Memory bitmap located @ 0x%x (%d B %d KB %d MB)", stack_map_mem, smm_bitmap_size, smm_bitmap_size / 1024, smm_bitmap_size / (1024 * 1024));

	mmm_debug("Stack Memory start addr @ 0x%x", stack_start_addr);
	int i = 0;
	for (i = 0; i < smm_bitmap_size / 4; i++) {
		stack_map_mem[i] = 0xFFFFFFFF;
	}
}

void smm_dump() {
	int i = 0;
	int j = 0;
	uint32_t free_pages = 0;
	uint32_t used_pages = 0;

	smm_debug(
			"Stack Memory located @ 0x%x (%d B %d KB %d MB)", stack_map_mem, smm_bitmap_size, smm_bitmap_size / 1024, smm_bitmap_size / (1024 * 1024));

	smm_debug("Stack Memory start addr @ 0x%x", stack_start_addr);
	smm_debug("Stack Memory  end  addr @ 0x%x", stack_end_addr);

	// Mark free RAM
	for (i = 0; i < smm_bitmap_size / 4; i++) {
		if (stack_map_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (stack_map_mem[i] & (1 << j)) { // Find the set bit!
					free_pages += 1;
				} else {
					used_pages += 1;
				}
			}

		} else {
			used_pages += 32;
		}

		if (stack_map_mem[i] != 0xFFFFFFFF) {
			smm_debug("Bitmap[%d]: 0x%x", i, stack_map_mem[i]);
		}
	}

	smm_debug("Pages used: %d", used_pages);
	smm_debug("Pages free: %d", free_pages);
}

uintptr_t map_stack_mem(uint32_t size) {

	uint32_t idx = 0;
	uint32_t frames = size / SMALL_PAGE_SIZE;
	if (frames * SMALL_PAGE_SIZE < size) {
		frames++;
	}
	frames++; // add one extra page to fault on stack overrun
	uint32_t vaddr = (uint32_t) smm_allocate_pages(frames);

	if (vaddr == 0) {
		return (NULL );
	}

	uint32_t paddr = (uint32_t) pmm_allocate_pages(frames - 1);

	if (paddr == 0) {
		// free vaddr
		for (idx = 0; idx < frames; idx++) {
			smm_free_page((uint8_t*) vaddr + SMALL_PAGE_SIZE * idx);
		}
		return (NULL );
	}

	vaddr += SMALL_PAGE_SIZE;

	kernel_mem_info_t mem;
	mem.type = SMALL_PAGE;
	mem.nonsecure = 0;
	mem.bufferable = 0;
	mem.cacheable = 0;
	mem.ap = AP_SVC_RW_USR_NO;
	mem.execute = EXEC_NON;
	mem.shareable = 0;
	mem.paddr = paddr;
	mem.vaddr = vaddr;

	for (idx = 0; idx < frames - 1; idx++) {
		mem.paddr = paddr + SMALL_PAGE_SIZE * idx;
		mem.vaddr = vaddr + SMALL_PAGE_SIZE * idx;
		map_kernel_memory(&mem);
		//smm_debug("Mapping Stack MEM [%d] 0x%x -> 0x%x (0x%x Bytes)", idx, mem.paddr, mem.vaddr, SMALL_PAGE_SIZE);
	}

	return ((uintptr_t) (mem.vaddr + 0xFF8));
}

void* smm_allocate_pages(uint32_t numpages) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < smm_bitmap_size / 4; i++) {
		if (stack_map_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (stack_map_mem[i] & (1 << j)) { // Find the set bit!
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
								stack_map_mem[ti] = stack_map_mem[ti]
										& ~(1 << tj);
							}
						}
						return ((uint8_t*) stack_start_addr
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

void smm_free_page(void* page) {
	int offset = ((uint8_t*) page - (uint8_t*) stack_start_addr) / 4096;
	int i = offset / 32;
	int j = offset - (i * 32);
	stack_map_mem[i] = stack_map_mem[i] | (1 << j); // Mark as free!
}

uint32_t smm_get_start_addr() {
	return ((uint32_t) stack_start_addr);
}

uint32_t smm_get_end_addr() {
	return ((uint32_t) stack_end_addr);
}

uint8_t smm_is_mem_addr(uintptr_t vaddr) {
	uint32_t nvaddr = (uint32_t) vaddr;

	if (nvaddr >= mmm_get_start_addr() && nvaddr < mmm_get_end_addr()) {
		// is in mem space but not mapped
		int offset = (nvaddr - mmm_get_start_addr()) / 4096;
		int i = offset / 32;
		int j = offset - (i * 32);
		if ((stack_map_mem[i] & (1 << j)) == 1) {
			// vaddr is available
			return (-1);
		}
		return (1);
	}

	return (0);
}
