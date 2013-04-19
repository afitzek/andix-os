/*
 * mmu.c
 *
 *  Created on: Jan 11, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <mm/mm.h>

// ============================================================================
// Global Variables
// ============================================================================

extern uint32_t __phys_load_addr;
extern uint32_t __kernel_mb_size;

// ============================================================================

// ============================================================================
// Functions
// ============================================================================
/**
 * Map mem information into a page table
 * @param ptable The physical address of the page table
 * @param section The mem info section
 * @return 0 on success
 */
int32_t map_memory(uintptr_t ptable, kernel_mem_info_t *section) {
	return (map_memory_v(p_to_v(ptable), section));
}

/**
 * Maps a kernel mem information into kernel address space
 * @param section The mem info section
 * @return 0 on sucess
 */
int32_t map_kernel_memory(kernel_mem_info_t *section) {
	if (section->vaddr < 0x80000000) {
		vmm_error("MAP ERROR NON KERNEL ADR: 0x%x", section->vaddr);
		return (-1);
	}

	if (section->ap == AP_SVC_RW_USR_RW || section->ap == AP_SVC_RW_USR_RO
			|| section->ap == AP_SVC_RO_USR_RO) {
		vmm_debug("We don't allow any USR access to kernel memory!");
		return (-1);
	}

	return (map_memory(get_kernel_table(), section));
}

/**
 * Map memory information section into page table
 * @param vtable The virtual address of the page table
 * @param section The section information
 * @return 0 on success
 */
