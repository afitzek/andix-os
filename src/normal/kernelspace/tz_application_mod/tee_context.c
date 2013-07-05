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
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_context.h>
//#include <asm-generic/current.h>

tee_list_t* tee_contexts;

int tee_context_init() {
	tee_contexts = (tee_list_t*) kmalloc(sizeof(tee_list_t), GFP_KERNEL);

	if (tee_contexts == NULL) {
		printk(KERN_ERR "tee_context_init: Out of memory");
		return (-1);
	}

	tee_list_init(tee_contexts, NULL);

	return (0);
}

tee_context* tee_context_add() {
	uint32_t id = 0;

	tee_context* ctx = (tee_context*) kmalloc(sizeof(tee_context), GFP_KERNEL);

	tee_context* ctx_with_id = NULL;

	if (ctx == NULL) {
		printk(KERN_ERR "tee_context_add: Out of memory");
		return (NULL);
	}

	do {
		get_random_bytes(&id, sizeof(id));
		ctx_with_id = tee_context_find_by_id(id);
	} while (ctx_with_id != NULL);

	ctx->id = id;
	ctx->id = current->pid;

	tee_list_add(tee_contexts, (void*) ctx);

	return (ctx);
}

void tee_context_free(tee_context* ctx) {
	tee_list_t* entry = NULL;
	if (ctx != NULL) {
		entry = tee_list_find_data(tee_contexts, (void*) ctx);
		if (entry != NULL) {
			tee_list_remove(entry);
		}
		kfree((void*) ctx);
	}
}

tee_context* tee_context_find_by_pid(pid_t pid) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_context* ctx;
	list_for_each_safe(pos, next, tee_contexts)
	{
		if (pos->data != NULL) {
			ctx = (tee_context*) pos->data;
			if (ctx != NULL && ctx->pid == pid) {
				break;
			}
			ctx = NULL;
		}
	}

	return (ctx);
}

tee_context* tee_context_find_by_id(uint32_t id) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_context* ctx;
	list_for_each_safe(pos, next, tee_contexts)
	{
		if (pos->data != NULL) {
			ctx = (tee_context*) pos->data;
			if (ctx != NULL && ctx->id == id) {
				break;
			}
			ctx = NULL;
		}
	}

	return (ctx);
}

tee_context* tee_context_find_by_tzid(uint32_t tzid) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_context* ctx;
	list_for_each_safe(pos, next, tee_contexts)
	{
		if (pos->data != NULL) {
			ctx = (tee_context*) pos->data;
			if (ctx != NULL && ctx->tz_id == tzid) {
				break;
			}
			ctx = NULL;
		}
	}

	return (ctx);
}
