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
 * umm.c
 *
 *  Created on: Dec 2, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

// User mode memory management
// Process Layout:
/*
 * 0x00000000 - 0x00000FFF  null pointer trap
 * 0x00001000 - 0xXXXXXXXX  .text
 * 0xXXXXXXXX - 0xXXXXXXXX  .data
 * 0xXXXXXXXX - 0xXXXXXXXX  .bss
 * 0xXXXXXXXX - 0xXXXXXXXX  .heap
 * ....
 * 0xXXXXXXXX - 0x6FFFFFF8  .stack
 * 0x70001000 - 0x76401000	.mapable
 */

#include <mm/mm.h>

void map_user_memory(uintptr_t vptd, kernel_mem_info_t *section) {
	if (section->vaddr >= 0x80000000) {
		umm_debug("MAP ERROR KERNEL ADR: 0x%x", section->vaddr);
		return;
	}

	if (section->ap == AP_SVC_RW_USR_NO || section->ap == AP_SVC_RO_USR_NO
			|| section->ap == AP_SVC_NO_USR_NO) {
		umm_debug("We should not map memory to userspace with no user access");
		return;
	}
	map_memory_v(vptd, section);

	umm_debug("Memory mapped 0x%x -> 0x%x", section->paddr, section->vaddr);
}

void* allocate_task_mapped_page_frames(uint32_t numpages, struct user_process_t *proc) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < sizeof(proc->membitmap) / 4; i++) {
		if (proc->membitmap[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (proc->membitmap[i] & (1 << j)) { // Find the set bit!
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
								proc->membitmap[ti] = proc->membitmap[ti]
										& ~(1 << tj);
							}
						}
						return ((uint8_t*) USER_START_MAPPED_MEM
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

void free_task_mapped_page(void* page, struct user_process_t *proc) {
	int offset = ((uint8_t*) page - (uint8_t*) USER_START_MAPPED_MEM) / 4096;
	int i = offset / 32;
	int j = offset - (i * 32);
	proc->membitmap[i] = proc->membitmap[i] | (1 << j); // Mark as free!
}

uint8_t* allocate_map_mem_to_task(uint32_t size, struct user_process_t *proc) {
	uint8_t* physical_mem = (uint8_t*)
			pmm_allocate_pages(needed_pages(0, size));
	if(physical_mem == NULL) {
		return (NULL);
	}
	return (map_mem_to_task(physical_mem, size, proc));
}

void free_mem_from_task(uint8_t* vaddr, uint32_t size, struct user_process_t *proc) {
	//uint32_t pages = needed_pages(0, size);
	uint8_t* paddr = v_to_p(vaddr);
	uint32_t freed = 0;

	unmap_mem_from_task(vaddr, size, proc);

	while(freed < size) {
		pmm_free_page(paddr);
		paddr += SMALL_PAGE_SIZE;
		freed += SMALL_PAGE_SIZE;
	}
}

uint8_t* map_mem_to_task(uint8_t* paddr, uint32_t size, struct user_process_t *proc) {
	task_debug("Mapping mem 0x%x to proc %d", paddr, proc->pid);
	kernel_mem_info_t info;
	info.ap = AP_SVC_RW_USR_RW;
	info.execute = EXEC_NON;
	info.bufferable = 1;
	info.cacheable = 1;
	info.nonsecure = 0;
	info.shareable = 0;
	info.type = SMALL_PAGE;
	uint32_t pgcount = needed_pages(paddr, size);
	task_debug("Needing %d pages", pgcount);
	uint32_t frame_start = (uint32_t) allocate_task_mapped_page_frames(pgcount, proc);
	info.paddr = (uint32_t)paddr;

	for(uint32_t i = 0; i < pgcount; i++) {
		info.vaddr = frame_start + SMALL_PAGE_SIZE * i;
		map_memory_v(proc->vuserPD, &info);
	}
	task_debug("Memory mapped to 0x%x to proc %d",
			frame_start, proc->pid);

	return ((uint8_t*)((frame_start & 0xFFFFF000) | (((uint32_t)paddr) & 0xFFF)));
}

void unmap_mem_from_task(uint8_t* vaddr, uint32_t size, struct user_process_t *proc) {
	uint32_t freed = 0;
	if((uint32_t)vaddr >= USER_START_MAPPED_MEM && ((uint32_t)vaddr + size) <
			(USER_START_MAPPED_MEM + sizeof(proc->membitmap) * 32 *
					SMALL_PAGE_SIZE)) {
		for(freed = 0; freed < size; freed += SMALL_PAGE_SIZE) {
			unmap_memory_from_pd((uint32_t)vaddr, proc->vuserPD);
		}
	} else {
		task_error("Trying to unmap not mapable memory 0x%x", vaddr);
	}
}

void map_initial_user_stack(uintptr_t vptd) {
	kernel_mem_info_t info;
	info.ap = AP_SVC_RW_USR_RW;
	info.execute = EXEC_NON;
	info.bufferable = 1;
	info.cacheable = 1;
	info.nonsecure = 0;
	info.shareable = 0;
	info.type = SMALL_PAGE;

	umm_debug("Allocating p stack");

	info.paddr = (uint32_t)pmm_allocate_page();
	info.vaddr = 0x78FFFFFF;

	umm_debug("Mapping stack to memory");

	map_user_memory(vptd, &info);
}

uintptr_t create_page_directory(uintptr_t* ptd) {
	// 2 frames each 4096 KB should be enough because we only need 2GB mapped upper 2GB are kernel
	(*ptd) = pmm_allocate_aligned_pages(2, (1 << 14));
	uintptr_t vptd = mmm_allocate_pages(2);

	kernel_mem_info_t pdinfo;
	pdinfo.ap = AP_SVC_RW_USR_NO;
	pdinfo.bufferable = 0;
	pdinfo.cacheable = 0;
	pdinfo.execute = EXEC_NON;
	pdinfo.nonsecure = 0;
	pdinfo.shareable = 0;
	pdinfo.type = SMALL_PAGE;
	pdinfo.tex = 0;

	map_kernel_sections((uint32_t) (*ptd), (uint32_t) (*ptd) + SMALL_PAGE_SIZE * 2 - 1,(uint32_t) vptd, &pdinfo);

	memset(vptd, 0x0, 2 * SMALL_PAGE_SIZE);

	return (vptd);
}

void set_user_thread_reg(uint32_t p) {
	asm volatile ("MCR p15, 0, %0, c13, c0, 3"
			:: "r" (p) : );
}

uint32_t get_user_thread_reg(void) {
	uint32_t p;
	asm volatile ("MRC p15, 0, %0, c13, c0, 3"
			: "=r" (p) :: );
	return p;
}
