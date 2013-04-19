/*
 * tee_context.c
 *
 *  Created on: Mar 21, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <kprintf.h>
#include <tee/tee_context.h>

list* tee_context_list = NULL;

void tee_context_initialize() {
	tee_context_list = (list*) kmalloc(sizeof(list));
	tee_context_list->next = tee_context_list;
	tee_context_list->prev = tee_context_list;
	tee_context_list->data = NULL;
}

tee_context* tee_context_create() {
	tee_context* context = (tee_context*) kmalloc(sizeof(tee_context));
	if (!context) {
		tee_error("Failed to create context (out of memory)");
		return (NULL );
	}

	uint32_t id = 1;
	tee_context* test = NULL;

	do {
		id = random_next32();
		test = tee_context_find(id);
	} while (test != NULL );

	tee_info("New TEE context with ID %x", id);

	context->_id = id;

	if (list_add(tee_context_list, (uintptr_t) context) != 0) {
		tee_error("Failed to create context (out of memory)");
		kfree(context);
		return (NULL );
	}
	return (context);
}

void tee_context_remove(tee_context* ctx) {
	if (ctx == NULL ) {
		return;
	}

	// TODO: cleanup context

	list* entry = list_find_data(tee_context_list, (uintptr_t) ctx);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

tee_context* tee_context_find(uint32_t id) {
	list* pos;
	list* next;
	tee_context* context = NULL;
	tee_context* context_res = NULL;
	tee_debug("Searching TEE context @ %x", id);
	list_for_each_safe(pos, next, tee_context_list)
	{
		if (pos->data != NULL ) {
			context = (tee_context*) pos->data;
			if (context->_id == id) {
				context_res = context;
				break;
			}
		}
	}
	return (context_res);
}
