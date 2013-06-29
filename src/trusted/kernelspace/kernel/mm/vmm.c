/*
 * vmm.c
 *
 *  Created on: Sep 7, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/mm.h>

// ============================================================================
// Parameters
// ============================================================================
extern uint32_t _end;
extern uint32_t _code;
extern uint32_t __phys_load_addr;
extern uint32_t __phys_target_addr;
extern uint32_t __virt_load_addr;

uint8_t heap_init = 0;
void* heap_end;
k_mem_block_t* heap_start;
uintptr_t phys_heap_start;
uintptr_t phys_heap_end;

uint32_t vhstart;
uint32_t vhend;
uint32_t phstart;
uint32_t phend;

// ============================================================================
// Functions
// ============================================================================

/**
 * Maps the atags from given physical address to a virtual address
 * @param phys the physical address of atags
 * @return the virtual address
 */
uint32_t map_atags(uintptr_t phys) {
	kernel_mem_info_t section_description;

	// Setup Mappings
	section_description.domain = 0;
	section_description.ap = AP_SVC_RW_USR_NO;
	section_description.execute = EXEC_NON;
	section_description.tex = 0x0;
	section_description.cacheable = 1;
	section_description.bufferable = 1;
	section_description.nonsecure = 0;
	section_description.shareable = 0;
	section_description.type = SECTION;
	section_description.paddr = (uint32_t) phys;
	//section_description.vaddr = (uint32_t)0xC0000000;
	section_description.vaddr = (((uint32_t) &_code & 0xFFF00000) - 0x100000);

	vmm_debug("Mapping ATAGS @ 0x%x", section_description.vaddr);

	if (map_kernel_memory(&section_description) != 0) {
		return (virt_to_phys(phys));
	}

	return (section_description.vaddr + ((uint32_t) phys & 0xFFFFF));
}

/**
 * Initialize virtual memory management
 * @param startTag The atags
 */
void init_vmm(struct atag* startTag) {

	uintptr_t phys_pmm_start = NULL;
	uintptr_t phys_pmm_end = NULL;

	uintptr_t virt_mmm_start = (uintptr_t) 0x80000000;
	uintptr_t virt_mmm_end = (uintptr_t) 0xC0000000;

	uintptr_t virt_smm_start = NULL;
	uintptr_t virt_smm_end = (uintptr_t) 0xFFFFFFFF;

	struct atag* mem = NULL;
	uint32_t* ttbr1 = p_to_v(get_kernel_table());
	//uint32_t* ttbr0 = p_to_v(get_user_table());

	//vmm_debug("TTBR0 Table @ 0x%x TTBR1 @ v 0x%x", ttbr0, ttbr1);
	//vmm_debug("TTBR1 Table @ v 0x%x p 0x%x", ttbr1, v_to_p(ttbr1));

	vmm_debug("initializing kernel heap ...");

	init_heap();

	virt_smm_start = (uintptr_t) (vhend + 1);

	vmm_debug("initializing kernel heap [OK]");

	//dump_heap();

	uint32_t ptr = (uint32_t) v_to_p(&_code);

	vmm_debug("v. ptr ... 0x%x", ptr);
	mem = startTag;
	do {
		mem = atag_get_next_mem_tag(mem);
		//vmm_debug("checkin .. ");
		if (mem != NULL ) {
			uint32_t start = mem->u.mem.start;
			uint32_t end = (uint32_t) (start + mem->u.mem.size);

			vmm_debug("Start 0x%x End: 0x%x", start, end);

			if (ptr > start && ptr < end) {
				// Thats our memory segment
				uint32_t index = (uint32_t) heap_end >> 20;
				uint32_t index_end = end >> 20;
				index++;
				// Unmap ...
				vmm_debug("cleaning from idx: 0x%x -> 0x%x", index, index_end);
				for (; index < index_end; index++) {
					uint32_t* l1_pte =
							(uint32_t*) ((uint32_t) ttbr1 + index * 4);
					(*l1_pte) = 0x0;
				}

				phys_pmm_start = phys_heap_end;
				if (mem->u.mem.size > SECURE_MEMORY_SIZE) {
					end = (uint32_t) (start + SECURE_MEMORY_SIZE);
				} // otherwise we hope it is enough
				phys_pmm_end = (uintptr_t) end;

				vmm_debug("PMM ... ");
				pmm_init((uint32_t) phys_pmm_start, (uint32_t) phys_pmm_end);

				mem = NULL;
			}
		}
	} while (mem != NULL );

	mem = startTag;
	do {
		mem = atag_get_next_mem_tag(mem);
		if (mem != NULL ) {
			uint32_t start = mem->u.mem.start;
			uint32_t end = (uint32_t) (start + mem->u.mem.size);

			if (ptr > start && ptr < end) {
				if (mem->u.mem.size > SECURE_MEMORY_SIZE) {
					//uint32_t lsize = mem->u.mem.size - SECURE_MEMORY_SIZE;
					end = (uint32_t) (start + SECURE_MEMORY_SIZE - 1);
					pmm_add_phys_mem_area(start + SECURE_MEMORY_SIZE,
							start + mem->u.mem.size, MEM_TYPE_UNSECURE);
					pmm_add_phys_mem_area(start, end, MEM_TYPE_SECURE);
				} else {
					pmm_add_phys_mem_area(start, end, MEM_TYPE_SECURE);
				}

			} else {
				pmm_add_phys_mem_area(start, end, MEM_TYPE_UNSECURE);
			}
		}
	} while (mem != NULL );
	vmm_debug("MMM ... ");
	mmm_init((uint32_t) virt_mmm_start, (uint32_t) virt_mmm_end);

	smm_init((uint32_t) virt_smm_start, (uint32_t) virt_smm_end);

	vmm_debug("=========================================");
	vmm_debug("Memory overview:");
	vmm_debug("-----------------------------------");
	vmm_debug("Kernel HEAP:");
	vmm_debug("virtual : 0x%x ... 0x%x", vhstart, vhend);
	vmm_debug("physical: 0x%x ... 0x%x", phstart, phend);
	vmm_debug("-----------------------------------");
	vmm_debug("Physical Pages available:");
	vmm_debug("physical: 0x%x ... 0x%x", phys_pmm_start, phys_pmm_end);
	vmm_debug("-----------------------------------");
	vmm_debug("Virtual Memory:");
	vmm_debug("virtual : 0x%x ... 0x%x", virt_mmm_start, virt_mmm_end);
	vmm_debug("-----------------------------------");
	vmm_debug("Stack Memory:");
	vmm_debug("virtual : 0x%x ... 0x%x", virt_smm_start, virt_smm_end);
	vmm_debug("-----------------------------------");
	vmm_debug("=========================================");
}