int32_t map_memory_v(uintptr_t vtable, kernel_mem_info_t *section) {
	uint32_t* l1_tt = vtable;
	uint32_t* l2_tt = NULL;

	uint32_t virtual_address = section->vaddr;

	uint32_t l1_index = virtual_address >> 20; // Use upper 12 bits as index

	uint32_t l2_index = (virtual_address >> 12) & 0xFF;

	//vmm_debug("Level 1 Table @ 0x%x", l1_tt);

	//vmm_debug("Mapping Index: 0x%x L1: 0x%x L2: 0x%x", virtual_address, l1_index, l2_index);

	//vmm_debug("Mapping 0x%x -> 0x%x", section->paddr, section->vaddr);

	uint32_t* l1_pte = (uint32_t*) ((uint32_t) l1_tt + l1_index * 4);

	// TODO: remove when domains are supported!
	section->domain = 0;

	if (((*l1_pte) & L1_TYPE_MASK) == 0) {
		//vmm_debug("Level 1 not mapped");
		// TODO: check later if this page is swapped!!

		// Still available
		if (section->type == SECTION) {
			(*l1_pte) = (section->paddr & 0xFFF00000) | SECTION_TYPE;
			(*l1_pte) |= ((section->nonsecure & 1) << SECTION_SHIFT_NONSECURE);
			(*l1_pte) |= ((section->shareable & 1) << SECTION_SHIFT_SHARED);
			(*l1_pte) |= (((section->ap & AP2_MASK) >> AP2_SHIFT)
					<< SECTION_SHIFT_AP2);
			(*l1_pte) |= ((section->tex & TEX_MASK) << SECTION_SHIFT_TEX);
			(*l1_pte) |= ((section->ap & AP_MASK) << SECTION_SHIFT_AP);
			(*l1_pte) |= ((section->domain & DOMAIN_MASK)
					<< SECTION_SHIFT_DOMAIN);
			(*l1_pte) |= ((section->execute & 1) << SECTION_SHIFT_NONEXEC);
			(*l1_pte) |= ((section->cacheable & 1) << SECTION_SHIFT_CACHE);
			(*l1_pte) |= ((section->bufferable & 1) << SECTION_SHIFT_BUFFER);

			//vmm_debug(
			//		"MMU: [%d] 0x%x ... 0x%x -> 0x%x ... 0x%x",
			//		l1_index, (l1_index << 20), (l1_index << 20) + 0x100000 - 1,
			//		(section->paddr & 0xFFF00000), (section->paddr & 0xFFF00000) + 0x100000 - 1);

		} else if (section->type == SMALL_PAGE || section->type == LARGE_PAGE) {
			// create new coarse page table
			l2_tt = (uint32_t*) kmalloc_align(COARSE_PAGE_SIZE, 0x1000);
			for (int i = 0; i < COARSE_PAGE_SIZE / 4; i++) {
				l2_tt[i] = 0;
			}
			//vmm_debug(
			//		"MMU: allocating L2 Table for [%d] 0x%x ... 0x%x",
			//		l1_index, (l1_index << 20), (l1_index << 20) + 0x100000 - 1);

			//setup l1 page table entry
			(*l1_pte) = ((uint32_t) v_to_p(l2_tt) & L2_ADDR_MASK)
					| PAGE_TABLE_TYPE;
			//vmm_debug("MMU: l1_pte 0x%x", (*l1_pte));

			(*l1_pte) |= ((section->domain & DOMAIN_MASK)
					<< COARSE_TABLE_SHIFT_DOMAIN);
			(*l1_pte) |= ((section->nonsecure & 1)
					<< COARSE_TABLE_SHIFT_NONSECURE);

			if (section->type == SMALL_PAGE) {

				//uint32_t* l2_pte = (uint32_t*) ((uint32_t) l2_tt + l2_index * 4);

				// setup l2 page table entry
				l2_tt[l2_index] = ((uint32_t) section->paddr & 0xFFFFF000)
						| SMALL_PAGE_TYPE;
				l2_tt[l2_index] |= ((section->shareable & 1)
						<< SMALL_PAGE_SHIFT_SHARE);
				l2_tt[l2_index] |= (((section->ap & AP2_MASK) >> AP2_SHIFT)
						<< SMALL_PAGE_SHIFT_AP2);
				l2_tt[l2_index] |= ((section->tex & TEX_MASK)
						<< SMALL_PAGE_SHIFT_TEX);
				l2_tt[l2_index] |= ((section->ap & AP_MASK)
						<< SMALL_PAGE_SHIFT_AP);
				l2_tt[l2_index] |= ((section->execute & 1)
						<< SMALL_PAGE_SHIFT_NONEXEC);
				l2_tt[l2_index] |= ((section->cacheable & 1)
						<< SMALL_PAGE_SHIFT_CACHE);
				l2_tt[l2_index] |= ((section->bufferable & 1)
						<< SMALL_PAGE_SHIFT_BUFFER);
			} else {
				uint32_t pte = 0;

				// setup l2 page table entry
				pte =
						((uint32_t) section->paddr & 0xFFFF0000)
								| LARGE_PAGE_TYPE;
				pte |= ((section->shareable & 1) << LARGE_PAGE_SHIFT_SHARE);
				pte |= (((section->ap & AP2_MASK) >> AP2_SHIFT)
						<< LARGE_PAGE_SHIFT_AP2);
				pte |= ((section->tex & TEX_MASK) << LARGE_PAGE_SHIFT_TEX);
				pte |= ((section->ap & AP_MASK) << LARGE_PAGE_SHIFT_AP);
				pte |= ((section->execute & 1) << LARGE_PAGE_SHIFT_NONEXEC);
				pte |= ((section->cacheable & 1) << LARGE_PAGE_SHIFT_CACHE);
				pte |= ((section->bufferable & 1) << LARGE_PAGE_SHIFT_BUFFER);

				for (int i = 0; i < 16; i++) {
					//uint32_t* l2_pte = (uint32_t*) ((uint32_t) l2_tt
					//		+ (l2_index + i) * 4);
					l2_tt[l2_index + i] = pte;
				}
			}

		} else {
			vmm_error("INVALID memory info type!! %d", section->type);
			// DONE
			return (-1);
		}
	} else if (((*l1_pte) & L1_TYPE_MASK) == PAGE_TABLE_TYPE) {
		l2_tt = (uint32_t*) ((*l1_pte) & L2_ADDR_MASK);
		l2_tt = p_to_v(l2_tt);
		//uint32_t* l2_pte = (uint32_t*) ((uint32_t) l2_tt + l2_index * 4);
		//vmm_debug(
		//		"Level 1 (0x%x) PT mapped -> L2 Table @ 0x%x PTE 0x%x", l1_tt, l2_tt, &l2_tt[l2_index]);
		if (section->type == SMALL_PAGE) {
			if ((l2_tt[l2_index] & L2_TYPE_MASK) == 0) {
				// TODO: check if swapped later

				l2_tt[l2_index] = ((uint32_t) section->paddr & 0xFFFFF000)
						| SMALL_PAGE_TYPE;
				l2_tt[l2_index] |= ((section->shareable & 1)
						<< SMALL_PAGE_SHIFT_SHARE);
				l2_tt[l2_index] |= (((section->ap & AP2_MASK) >> AP2_SHIFT)
						<< SMALL_PAGE_SHIFT_AP2);
				l2_tt[l2_index] |= ((section->tex & TEX_MASK)
						<< SMALL_PAGE_SHIFT_TEX);
				l2_tt[l2_index] |= ((section->ap & AP_MASK)
						<< SMALL_PAGE_SHIFT_AP);
				l2_tt[l2_index] |= ((section->execute & 1)
						<< SMALL_PAGE_SHIFT_NONEXEC);
				l2_tt[l2_index] |= ((section->cacheable & 1)
						<< SMALL_PAGE_SHIFT_CACHE);
				l2_tt[l2_index] |= ((section->bufferable & 1)
						<< SMALL_PAGE_SHIFT_BUFFER);
			} else {
				vmm_error("#### !!! L2 Already mapped !!! #### "
						"(V: 0x%x, T: 0x%x)", virtual_address,
						(l2_tt[l2_index] & L2_TYPE_MASK));
				return (-1);
			}
		} else if (section->type == LARGE_PAGE) {
			for (int i = 0; i < 16; i++) {
				if ((l2_tt[l2_index + i] & L2_TYPE_MASK) != 0) {
					vmm_error("#### !!! L2 already partial mapped !!! ####");
					return (-1);
				}
			}

			uint32_t pte = 0;

			// setup l2 page table entry
			pte = ((uint32_t) section->paddr & 0xFFFF0000) | LARGE_PAGE_TYPE;
			pte |= ((section->shareable & 1) << LARGE_PAGE_SHIFT_SHARE);
			pte |= (((section->ap & AP2_MASK) >> AP2_SHIFT)
					<< LARGE_PAGE_SHIFT_AP2);
			pte |= ((section->tex & TEX_MASK) << LARGE_PAGE_SHIFT_TEX);
			pte |= ((section->ap & AP_MASK) << LARGE_PAGE_SHIFT_AP);
			pte |= ((section->execute & 1) << LARGE_PAGE_SHIFT_NONEXEC);
			pte |= ((section->cacheable & 1) << LARGE_PAGE_SHIFT_CACHE);
			pte |= ((section->bufferable & 1) << LARGE_PAGE_SHIFT_BUFFER);

			for (int i = 0; i < 16; i++) {
				l2_tt[l2_index + i] = pte;
			}
		} else {
			vmm_error(
					"TRYING TO MAP SECTION OR SUPERSECTION INTO COARSE TABLE!!");
			// DONE
			return (-1);
		}
	} else {
		vmm_error("#### !!! L1 Already mapped !!! ####");
		return (-1);
	}
	CP15DSB;

	return (0);
}

