/*
 * shared_mem.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <tz_application_mod/andixtee.h>
#include <asm-generic/cacheflush.h>

int needed_log_pages(size_t memsize) {
	int pgcount = memsize / PAGE_SIZE;
	int log_alloc = 0;
	int pgcount_alloc = ipow(2, log_alloc);  // 2 ^ 0

	while (pgcount_alloc < pgcount) {
		log_alloc++;
		pgcount_alloc = ipow(2, log_alloc);
	}

	return (log_alloc);
}

void flush_cache(struct page* shared_pages) {
	flush_dcache_page(shared_pages);
}

uint32_t cl_size;

void invalidate(void* memory, uint32_t size) {
	uint32_t start = (uint32_t) memory;
	uint32_t end = start + size;
	start = round_down(start, cl_size);
	end = round_up(end, cl_size);
	while (start < end) {
		asm ("mcr p15, 0, %0, c7, c6, 1" : : "r" (start)
				: "memory");
		start += cl_size;
	}
	CP15DSB;
	CP15ISB;
}

void invalidate_clean(void* memory, uint32_t size) {
	uint32_t start = (uint32_t) memory;
	uint32_t end = start + size;
	start = round_down(start, cl_size);
	end = round_up(end, cl_size);
	while (start < end) {
		asm ("mcr p15, 0, %0, c7, c14, 1" : : "r" (start)
				: "memory");
		start += cl_size;
	}
	CP15DSB;
	CP15ISB;
}


void init_shared_mem(void) {
	uint32_t ctr;

	asm volatile("mrc p15, 0, %0, c0, c0, 1" : "=r" (ctr));
	cl_size = 4 << ((ctr >> 16) & 0xf);
}

void free_mapable_memory(struct page* shared_pages, uint32_t size) {
	int pages = needed_log_pages(size);
	if (shared_pages != NULL ) {
		__free_pages(shared_pages, pages);
	}
}

struct page* allocate_mapable_memory(uint32_t size, void** paddr, void** vaddr) {
	struct page* shared_pages;
	int pages = needed_log_pages(size);

	shared_pages = alloc_pages(GFP_KERNEL | GFP_DMA, pages);
	if (shared_pages == NULL ) {
		printk(KERN_ERR "Failed to allocate memory\n");
		return (0);
	}
	(*vaddr) = (void*) page_address(shared_pages);
	(*paddr) = (void*) virt_to_phys((*vaddr));
	//printk(KERN_INFO "Mapable memory @ 0x%x p (0x%x)\n",
	//		(unsigned int) (*vaddr), (unsigned int) (*paddr));

	return (shared_pages);
}