/**
 * Gets TTBR0 (The userspace page table)
 * @return The physical page table
 */
uint32_t* get_user_table() {
	uint32_t mmu_user_table = 0x0;

	__asm__ __volatile__("MRC p15, 0, %0, c2, c0, 0": "=r" (mmu_user_table)::"memory");

	return ((uint32_t*) mmu_user_table);
}

/**
 * Gets TTBR1 (The kernel page table)
 * @return The physical page table
 */
uint32_t* get_kernel_table() {
	uint32_t mmu_user_table = 0x0;

	__asm__ __volatile__("MRC p15, 0, %0, c2, c0, 1": "=r" (mmu_user_table)::"memory");

	return ((uint32_t*) mmu_user_table);
}

/**
 * Sets the user table (TTBR0)
 * @param table The page table
 */
void set_user_table(uint32_t* table) {
	__asm__ __volatile__("MCR p15, 0, %0, c2, c0, 0":: "r" (table):"memory", "cc");
}

/**
 * Sets the kernel table (TTBR1)
 * @param table The page table
 */
void set_kernel_table(uint32_t* table) {
	__asm__ __volatile__("MCR p15, 0, %0, c2, c0, 1":: "r" (table):"memory", "cc");
}

/**
 * Translate virtual to a physical address
 * @param ptr The virtual address to translate
 * @return The physical address < 0xFFF on failure
 */
void* v_to_p(void* ptr) {
	uint32_t addr;
	//vmm_debug("Translating v (0x%x)", ptr);
	__asm__ __volatile__(
			"MCR p15, 0, %1, c7, c8, 1\n"
			"MRC p15, 0, %0, c7, c4, 0":
			"=r" (addr): "r" (ptr):"memory", "cc");

	if (addr < 0xFF) {
		vmm_debug("Failed to translate v (0x%x) to p", ptr);
	}

	addr = (addr & 0xFFFFF000) + ((uint32_t) ptr & 0xFFF);

	return ((void*) addr);
}

/**
 * Kernel Virtual offset
 * @return the offset between kernel virtual and physical space
 */
uint32_t getKernelVirtualOffset() {
	return (__virt_load_addr - __phys_target_addr);
}

/**
 * Calculate the needed pages for pagesize 4K
 * @param ptr The data ptr
 * @param size The data size
 * @return The page count 0 on error
 */
uint32_t needed_pages(uint8_t* ptr, uint32_t size) {
	uint32_t start = (uint32_t) ptr;
	uint32_t end = start + size;
	uint32_t start_page = (start & 0xFFFFF000) >> 12;
	uint32_t end_page = (end & 0xFFFFF000) >> 12;
	end_page++;
	if (end < start) {
		vmm_error("Failed to calculate needed pages memory overrun!");
		return (0);
	}

	return (end_page - start_page);
}

