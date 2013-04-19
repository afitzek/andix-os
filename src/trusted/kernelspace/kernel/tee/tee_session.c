/*
 * tee_session.c
 *
 *  Created on: Mar 24, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee/tee_session.h>
#include <kprintf.h>

list* tee_session_list = NULL;

void tee_session_initialize() {
	tee_session_list = (list*) kmalloc(sizeof(list));
	tee_session_list->next = tee_session_list;
	tee_session_list->prev = tee_session_list;
	tee_session_list->data = NULL;
}

tee_session* tee_session_create(tee_context* ctx) {

	if(ctx == NULL) {
		tee_error("Failed to create session ctx is NULL");
		return (NULL);
	}

	tee_session* session = (tee_session*) kmalloc(sizeof(tee_session));
	if (!session) {
		tee_error("Failed to create session (out of memory)");
		return (NULL );
	}

	uint32_t id = 1;
	tee_session* test = NULL;

	do {
		id = random_next32();
		test = tee_session_find(id);
	} while (test != NULL );

	tee_info("New TEE session with ID %x", id);

	session->_id = id;

	session->context = ctx;

	if (list_add(tee_session_list, (uintptr_t) session) != 0) {
		tee_error("Failed to create context (out of memory)");
		kfree(session);
		return (NULL );
	}

	return (session);
}

void tee_session_destroy(tee_session* session) {
	if (session == NULL ) {
		return;
	}

	// TODO: cleanup session

	list* entry = list_find_data(tee_session_list, (uintptr_t) session);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

tee_session* tee_session_find(uint32_t id) {
	list* pos;
	list* next;
	tee_session* session = NULL;
	tee_session* session_res = NULL;
	tee_debug("Searching TEE session @ %x", id);
	list_for_each_safe(pos, next, tee_session_list)
	{
		if (pos->data != NULL ) {
			session = (tee_session*) pos->data;
			if (session->_id == id) {
				session_res = session;
				break;
			}
		}
	}
	return (session_res);
}

tee_session* tee_session_find_by_ctx(tee_context* context) {
	list* pos;
	list* next;
	tee_session* session = NULL;
	tee_session* session_res = NULL;

	if (context == NULL ) {
		tee_error("Searching TEE session with context NULL!");
		return (NULL );
	}

	tee_debug("Searching TEE session @ CTX-> %x", context->_id);
	list_for_each_safe(pos, next, tee_session_list)
	{
		if (pos->data != NULL ) {
			session = (tee_session*) pos->data;
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
