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
 * mm.c
 *
 *  Created on: Jan 11, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/mm.h>

uintptr_t kmalloc(uint32_t size) {
	k_mem_block_t* current;
	k_mem_block_t* best;

	best = (k_mem_block_t*) NULL;

	if (size == 0) {
		return ((uintptr_t) best);
	}

	//vmm_debug("Allocating %d Bytes", size);

	current = heap_start;

	while ((void*) current < heap_end) {
		//vmm_debug("Looking Block @ 0x%x", current);
		if (M_IS_FREE(current)) {

			// this block would fit
			if (current->size >= size) {
				if (best == 0) {
					best = current;
				} else {
					if (best->size > current->size) {
						best = current;
					}
				}
			}
		}

		current = (k_mem_block_t*) ((uint8_t*) current + current->size
				+ sizeof(k_mem_block_t));
		//vmm_debug("Looking Block @ 0x%x", current);
	}

	if (best == 0) {
		// OUT OF MEMORY!
		vmm_debug("OUT OF MEMORY!!!");
		return ((uintptr_t) best);
	}

	prepareBestBlock(best, size);
	//vmm_debug("Allocating MEM @ 0x%x", (uint8_t*) best + sizeof(k_mem_block_t));
	return ((uintptr_t) ((void*) ((uint8_t*) best + sizeof(k_mem_block_t))));
}

void kfree(uintptr_t blk) {
	if (blk >= (uintptr_t) heap_start && blk < (uintptr_t) heap_end) {
		//vmm_debug("Freeing MEM @ 0x%x", blk);
		k_mem_block_t* current = (k_mem_block_t*) ((uint8_t*) blk
				- sizeof(k_mem_block_t));
		k_mem_block_t* last = M_LAST_BLOCK(current);
		k_mem_block_t* next = M_NEXT_BLOCK(current);

		//vmm_debug("CURRENT BLOCK: 0x%x", current);
		//vmm_debug("LAST    BLOCK: 0x%x", last);
		//vmm_debug("NEXT    BLOCK: 0x%x", next);

		if (last >= heap_start) {
			if (M_IS_FREE(last)) {
				// merge with this block ...
				last->size = last->size + sizeof(k_mem_block_t) + current->size;
				current = last;
			}
		}

		if ((void*) next < heap_end) {
			if (M_IS_FREE(next)) {
				current->size = current->size + sizeof(k_mem_block_t)
						+ next->size;
				next = (k_mem_block_t*) ((uint8_t*) current + current->size
						+ sizeof(k_mem_block_t));
			}
			if ((void*) next < heap_end) {
				next->prev_size = current->size;
			}
		}
		current->flags = 0;
	}
}

uintptr_t kmalloc_align(uint32_t size, uint32_t align) {
	k_mem_block_t* current;
	k_mem_block_t* best;
	k_mem_block_t* last;

	best = (k_mem_block_t*) NULL;

	// TODO: find better way to check!
	align = (uint32_t) check_align(align);

	//vmm_debug("Allocating %d Bytes aligned %d", size, align);

	if (size == 0) {
		return ((uintptr_t) best);
	}

	current = heap_start;
	uint32_t padding = 0;
	uint8_t* offset = 0;
	while ((void*) current < heap_end) {
		//vmm_debug("Looking Block @ 0x%x", current);
		if (M_IS_FREE(current)) {
			offset = (uint8_t*) current + sizeof(k_mem_block_t);
			padding = (uint32_t) (align - ((uint32_t) offset & (align - 1)));
			//vmm_debug("   OFFSET 0x%x PADDING 0x%x", offset, padding);
			// this block would fit
			if(padding < current->size) {
				if (current->size - padding >= size) {
					if (best == 0) {
						best = current;
					} else {
						if (best->size > current->size) {
							best = current;
						}
					}
				}
			}
		}

		current = (k_mem_block_t*) ((uint8_t*) current + current->size
				+ sizeof(k_mem_block_t));
	}

	if (best == 0) {
		// OUT OF MEMORY!
		vmm_debug("OUT OF MEMORY!!!");
		return ((uintptr_t) best);
	}

	// best block fits block alignment!
	last = M_LAST_BLOCK(best);
	offset = (uint8_t*) best + sizeof(k_mem_block_t);
	padding = (uint32_t) (align - ((uint32_t) offset & (align - 1)));

	last->size += padding;
	uint32_t bsize = best->size;
	uint8_t* newoffset = offset + padding;
	best = (k_mem_block_t*) (newoffset - sizeof(k_mem_block_t));
	best->prev_size = last->size;
	best->size = bsize - padding;
	//vmm_debug("   OFFSET 0x%x PADDING 0x%x => NEWOFFSET 0x%x",
	//	offset, padding, newoffset);
	//vmm_debug("   BEST 0x%x LAST @ 0x%x (0x%x) -> 0x%x",
	//	best, last, last->size, M_NEXT_BLOCK(last));

	prepareBestBlock(best, size);

	//vmm_debug(
	//		"Allocating MEM @ 0x%x (%d)", (uint8_t*) best + sizeof(k_mem_block_t), best->size);
	return ((uintptr_t) ((uint8_t*) best + sizeof(k_mem_block_t)));
}

