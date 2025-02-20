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
 * tee_memory.c
 *
 *  Created on: Apr 3, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee/tee_memregion.h>
#include <kprintf.h>
#include <mm/mm.h>
#include <devices/random/random.h>

list* tee_memory_list = NULL;

list* tee_temp_memory_list = NULL;

void tee_memory_initialize() {
	tee_memory_list = (list*) kmalloc(sizeof(list));
	tee_memory_list->next = tee_memory_list;
	tee_memory_list->prev = tee_memory_list;
	tee_memory_list->data = NULL;

	tee_temp_memory_list = (list*) kmalloc(sizeof(list));
	tee_temp_memory_list->next = tee_temp_memory_list;
	tee_temp_memory_list->prev = tee_temp_memory_list;
	tee_temp_memory_list->data = NULL;
}

tee_temp_memory* tee_tmp_mem_create(void) {
	tee_temp_memory* memory = (tee_temp_memory*) kmalloc(
			sizeof(tee_temp_memory));
	if (!memory) {
		tee_error("Failed to create memory (out of memory)");
		return (NULL );
	}

	uint32_t id = 0;
	tee_memory* test = NULL;

	do {
		id = random_next32();
		test = tee_memory_find(id);
	} while (test != NULL );

	tee_info("New TEE memory with ID %x", id);

	memory->_id = id;
	//memory->context = context;

	if (list_add(tee_temp_memory_list, (uintptr_t) memory) != 0) {
		tee_error("Failed to create memory (out of memory)");
		kfree((uintptr_t) memory);
		return (NULL );
	}

	return (memory);
}

tee_temp_memory* tee_tmp_find_by_vaddr_and_task(uint32_t vaddr, tid_t tid) {
	list* pos;
	list* next;
	tee_temp_memory* session = NULL;
	tee_temp_memory* session_res = NULL;

	tee_debug("Searching TEE memory @ PADDR-> %x", paddr);
	list_for_each_safe(pos, next, tee_temp_memory_list)
	{
		if (pos->data != NULL ) {
			session = (tee_temp_memory*) pos->data;
			if (session->uaddr == vaddr && session->task_id == tid) {
				session_res = session;
				break;
			}
		}
	}
	return (session_res);
}

void tee_tmp_mem_free(tee_temp_memory* mem) {
	if (mem == NULL ) {
		return;
	}

	list* entry = list_find_data(tee_temp_memory_list, (uintptr_t) mem);
	if (entry != NULL ) {
		list_remove(entry);
	}

	kfree(mem);
}

tee_memory* tee_memory_create(tee_context* context) {

	if (context == NULL ) {
		tee_error("Failed to create memory context is NULL");
		return (NULL );
	}

	tee_memory* memory = (tee_memory*) kmalloc(sizeof(tee_memory));
	if (!memory) {
		tee_error("Failed to create memory (out of memory)");
		return (NULL );
	}

	memory->mappedtasks = (list*) kmalloc(sizeof(list));
	if (memory->mappedtasks == NULL ) {
		kfree((uintptr_t) memory);
		tee_error("Failed to create memory (out of memory)");
		return (NULL );
	}
	memory->mappedtasks->next = memory->mappedtasks;
	memory->mappedtasks->prev = memory->mappedtasks;
	memory->mappedtasks->data = NULL;

	uint32_t id = 0;
	tee_memory* test = NULL;

	do {
		id = random_next32();
		test = tee_memory_find(id);
	} while (test != NULL );

	tee_info("New TEE memory with ID %x", id);

	memory->_id = id;

	memory->context = context;

	if (list_add(tee_memory_list, (uintptr_t) memory) != 0) {
		tee_error("Failed to create memory (out of memory)");
		kfree((uintptr_t) memory);
		return (NULL );
	}

	return (memory);
}

void tee_memory_destroy(tee_memory* memory) {
	if (memory == NULL ) {
		return;
	}

	// TODO: cleanup memory
	// check if we already have a mapping...
	list* pos;
	list* next;
	tee_mapped_mem* mapped;
	list_for_each_safe(pos, next, memory->mappedtasks)
	{
		if (pos->data != NULL ) {
			mapped = (tee_mapped_mem*) pos->data;
			unmap_mem_from_task((uint8_t*) mapped->vaddr, memory->size,
					mapped->proc);
			kfree((uintptr_t) mapped);
			list_remove(pos);
		}
	}

	kfree((uintptr_t) (memory->mappedtasks));

	list* entry = list_find_data(tee_memory_list, (uintptr_t) memory);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

tee_memory* tee_memory_find(uint32_t id) {
	list* pos;
	list* next;
	tee_memory* session = NULL;
	tee_memory* session_res = NULL;

	tee_debug("Searching TEE memory @ ID-> %x", id);
	list_for_each_safe(pos, next, tee_memory_list)
	{
		if (pos->data != NULL ) {
			session = (tee_memory*) pos->data;
			if (session->_id == id) {
				session_res = session;
				break;
			}
		}
	}
	return (session_res);
}

tee_memory* tee_memory_find_by_ctx(tee_context* context) {
	list* pos;
	list* next;
	tee_memory* session = NULL;
	tee_memory* session_res = NULL;

	if (context == NULL ) {
		tee_error("Searching TEE memory with context NULL!");
		return (NULL );
	}

	tee_debug("Searching TEE memory @ CTX-> %x", context->_id);
	list_for_each_safe(pos, next, tee_memory_list)
	{
		if (pos->data != NULL ) {
			session = (tee_memory*) pos->data;
			if (session->context != NULL ) {
				if (session->context->_id == context->_id) {
					session_res = session;
					break;
				}
			}
		}
	}
	return (session_res);
}

uint32_t tee_memory_map_to_task(struct user_process_t *proc, tee_memory* memory) {
	uint32_t tvaddr = 0;

	if (memory == NULL ) {
		tee_error("Failed to map memory to task (memory is null)");
		return (0);
	}

	if (proc == NULL ) {
		tee_error("Failed to map memory to task (proc is null)");
		return (0);
	}

	// check if we already have a mapping...
	list* pos;
	list* next;
	tee_mapped_mem* mapped;
	list_for_each_safe(pos, next, memory->mappedtasks)
	{
		if (pos->data != NULL ) {
			mapped = (tee_mapped_mem*) pos->data;
			if (mapped->proc == proc) {
				tvaddr = mapped->vaddr;
				return (tvaddr);
			}
		}
	}

	mapped = (tee_mapped_mem*) kmalloc(sizeof(tee_mapped_mem));

	if (mapped == NULL ) {
		tee_error("Failed to map memory to task (out of memory)");
		return (0);
	}

	mapped->vaddr = (uint32_t) map_mem_to_task((uint8_t*) memory->paddr,
			memory->size, proc);

	if (mapped->vaddr == 0) {
		tee_error("Failed to map memory to task (map to proc failed!)");
		kfree((uintptr_t) mapped);
		return (0);
	}

	if (list_add(memory->mappedtasks, (uintptr_t) mapped) != 0) {
		tee_error("Failed to map memory to task (out of memory)");
		kfree((uintptr_t) mapped);
		return (0);
	}

	return (mapped->vaddr);
}
