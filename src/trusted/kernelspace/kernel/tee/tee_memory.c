/*
 * tee_memory.c
 *
 *  Created on: Apr 3, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee/tee_memregion.h>
#include <kprintf.h>

list* tee_memory_list = NULL;

void tee_memory_initialize() {
	tee_memory_list = (list*) kmalloc(sizeof(list));
	tee_memory_list->next = tee_memory_list;
	tee_memory_list->prev = tee_memory_list;
	tee_memory_list->data = NULL;
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
		kfree(memory);
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
		kfree(memory);
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
					mapped->task);
			kfree(mapped);
			list_remove(pos);
		}
	}

	kfree(memory->mappedtasks);

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

uint32_t tee_memory_map_to_task(task_t *task, tee_memory* memory) {
	uint32_t tvaddr = 0;

	if (memory == NULL ) {
		tee_error("Failed to map memory to task (memory is null)");
		return (0);
	}

	if (task == NULL ) {
		tee_error("Failed to map memory to task (task is null)");
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
			if (mapped->task == task) {
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
			memory->size, task);

	if (mapped->vaddr == 0) {
		tee_error("Failed to map memory to task (map to task failed!)");
		kfree(mapped);
		return (0);
	}

	if (list_add(memory->mappedtasks, (uintptr_t) mapped) != 0) {
		tee_error("Failed to map memory to task (out of memory)");
		kfree(mapped);
		return (0);
	}

	return (mapped->vaddr);
}
