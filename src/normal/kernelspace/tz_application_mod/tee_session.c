/*
 * tee_session.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_list.h>
#include <tz_application_mod/tee_session.h>

tee_list_t* tee_sessions;

int tee_session_init() {
	tee_sessions = (tee_list_t*) kmalloc(sizeof(tee_list_t), GFP_KERNEL);

	if (tee_sessions == NULL) {
		printk(KERN_ERR "tee_session_init: Out of memory");
		return (-1);
	}

	tee_list_init(tee_sessions, NULL);

	return (0);
}

tee_session* tee_session_add(tee_context* ctx) {
	uint32_t id = 0;

	tee_session* session = (tee_session*) kmalloc(sizeof(tee_session),
			GFP_KERNEL);

	tee_session* session_with_id = NULL;

	if (session == NULL) {
		printk(KERN_ERR "tee_session_add: Out of memory");
		return (NULL);
	}

	do {
		get_random_bytes(&id, sizeof(id));
		session_with_id = tee_session_find_by_id(id);
	} while (session_with_id != NULL);

	session->id = id;
	session->ctx = ctx;

	tee_list_add(tee_sessions, (void*) session);

	return (session);
}

void tee_session_free(tee_session* session) {
	tee_list_t* entry = NULL;
	if (session != NULL) {
		entry = tee_list_find_data(tee_sessions, (void*) session);
		if (entry != NULL) {
			tee_list_remove(entry);
		}
		kfree((void*) session);
	}
}

tee_session* tee_session_find_by_id(uint32_t id) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_session* session;
	list_for_each_safe(pos, next, tee_sessions)
	{
		if (pos->data != NULL) {
			session = (tee_session*) pos->data;
			if (session != NULL && session->id == id) {
				break;
			}
			session = NULL;
		}
	}

	return (session);
}

tee_session* tee_session_find_by_ctx(tee_context* ctx) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_session* session;
	list_for_each_safe(pos, next, tee_sessions)
	{
		if (pos->data != NULL) {
			session = (tee_session*) pos->data;
			if (session != NULL && session->ctx == ctx) {
				break;
			}
			session = NULL;
		}
	}

	return (session);
}

tee_session* tee_session_find_by_tzid(uint32_t tzid) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_session* session;
	list_for_each_safe(pos, next, tee_sessions)
	{
		if (pos->data != NULL) {
			session = (tee_session*) pos->data;
			if (session != NULL && session->tz_id == tzid) {
				break;
			}
			session = NULL;
		}
	}

	return (session);
}