/**
 * Translate physical to virtual address
 *
 * !THIS ONLY WORKS FOR STATIC KERNEL SPACE!!
 * @param ptr The physical address
 * @return The virtual address
 */
void* p_to_v(void* ptr) {
	return ((void*) ((uint32_t) ptr + getKernelVirtualOffset()));
}

// ============================================================================

void controlSet(uint32_t value, uint32_t mask) {
	uint32_t c1format;
	asm volatile("MRC p15, 0, %0, c1, c0, 0": "=r" ((c1format)) :: "memory");
	/* read control register */
	c1format &= ~mask;
	/* clear bits that change */
	c1format |= value;
	/* set bits that change */
	asm volatile("MCR p15, 0, %0, c1, c0, 0":: "r" ((c1format)) : "memory", "cc");
	/* write control register */
	asm volatile("DSB\nISB\n");
}

void flushTLB(void) {
	uint32_t reg = 0;
	asm volatile("MCR     p15, 0, %0, c8, c3,  0\n"
			"MCR     p15, 0, %0, c7, c5,  6\n"
			"DSB\n"
			"ISB\n":: "r" ((reg)) : "memory", "cc");
}

void clean_user() {
	uint32_t* ttbr0 = p_to_v(get_user_table());
	vmm_debug("Cleaning TTBR0 ...");

	for (int i = 0; i < 2048; i++) {
		ttbr0[i] = 0;
	}

	vmm_debug("Cleaning TTBR0 [DONE]");
}

void init_heap() {

	kernel_mem_info_t section_description;
	vhstart = ((uint32_t) &_end + 0x00100000) & 0xFFF00000; // vaddr
	vhend = vhstart + KERNEL_HEAP_SIZE - 1; // vaddr

	phstart = vhstart - getKernelVirtualOffset(); // paddr
	phend = vhend - getKernelVirtualOffset(); // paddr

	vmm_debug("End @ 0x%x", &_end);
	vmm_debug("HEAP from 0x%x .. 0x%x -> 0x%x .. 0x%x", phstart, phend, vhstart,
			vhend);

	heap_end = (void*) vhend;
	heap_start = (k_mem_block_t*) vhstart;

	phys_heap_end = (uintptr_t) (phend + 1);
	phys_heap_start = (uintptr_t) phstart;

	// Setup Mappings
	section_description.domain = 0;
	section_description.ap = AP_SVC_RW_USR_NO;
	section_description.execute = EXEC_NON;
	section_description.tex = 0x0;
	section_description.cacheable = 1;
	section_description.bufferable = 1;
	section_description.nonsecure = 0;
	section_description.shareable = 0;
	section_description.type = SECTION;
	section_description.paddr = phstart;
	section_description.vaddr = vhstart;

	//vmm_debug("Mapping ATAGS @ 0x%x", section_description.vaddr);
	map_kernel_sections(phstart, phend, vhstart, &section_description);

	heap_start->flags = 0;
	heap_start->prev_size = 0;
	heap_start->size = KERNEL_HEAP_SIZE - sizeof(k_mem_block_t);

	heap_init = 1;
}

void dump_heap() {
	k_mem_block_t* block = heap_start;

	uint32_t hsize = (uint32_t) heap_end - (uint32_t) heap_start;

	uint32_t hfree = hsize;
	uint32_t halloc = 0;

	hfree = 0;

	vmm_debug("===============================");
	vmm_debug("HEAP DUMP:");
	vmm_debug("LOCA  : 0x%x -> 0x%x", heap_start, heap_end);
	vmm_debug("BLOCKS: -----------------------");

	while ((void*) block < heap_end) {
		vmm_debug("[%c] 0x%x ... 0x%x BLK @ 0x%x (%d B, %d KB, %d MB)",
				M_IS_FREE(block) ? 'F' : 'U',
				(uint32_t)block + sizeof(k_mem_block_t),
				(uint32_t)block + sizeof(k_mem_block_t) + block->size, block,
				block->size, block->size / 1024, block->size / (1024 * 1024));

		if (M_IS_FREE(block)) {
			hfree += block->size;
		} else {
			halloc += block->size;
		}
		block = (k_mem_block_t*) ((uint8_t*) block + sizeof(k_mem_block_t)
				+ block->size);
	}
	vmm_debug("-------------------------------");
	vmm_debug("SIZE  : 0x%x (%d B, %d KB, %d MB)", hsize, hsize, hsize / 1024,
			hsize / (1024 * 1024));
	vmm_debug("FREE  : 0x%x (%d B, %d KB, %d MB)", hfree, hfree, hfree / 1024,
			hfree / (1024 * 1024));
	vmm_debug("USED  : 0x%x (%d B, %d KB, %d MB)", halloc, halloc,
			halloc / 1024, halloc / (1024 * 1024));
	vmm_debug("===============================");
}

