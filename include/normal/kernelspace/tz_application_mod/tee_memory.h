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
 * tee_memory.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_MEMORY_H_
#define TEE_MEMORY_H_

#include <tz_application_mod/tee_context.h>
#include <tz_application_mod/types.h>

#define TEE_MEM_STATE_MAPPED		(0)
#define TEE_MEM_STATE_TEMP			(1)


typedef struct {
	uint32_t id;
	tee_context* ctx;
	uint32_t tz_id;
	void* user_addr;
	void* com_vaddr;
	void* com_paddr;
	uint32_t state;
	uint32_t flags;
	size_t size;
} tee_shared_memory;

int tee_memory_init(void);
tee_shared_memory* tee_memory_add(tee_context* ctx);
void tee_memory_free(tee_shared_memory* session);
tee_shared_memory* tee_memory_find_by_id(uint32_t id);
tee_shared_memory* tee_memory_find_by_tzid(uint32_t tzid);
tee_shared_memory* tee_memory_find_by_ctx(tee_context* ctx);
tee_shared_memory* tee_memory_find_by_paddr(void* paddr);
int tee_memory_check_state(tee_shared_memory* mem, int state_param);
void tee_memory_set_state(tee_shared_memory* mem, int state_param);
void tee_memory_clear_state(tee_shared_memory* mem, int state_param);

tee_shared_memory* tee_tmp_mem_add(tee_context* ctx);
void tee_tmp_mem_free(tee_shared_memory* mem);
tee_shared_memory* tee_tmp_memory_find_by_paddr(void* paddr);

#endif /* TEE_MEMORY_H_ */
