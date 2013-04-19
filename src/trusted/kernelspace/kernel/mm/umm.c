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

	umm_debug("Mapping memory");

	map_memory_v(vptd, section);

	umm_debug("Memory mapped");
}

void* allocate_task_mapped_page_frames(uint32_t numpages, task_t *task) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < sizeof(task->membitmap) / 4; i++) {
		if (task->membitmap[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (task->membitmap[i] & (1 << j)) { // Find the set bit!
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
								task->membitmap[ti] = task->membitmap[ti]
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

void free_task_mapped_page(void* page, task_t *task) {
	int offset = ((uint8_t*) page - (uint8_t*) USER_START_MAPPED_MEM) / 4096;
	int i = offset / 32;
	int j = offset - (i * 32);
	task->membitmap[i] = task->membitmap[i] | (1 << j); // Mark as free!
}


uint8_t* map_mem_to_task(uint8_t* paddr, uint32_t size, task_t* task) {
	task_debug("Mapping mem 0x%x to task %d", paddr, task->tid);
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
	uint32_t frame_start = (uint32_t) allocate_task_mapped_page_frames(pgcount, task);
	info.paddr = (uint32_t)paddr;

	for(uint32_t i = 0; i < pgcount; i++) {
		info.vaddr = frame_start + SMALL_PAGE_SIZE * i;
		map_memory_v(task->vuserPD, &info);
	}
	task_debug("Memory mapped to 0x%x to task %d",
			frame_start, task->tid);

	return ((uint8_t*)((frame_start & 0xFFFFF000) | (((uint32_t)paddr) & 0xFFF)));
}

void unmap_mem_from_task(uint8_t* vaddr, uint32_t size, task_t* task) {
	// TODO
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

