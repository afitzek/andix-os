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
 * tee_operations.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_OPERATIONS_H_
#define TEE_OPERATIONS_H_

#include <communication_types.h>
#include <tz_application_mod/tee_context.h>
#include <tz_application_mod/tee_session.h>
#include <tz_application_mod/tee_memory.h>
#include <tz_application_mod/tee_logic.h>

#define TEE_EVENT_RET_SUCCESS	(0)
#define TEE_EVENT_RET_ERROR		(-1)
#define TEE_EVENT_RET_NO_TZ		(1)

typedef int (*tee_op_handler)(TZ_TEE_SPACE* com_mem);

typedef struct {
	tee_op_handler pre;
	tee_op_handler post;
} op_event;

int tee_op_dummy(TZ_TEE_SPACE* com_mem);

int tee_ctx_init_pre(TZ_TEE_SPACE* com_mem);
int tee_ctx_init_post(TZ_TEE_SPACE* com_mem);

int tee_ctx_finalize_pre(TZ_TEE_SPACE* com_mem);
int tee_ctx_finalize_post(TZ_TEE_SPACE* com_mem);

int tee_mem_reg_pre(TZ_TEE_SPACE* com_mem);
int tee_mem_reg_post(TZ_TEE_SPACE* com_mem);
int tee_mem_rel_pre(TZ_TEE_SPACE* com_mem);
int tee_mem_rel_post(TZ_TEE_SPACE* com_mem);

int tee_session_open_pre(TZ_TEE_SPACE* com_mem);
int tee_session_open_post(TZ_TEE_SPACE* com_mem);

int tee_session_close_pre(TZ_TEE_SPACE* com_mem);
int tee_session_close_post(TZ_TEE_SPACE* com_mem);

int tee_invoke_pre(TZ_TEE_SPACE* com_mem);
int tee_invoke_post(TZ_TEE_SPACE* com_mem);

extern op_event tee_ctx_init;
extern op_event tee_ctx_finalize;
extern op_event tee_mem_reg;
extern op_event tee_mem_rel;
extern op_event tee_session_open;
extern op_event tee_session_close;
extern op_event tee_invoke;

#endif /* TEE_OPERATIONS_H_ */