void dump_kernel_mmu(uint32_t vstart, uint32_t vend) {
	uint32_t* l1_tt = p_to_v(get_kernel_table());
	dump_mmu(vstart, vend, l1_tt);
}

/**
 * Dump MMU from virtual start to virtual end
 * @param vstart The virtual start address
 * @param vend The virutal end address
 */
void dump_mmu(uint32_t vstart, uint32_t vend, uint32_t* l1_tt) {
	uint32_t idx = vstart >> 20;
	uint32_t idx_end = vend >> 20;
	uint32_t l2idx = 0;

	if (vend <= vstart) {
		return;
	}
	//uint32_t* l1_tt = p_to_v(get_kernel_table());

	vmm_debug("Dumping Table @ 0x%x", l1_tt);

	vmm_debug("  V START  ...    V END   ->   P START  ...     P END  ");

	for (; idx <= idx_end; idx++) {
		uint32_t phys = l1_tt[idx] & 0xFFF00000;
		uint32_t virt = idx << 20;
		if ((l1_tt[idx] & 0x3) != 0) {
			if ((l1_tt[idx] & 0x3) == 2) {
				vmm_debug("[%d] 0x%x ... 0x%x -> 0x%x ... 0x%x [%d]", idx, virt,
						virt + 0x100000 - 1, phys, phys + 0x100000 - 1,
						l1_tt[idx] & 0x3);
			} else {
				uint32_t* l2_tt = (uint32_t*) (l1_tt[idx] & L2_ADDR_MASK);
				l2_tt = p_to_v(l2_tt);
				vmm_debug("[%d] 0x%x ... 0x%x -> 0x%x ... 0x%x [%d]", idx, virt,
						virt + 0x100000 - 1, phys, phys + 0x100000 - 1,
						l1_tt[idx] & 0x3);
				vmm_debug("PageTable @ 0x%x", l2_tt);
				for (l2idx = 0; l2idx < COARSE_PAGE_SIZE / 4; l2idx++) {
					phys = l2_tt[l2idx] & L2_ADDR_MASK;
					uint32_t virt2 = (virt + l2idx * SMALL_PAGE_SIZE);
					if (virt2 >= vstart && virt2 <= vend) {
						if ((l2_tt[l2idx] & L2_TYPE_MASK) == SMALL_PAGE_TYPE
								|| (l2_tt[l2idx] & L2_TYPE_MASK)
										== SMALL_PAGE_TYPE + 1) {
							vmm_debug(
									" -- [%d] 0x%x ... 0x%x -> 0x%x ... 0x%x [%d] @ "
											"0x%x", l2idx, virt2,
									virt2 + SMALL_PAGE_SIZE - 1, phys,
									phys + SMALL_PAGE_SIZE - 1,
									l2_tt[l2idx] & L2_TYPE_MASK, &l2_tt[l2idx]);
						} else if ((l2_tt[l2idx] & L2_TYPE_MASK)
								== LARGE_PAGE_TYPE) {
							vmm_debug(
									" -- [%d] 0x%x ... 0x%x -> 0x%x ... 0x%x [%d] @ "
											"0x%x", l2idx, virt2,
									virt2 + LARGE_PAGE_SIZE - 1, phys,
									phys + LARGE_PAGE_SIZE - 1,
									l2_tt[l2idx] & L2_TYPE_MASK, &l2_tt[l2idx]);
						}
					}
				}
			}
		}
	}

}

