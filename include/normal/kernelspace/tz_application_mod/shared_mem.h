/*
 * shared_mem.h
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

void free_mapable_memory(struct page* shared_pages, uint32_t size);
struct page* allocate_mapable_memory(uint32_t size, void** paddr, void** vaddr);
void invalidate(void* memory, uint32_t size);
void flush_cache(struct page* shared_pages);
void invalidate_clean(void* memory, uint32_t size);
void init_shared_mem();

#endif /* SHARED_MEM_H_ */
