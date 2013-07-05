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
 * tee_context.c
 *
 *  Created on: Mar 21, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <kprintf.h>
#include <tee/tee_context.h>
#include <mm/mm.h>
#include <devices/random/random.h>

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
		kfree((uintptr_t)context);
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
