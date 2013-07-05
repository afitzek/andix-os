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
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_memory.h>
#include <tz_application_mod/tee_list.h>

tee_list_t* tee_memories;

int tee_memory_init() {
	tee_memories = (tee_list_t*) kmalloc(sizeof(tee_list_t), GFP_KERNEL);

	if (tee_memories == NULL) {
		printk(KERN_ERR "tee_memory_init: Out of memory");
		return (-1);
	}

	tee_list_init(tee_memories, NULL);

	return (0);
}

tee_shared_memory* tee_memory_add(tee_context* ctx) {
	uint32_t id = 0;

	tee_shared_memory* mem = (tee_shared_memory*) kmalloc(
			sizeof(tee_shared_memory), GFP_KERNEL);

	tee_shared_memory* mem_with_id = NULL;

	if (mem == NULL) {
		printk(KERN_ERR "tee_memory_add: Out of memory");
		return (NULL);
	}

	do {
		get_random_bytes(&id, sizeof(id));
		mem_with_id = tee_memory_find_by_id(id);
	} while (mem_with_id != NULL);

	mem->id = id;
	mem->ctx = ctx;

	tee_list_add(tee_memories, (void*) mem);

	return (mem);
}

void tee_memory_free(tee_shared_memory* mem) {
	tee_list_t* entry = NULL;
	if (mem != NULL) {
		entry = tee_list_find_data(tee_memories, (void*) mem);
		if (entry != NULL) {
			tee_list_remove(entry);
		}
		kfree((void*) mem);
	}
}

tee_shared_memory* tee_memory_find_by_id(uint32_t id) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_shared_memory* mem;
	list_for_each_safe(pos, next, tee_memories)
	{
		if (pos->data != NULL) {
			mem = (tee_shared_memory*) pos->data;
			if (mem != NULL && mem->id == id) {
				break;
			}
			mem = NULL;
		}
	}

	return (mem);
}

tee_shared_memory* tee_memory_find_by_tzid(uint32_t tzid) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_shared_memory* mem;
	list_for_each_safe(pos, next, tee_memories)
	{
		if (pos->data != NULL) {
			mem = (tee_shared_memory*) pos->data;
			if (mem != NULL && mem->tz_id == tzid) {
				break;
			}
			mem = NULL;
		}
	}

	return (mem);
}

tee_shared_memory* tee_memory_find_by_ctx(tee_context* ctx) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_shared_memory* mem;
	list_for_each_safe(pos, next, tee_memories)
	{
		if (pos->data != NULL) {
			mem = (tee_shared_memory*) pos->data;
			if (mem != NULL && mem->ctx == ctx) {
				break;
			}
			mem = NULL;
		}
	}

	return (mem);
}

tee_shared_memory* tee_memory_find_by_paddr(void* paddr) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_shared_memory* mem;
	list_for_each_safe(pos, next, tee_memories)
	{
		if (pos->data != NULL) {
			mem = (tee_shared_memory*) pos->data;
			if (mem != NULL && mem->com_paddr == paddr) {
				break;
			}
			mem = NULL;
		}
	}

	return (mem);
}