/**
 * Dump kernel MMU content
 */
void dump_mmu_full() {
	dump_kernel_mmu(0x80000000, 0xFFFFFFFF);
}

/**
 * Map kernel memory from physical start to physical end, starting from
 * virtual start with section attributes
 * @param start The physical start address
 * @param end The physical end address
 * @param vstart The virtual start address
 * @param section The mem info
 * @return 0 on success
 */
int32_t map_kernel_sections(uint32_t start, uint32_t end, uint32_t vstart,
		kernel_mem_info_t *section) {
	uint32_t psize = 0;
	uint32_t fvstart = vstart;
	uint32_t fvend = 0;
	uint8_t failed = 0;

	vmm_debug("Mapping Kernel Memory: 0x%x ... 0x%x -> 0x%x ... 0x%x",
			start, end, vstart, vstart + (end - start));

	if (section->type == LARGE_PAGE) {
		psize = LARGE_PAGE_SIZE;
	} else if (section->type == SMALL_PAGE) {
		psize = SMALL_PAGE_SIZE;
	} else {
		psize = SECTION_SIZE;
	}

	while (start <= end) {
		section->paddr = start;
		section->vaddr = vstart;

		if (map_kernel_memory(section) != 0) {
			failed = 1;
			fvend = vstart - 1;
			break;
		}

		start += psize;
		vstart += psize;
	}

	if (failed == 1) {
		// On failure unmap ...
		while (fvstart <= fvend) {
			unmap_kernel_memory(fvstart);
			fvstart += psize;
		}
		return (-1);
	}
	return (0);
}