void prepareBestBlock(k_mem_block_t* best, uint32_t size) {
	k_mem_block_t* next;
	k_mem_block_t* new;
	// Now we have a best free block!
	if (best->size > size + sizeof(k_mem_block_t)) {
		// need to split block!
		// split the block
		next = (k_mem_block_t*) ((uint8_t*) best + best->size);
		new = (k_mem_block_t*) ((uint8_t*) best + size + sizeof(k_mem_block_t));
		new->prev_size = size;
		new->flags = 0;
		new->size = best->size - sizeof(k_mem_block_t) - size;
		if ((void*) next < heap_end) {
			next->prev_size = new->size;
		}
		best->size = size;
	}
	best->flags = 1;
}

uint32_t check_align(uint32_t align) {
	if (align <= (1 << 1)) {
		return (1 << 1);
	} else if (align <= (1 << 2)) {
		return (1 << 2);
	} else if (align <= (1 << 3)) {
		return (1 << 3);
	} else if (align <= (1 << 4)) {
		return (1 << 4);
	} else if (align <= (1 << 5)) {
		return (1 << 5);
	} else if (align <= (1 << 6)) {
		return (1 << 6);
	} else if (align <= (1 << 7)) {
		return (1 << 7);
	} else if (align <= (1 << 8)) {
		return (1 << 8);
	} else if (align <= (1 << 9)) {
		return (1 << 9);
	} else if (align <= (1 << 10)) {
		return (1 << 10);
	} else if (align <= (1 << 11)) {
		return (1 << 11);
	} else if (align <= (1 << 12)) {
		return (1 << 12);
	} else if (align <= (1 << 13)) {
		return (1 << 13);
	} else if (align <= (1 << 14)) {
		return (1 << 14);
	} else if (align <= (1 << 15)) {
		return (1 << 15);
	} else if (align <= (1 << 16)) {
		return (1 << 16);
	} else if (align <= (1 << 17)) {
		return (1 << 17);
	} else if (align <= (1 << 18)) {
		return (1 << 18);
	} else if (align <= (1 << 19)) {
		return (1 << 19);
	} else if (align <= (1 << 20)) {
		return (1 << 20);
	} else if (align <= (1 << 21)) {
		return (1 << 21);
	} else if (align <= (1 << 22)) {
		return (1 << 22);
	} else if (align <= (1 << 23)) {
		return (1 << 23);
	} else if (align <= (1 << 24)) {
		return (1 << 24);
	} else if (align <= (1 << 25)) {
		return (1 << 25);
	} else if (align <= (1 << 26)) {
		return (1 << 26);
	} else if (align <= (1 << 27)) {
		return (1 << 27);
	} else if (align <= (1 << 28)) {
		return (1 << 28);
	} else if (align <= (1 << 29)) {
		return (1 << 29);
	} else if (align <= (1 << 30)) {
		return (1 << 30);
	} else if (align <= (1 << 31)) {
		return (1 << 31);
	}
	return (1);
}

extern uint32_t _code;
extern uint32_t _code_end;
extern uint32_t _rodata;
extern uint32_t _bss_end;

uint32_t vmm_get_kernel_code_start_addr() {
	return ((uint32_t) &_code);
}

uint32_t vmm_get_kernel_code_end_addr() {
	return ((uint32_t) &_code_end);
}

uint8_t vmm_is_kernel_code_mem_addr(uintptr_t vaddr) {
	uint32_t addr = (uint32_t) vaddr;

	if (addr >= vmm_get_kernel_code_start_addr()
			&& addr < vmm_get_kernel_code_end_addr()) {
		return (1);
	}
	return (0);
}

uint32_t vmm_get_kernel_data_start_addr() {
	return ((uint32_t) &_rodata);
}

uint32_t vmm_get_kernel_data_end_addr() {
	return ((uint32_t) &_bss_end);
}

uint8_t vmm_is_kernel_data_mem_addr(uintptr_t vaddr) {
	uint32_t addr = (uint32_t) vaddr;

	if (addr >= vmm_get_kernel_data_start_addr()
			&& addr < vmm_get_kernel_data_end_addr()) {
		return (1);
	}
	return (0);
}

uint32_t vmm_get_kernel_heap_start_addr() {
	return (vhstart);
}

uint32_t vmm_get_kernel_heap_end_addr() {
	return (vhend);
}

uint8_t vmm_is_kernel_heap_mem_addr(uintptr_t vaddr) {
	uint32_t addr = (uint32_t) vaddr;

	if (addr >= vmm_get_kernel_heap_start_addr()
			&& addr < vmm_get_kernel_heap_end_addr()) {
		return (1);
	}
	return (0);
}

