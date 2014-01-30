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
 * loader.c
 *
 *  Created on: Nov 10, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <loader.h>
#include <mm/mm.h>

uint32_t setup_elf(uint8_t* payload, uint32_t size, struct user_process_t *process) {
	Elf32_Ehdr *hdr;
	Elf32_Phdr *phdr;
	//Elf32_Shdr *shdr;

	uint32_t vptd = process->vuserPD;
	uint32_t *heapstart = &process->vheap;
	*heapstart = 0;

	hdr = (Elf32_Ehdr*) payload;

	loader_debug("Starting at hdr=%p, size=%d", hdr, size);

	if (hdr->e_ident[EI_MAG0] != ELFMAG0 || hdr->e_ident[EI_MAG1] != ELFMAG1
			|| hdr->e_ident[EI_MAG2] != ELFMAG2
			|| hdr->e_ident[EI_MAG3] != ELFMAG3) {
		loader_debug("NOT a valid ELF buffer");
		return (0xFFFFFFFF);
	}

	if (hdr->e_type != ET_EXEC) {
		loader_debug("Invalid ELF Type");
		return (0xFFFFFFFF);
	}

	if (hdr->e_machine != EM_ARM) {
		loader_debug("Invalid ELF machine");
		return (0xFFFFFFFF);
	}

	if (hdr->e_version != EV_CURRENT) {
		loader_debug("Invalid ELF version");
		return (0xFFFFFFFF);
	}

	loader_debug("String Header IDX: %d", hdr->e_shstrndx);

	Elf32_Shdr* stringHdr = (Elf32_Shdr*) (payload + hdr->e_shoff
			+ sizeof(Elf32_Shdr) * hdr->e_shstrndx);

	loader_debug("String Table Offset: %d", stringHdr->sh_offset);

	char* stringTable = (char*) (payload + stringHdr->sh_offset);

	loader_debug("String Table @ 0x%x", stringTable);

	uint16_t idx = 0;
	loader_debug("PROGRAM HEADERS: ");

	for (idx = 0; idx < hdr->e_phnum; idx++) {
		phdr = (Elf32_Phdr*)(payload + hdr->e_phoff + sizeof(Elf32_Phdr) * idx);
		if ((uint32_t)phdr > ((uint32_t)payload + size)) {
			loader_debug("Program header outside of elf data");
			return (0xFFFFFFFF);
		}

		if (phdr->p_type == PT_LOAD) {
			loader_debug("Loading Program header");
			uintptr_t mfile = (uintptr_t)(payload + phdr->p_offset);
			loader_debug(
					"Load from 0x%x [0x%x bytes] -> v 0x0%x [0x%x bytes] %x", mfile, phdr->p_filesz, phdr->p_vaddr, phdr->p_memsz, phdr->p_flags);

			uint32_t size = 0;
			uintptr_t mem = 0;
			uint8_t ap = 0;
			kernel_mem_info_t info;
			info.bufferable = 0;
			info.cacheable = 0;
			info.nonsecure = 0;
			info.shareable = 0;
			info.tex = 0;
			info.type = SMALL_PAGE;
			info.execute = (phdr->p_flags & PF_X) ? (EXEC_OK) : (EXEC_NON);

			if ((phdr->p_flags & PF_W)&& (phdr->p_flags & PF_R)){
				ap = AP_SVC_RW_USR_RW;
			} else if(phdr->p_flags & PF_R) {
				ap = AP_SVC_RW_USR_RO;
			}



			loader_debug(
					"Mem info build (MEMSZ %d LOADSZ %d)", phdr->p_memsz, phdr->p_filesz);
			mem = (uintptr_t)phdr->p_vaddr;
			uint32_t start_page = ((uint32_t) mem & 0xFFFFF000) >> 12;
			uint32_t end_page = (((uint32_t) mem + phdr->p_memsz) & 0xFFFFF000)
					>> 12;
			if (((uint32_t) mem + phdr->p_memsz)
					!= (((uint32_t) mem + phdr->p_memsz) & 0xFFFFF000)) {
				end_page += 1;
			}
			uint32_t pgcount = end_page - start_page; //phdr->p_memsz / SMALL_PAGE_SIZE;
			pgcount++;
			loader_debug(
					"Needing %d %x-%x pages", pgcount, end_page, start_page);

			void* ppage = pmm_allocate_pages(pgcount);

			loader_debug("Mapping space to 0x%x", mem);
			size = pgcount * SMALL_PAGE_SIZE;

			for (uint32_t i = 0; i < pgcount; i++) {

				info.vaddr = phdr->p_vaddr + SMALL_PAGE_SIZE * i;
				info.paddr = (uint32_t) ppage + SMALL_PAGE_SIZE * i;
				loader_debug(
						"Mapping memory 0x%x -> 0x%x", info.paddr, info.vaddr);
				info.ap = ap;
				map_memory_v(vptd, &info);

				uint32_t vendaddr = phdr->p_vaddr + SMALL_PAGE_SIZE * (i + 1);

				if(vendaddr > (*heapstart)) {
					(*heapstart) = vendaddr & SMALL_PAGE_MASK;
				}
			}
			loader_debug("Setting memory to zero %d bytes", size);

			// set mem to 0
			uint32_t mi = 0;
			uintptr_t pmem = (uintptr_t)((uint32_t) mem & 0xFFFFF000);
			for (mi = 0; mi < size / 4; mi++) {
				pmem[mi] = 0;
			}

			loader_debug(
					"Filling memory 0x%x - 0x%x [%d]", mem, mem + phdr->p_filesz / 4, phdr->p_filesz);

			for (mi = 0; mi < phdr->p_filesz / 4; mi++) {
				mem[mi] = mfile[mi];
			}
			//loader_debug("Loaded Mempry:");
			//kdumpMem(mem, phdr->p_memsz);
			loader_debug("Loading Program header loaded!");
		}
		if (phdr->p_type == PT_TLS) {
			loader_debug("Found TLS v 0x%x [0x%x bytes]", phdr->p_vaddr, phdr->p_memsz);
			process->tls_template.start = phdr->p_vaddr;
			process->tls_template.size = phdr->p_memsz;
		}
	}

	uint32_t entry = hdr->e_entry;

	loader_info("USER HEAP START: 0x%x", (*heapstart));

	loader_debug("Payload OK!");

	dump_mmu(0x0, 0x20000, vptd);

	loader_debug("Entry point: 0x%x", entry);
	return (entry);
}
