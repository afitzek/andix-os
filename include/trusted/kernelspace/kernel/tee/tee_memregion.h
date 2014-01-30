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
 * tee_memregion.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_MEMREGION_H_
#define TEE_MEMREGION_H_

#include <common.h>
#include <tee/tee_context.h>
#include <task/user_process.h>

typedef struct {
	struct user_process_t *proc;
	uint32_t vaddr;
} tee_mapped_mem;

typedef struct {
	uint32_t	_id;
	uint32_t	paddr;
	uint32_t	vaddr;
	uint32_t	size;
	tid_t		task_id;
	uint32_t	uaddr;
	tee_context* context;
} tee_temp_memory;

typedef struct {
	uint32_t _id;
	tee_context* context;
	uint32_t	flags;
	uint32_t	paddr;
	uint32_t	vaddr;
	uint32_t	size;
	list*		mappedtasks;
} tee_memory;

void tee_memory_initialize();
tee_memory* tee_memory_create(tee_context* context);
void tee_memory_destroy(tee_memory* memory);
tee_memory* tee_memory_find(uint32_t id);
tee_memory* tee_memory_find_by_ctx(tee_context* context);
uint32_t tee_memory_map_to_task(struct user_process_t *proc, tee_memory* memory);
void tee_tmp_mem_free(tee_temp_memory* mem);
tee_temp_memory* tee_tmp_find_by_vaddr_and_task(uint32_t vaddr, tid_t tid);
tee_temp_memory* tee_tmp_mem_create(void);


#endif /* TEE_MEMREGION_H_ */