uintptr_t virt_to_phys(uintptr_t vaddr) {
	void* ptr = v_to_p((void*) vaddr);
	if (ptr < (void*) 0xFF) {
		return (NULL );
	}
	return ((uintptr_t) ptr);
}

uintptr_t virt_ns_to_phys(uintptr_t vaddr) {
	uint32_t addr;
	//vmm_debug("Translating v (0x%x)", ptr);
	__asm__ __volatile__(
			"MCR p15, 0, %1, c7, c8, 4\n"
			"MRC p15, 0, %0, c7, c4, 0":
			"=r" (addr): "r" (vaddr):"memory", "cc");

	if (addr < 0xFF) {
		vmm_debug("Failed to translate nonsecure v (0x%x) to p", vaddr);
		return (NULL);
	}

	addr = (addr & 0xFFFFF000) + ((uint32_t) vaddr & 0xFFF);

	return ((void*) addr);
}

uintptr_t map_phys_mem(uintptr_t paddr, uint32_t size, uint8_t access,
		uint8_t cache, uint8_t buffer, uint8_t nonsecure) {
	uint32_t pstart = (uint32_t) paddr;
	uint32_t pend = pstart + size;

	uint32_t pgcount = needed_pages((uint8_t*) paddr, size);

	//vmm_debug("Needing %d pages", pgcount);

	uint32_t vstart = (uint32_t) mmm_allocate_pages(pgcount);

	if (vstart == 0) {
		vmm_debug("Failed to allocate mapable memory");
		return (NULL );
	}

	kernel_mem_info_t info;
	info.domain = 0;
	info.ap = access;
	info.bufferable = buffer;
	info.cacheable = cache;
	info.nonsecure = nonsecure;
	info.execute = EXEC_NON;
	info.shareable = 0;
	info.tex = 0;
	info.type = SMALL_PAGE;

	if (map_kernel_sections(pstart, pend, vstart, &info) != 0) {
		vmm_debug("Failed to map kernel sections");
		return (NULL );
	}

	//vmm_debug("Mapping p 0x%x to v 0x%x", pstart, vstart);

	uint32_t mapped = mapped_p_to_v(vstart, pstart);

	return ((uintptr_t) mapped);
}

mem_space mem_get_type(uintptr_t addr) {
	if (mmm_is_mem_addr(addr) != 0) {
		return (MAPABLE_MEM);
	} else if (vmm_is_kernel_code_mem_addr(addr) != 0) {
		return (KERNEL_CODE);
	} else if (vmm_is_kernel_data_mem_addr(addr) != 0) {
		return (KERNEL_DATA);
	} else if (vmm_is_kernel_heap_mem_addr(addr) != 0) {
		return (KERNEL_HEAP);
	} else if (smm_is_mem_addr(addr) != 0) {
		return (STACK_MEM);
	}
	return (USER_MEM);
}

void unmap_memory(uintptr_t addr, uint32_t size) {
	mem_space mem = mem_get_type(addr);
	uint32_t start = 0;
	uint32_t end = 0;
	uint32_t vaddr = (uint32_t) addr;
	uint32_t vend = vaddr + size;

	if (vend < vaddr) {
		return;
	}

	do {
		vmm_get_virt_block_size(p_to_v(get_kernel_table()), vaddr, &start,
				&end);

		switch (mem) {
		case MAPABLE_MEM:
			unmap_kernel_memory(vaddr);
			mmm_free_page((void*) vaddr);
			break;
		case KERNEL_CODE:
		case KERNEL_DATA:
		case KERNEL_HEAP:
			vmm_error("Invalid Memory type for unmapping KERNEL")
			;
			break;
		case STACK_MEM:
			unmap_kernel_memory(vaddr);
			smm_free_page((void*) vaddr);
			break;
		default:
			vmm_error("Invalid Memory type for unmapping USER")
			;
			break;
		}
	} while (vend < end);
}

