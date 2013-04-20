/*
 * mmm.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/mm.h>

/**
 * This bitmap manages the physical RAM available
 * starting @ RAM_START
 *
 * 1 = free
 * 0 = allocated
 */
static uint32_t* secure_map_mem = NULL;

static void* virt_start_addr = NULL;
static void* virt_end_addr = NULL;
static uint32_t mmm_bitmap_size;

/**
 * The first 2 MB of memory are used by boot code and stuff
 */
void mmm_init(uint32_t virt_start, uint32_t virt_end) {
	virt_start_addr = (void*) virt_start;
	virt_end_addr = (void*) virt_end;

	// (total / PAGE_SIZE * 8)
	mmm_bitmap_size = (virt_end - virt_start) / (4096 * 8);

	secure_map_mem = kmalloc(mmm_bitmap_size);

	mmm_debug(
			"Mapped Memory bitmap located @ 0x%x (%d B %d KB %d MB)", secure_map_mem, mmm_bitmap_size, mmm_bitmap_size / 1024, mmm_bitmap_size / (1024 * 1024));

	mmm_debug("Mapped Memory start addr @ 0x%x", virt_start_addr);
	int i = 0;
	for (i = 0; i < mmm_bitmap_size / 4; i++) {
		secure_map_mem[i] = 0xFFFFFFFF;
	}
}

uint32_t mmm_get_start_addr() {
	return ((uint32_t)virt_start_addr);
}

uint32_t mmm_get_end_addr() {
	return ((uint32_t)virt_end_addr);
}

void mmm_dump() {
	int i = 0;
	int j = 0;
	uint32_t free_pages = 0;
	uint32_t used_pages = 0;

	mmm_debug(
			"Mapped Memory located @ 0x%x (%d B %d KB %d MB)", secure_map_mem, mmm_bitmap_size, mmm_bitmap_size / 1024, mmm_bitmap_size / (1024 * 1024));

	mmm_debug("Mapped Memory start addr @ 0x%x", virt_start_addr);
	mmm_debug("Mapped Memory  end  addr @ 0x%x", virt_end_addr);

	// Mark free RAM
	for (i = 0; i < mmm_bitmap_size / 4; i++) {
		if (secure_map_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (secure_map_mem[i] & (1 << j)) { // Find the set bit!
					free_pages += 1;
				} else {
					used_pages += 1;
				}
			}

		} else {
			used_pages += 32;
		}

		if (secure_map_mem[i] != 0xFFFFFFFF) {
			mmm_debug("Bitmap[%d]: 0x%x", i, secure_map_mem[i]);
		}
	}

	mmm_debug("Pages used: %d", used_pages);
	mmm_debug("Pages free: %d", free_pages);
}

void* mmm_allocate_page(void) {
	int i = 0;
	int j = 0;

	// Mark free RAM
	for (i = 0; i < mmm_bitmap_size / 4; i++) {
		if (secure_map_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (secure_map_mem[i] & (1 << j)) { // Find the set bit!
					// Found free page!
					secure_map_mem[i] = secure_map_mem[i] & ~(1 << j); // Mark as used!
					return ((uint8_t*) virt_start_addr + (i * 32 + j) * 4096);
				}
			}
		}
	}

	return (NULL );
}

uintptr_t map_io_mem(uintptr_t pstart, uint32_t size) {
	kernel_mem_info_t section_description;

	uint32_t idx = 0;
	uint32_t frames = size / SMALL_PAGE_SIZE;
	if(frames * SMALL_PAGE_SIZE < size) {
		frames++;
	}
	uint32_t vaddr = 0;

	// Setup IO Mappings
	section_description.domain = 0;
	section_description.ap = AP_SVC_RW_USR_NO;
	section_description.execute = EXEC_NON;
	section_description.tex = 0x0;
	section_description.cacheable = 0;
	section_description.bufferable = 0;
	section_description.nonsecure = 0;
	section_description.shareable = 0;
	section_description.type = SMALL_PAGE;

	if (frames == 1) {
		section_description.paddr = (uint32_t) pstart;
		vaddr = (uint32_t) mmm_allocate_page();
		section_description.vaddr = vaddr;
		map_kernel_memory(&section_description);
	} else {
		vaddr = (uint32_t) mmm_allocate_pages(frames);
		for (idx = 0; idx < frames; idx++) {
			section_description.paddr = (uint32_t) pstart
					+ SMALL_PAGE_SIZE * idx;
			section_description.vaddr = vaddr + SMALL_PAGE_SIZE * idx;
			map_kernel_memory(&section_description);
		}
	}

	mmm_debug(
			"Mapping IO MEM [%d] 0x%x -> 0x%x (0x%x Bytes)", frames, pstart, vaddr, size);

	return ((uintptr_t) vaddr);
}

void* mmm_allocate_pages(uint32_t numpages) {
	int i = 0;
	int j = 0;
	int k = 0;
	int ti = 0;
	int tj = 0;
	int starti = 0;
	int startj = 0;

	// Mark free RAM
	for (i = 0; i < mmm_bitmap_size / 4; i++) {
		if (secure_map_mem[i]) { // There is one bit set!
			for (j = 0; j < 32; j++) {
				if (secure_map_mem[i] & (1 << j)) { // Find the set bit!
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
								secure_map_mem[ti] = secure_map_mem[ti]
										& ~(1 << tj);
							}
						}
						return ((uint8_t*) virt_start_addr
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

void mmm_free_page(void* page) {
	int offset = ((uint8_t*) page - (uint8_t*) virt_start_addr) / 4096;
	int i = offset / 32;
	int j = offset - (i * 32);
	secure_map_mem[i] = secure_map_mem[i] | (1 << j); // Mark as free!
}

uint8_t mmm_is_mem_addr(uintptr_t vaddr) {
	uint32_t nvaddr = (uint32_t) vaddr;

	if(nvaddr >= mmm_get_start_addr() && nvaddr < mmm_get_end_addr()) {
		// is in mem space but not mapped
		int offset = (nvaddr - mmm_get_start_addr()) / 4096;
		int i = offset / 32;
		int j = offset - (i * 32);
		if((secure_map_mem[i] & (1 << j)) == 1) {
			// vaddr is available
			return (-1);
		}
		return (1);
	}

	return (0);
}