/**
 * Unmap the kernel memory 1:1 mapping from boot code
 */
void unmap_virtual_phys_kern() {
	uint32_t* l1_tt = p_to_v(get_kernel_table());
	uint32_t virtual_address = __phys_load_addr;
	uint32_t l1_index = virtual_address >> 20; // Use upper 12 bits as index
	uint32_t l1_index_end = l1_index + __kernel_mb_size;
	for (; l1_index < l1_index_end; l1_index++) {
		l1_tt[l1_index] = 0x0;
	}
}

/**
 * Gets virtual memory block size of virtual addr
 * @param vtable The page table to query (virtual address)
 * @param addr The virtual address
 * @param start The start of the memory entry in vtable (out)
 * @param end The end of the memory entry in vtable (out)
 */
void vmm_get_virt_block_size(uintptr_t vtable, uint32_t addr, uintptr_t start,
		uintptr_t end) {
	uint32_t* l1_tt = vtable;
	uint32_t* l2_tt = NULL;

	uint32_t virtual_address = addr;
	uint32_t l1_index = virtual_address >> 20; // Use upper 12 bits as index
	uint32_t l2_index = (virtual_address >> 12) & 0xFF;
	uint32_t* l1_pte = (uint32_t*) ((uint32_t) l1_tt + l1_index * 4);

	if (start == NULL || end == NULL ) {
		return;
	}

	if (((*l1_pte) & L1_TYPE_MASK) == 0) {
		(*start) = 0;
		(*end) = 0;
	} else if (((*l1_pte) & L1_TYPE_MASK) == PAGE_TABLE_TYPE) {
		l2_tt = (uint32_t*) ((*l1_pte) & L2_ADDR_MASK);
		l2_tt = p_to_v(l2_tt);
		if ((l2_tt[l2_index] & L2_TYPE_MASK) == 0) {
			(*start) = 0;
			(*end) = 0;
		} else if ((l2_tt[l2_index] & L2_TYPE_MASK) == SMALL_PAGE_TYPE) {
			(*start) = addr & SMALL_PAGE_MASK;
			(*end) = (*start) + SMALL_PAGE_SIZE - 1;
		} else if ((l2_tt[l2_index] & L2_TYPE_MASK) == LARGE_PAGE_TYPE) {
			(*start) = addr & SMALL_PAGE_MASK;
			(*end) = (*start) + LARGE_PAGE_SIZE - 1;
		}
	} else {
		(*start) = addr & SECTION_MASK;
		(*end) = (*start) + SECTION_SIZE - 1;
	}
}

// ============================================================================

void unmap_kernel_memory(uint32_t vaddr) {
	uint32_t* l1_tt = p_to_v(get_kernel_table());
	uint32_t* l2_tt = NULL;

	uint32_t l1_index = vaddr >> 20; // Use upper 12 bits as index
	uint32_t l2_index = (vaddr >> 12) & 0xFF;

	uint32_t* l1_pte = (uint32_t*) ((uint32_t) l1_tt + l1_index * 4);

	if (((*l1_pte) & L1_TYPE_MASK) == 0) {
		// OK we are done ...
		return;
	} else if (((*l1_pte) & L1_TYPE_MASK) == PAGE_TABLE_TYPE) {
		l2_tt = (uint32_t*) ((*l1_pte) & L2_ADDR_MASK);
		l2_tt = p_to_v(l2_tt);
		if ((l2_tt[l2_index] & L2_TYPE_MASK) == 0) {
			// Not mapped yet we are done ...
			return;
		} else {
			l2_tt[l2_index] = 0;
		}
	} else {
		(*l1_pte) = 0; // Unmap whole section ...
	}

}

