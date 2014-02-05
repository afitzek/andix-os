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
#include <common.h>
#include <mm/mm.h>
#include <tee/tee_constants.h>
#include <tee/tee_context.h>
#include <tee/tee_session.h>
#include <tee/tee_memregion.h>
#include <communication_types.h>
#include <kprintf.h>
#include <task/tee.h>
#include <task/thread.h>
#include <scheduler.h>
#include <tee_client_api.h>

static struct thread_t *tee_thread;

struct thread_t *tee_get_tee_thread(void) {
	return tee_thread;
}

uint32_t tee_release_mem(TZ_TEE_REGISTER_MEM* operation) {
	tee_memory* mem = tee_memory_find(operation->memid);

	if (mem == NULL ) {
		return (TEE_ERROR_BAD_STATE);
	}

	/*if (mem->vaddr != 0) {
	 uint32_t mmm_pages = needed_pages((void*) (mem->paddr), mem->size);
	 uint32_t i = 0;
	 for (i = 0; i < mmm_pages; i++) {
	 unmap_kernel_memory(mem->vaddr + (SMALL_PAGE_SIZE * i));
	 mmm_free_page((void*) (mem->vaddr + (SMALL_PAGE_SIZE * i)));
	 }
	 }*/

	tee_memory_destroy(mem);
	return (TEE_SUCCESS);
}

uint32_t tee_register_mem(TZ_TEE_REGISTER_MEM* operation) {
	void* unsecure_paddr = (void*) operation->paddr;

	tee_info("Registering mem @ p 0x%x", unsecure_paddr);

	if (!is_valid_nonsecure_ram_addr(unsecure_paddr)) {
		tee_error("Invalid physical ram address: @ 0x%x", operation->paddr);
		return (TEE_ERROR_BAD_PARAMETERS);
	}

	tee_context* ctx = tee_context_find(operation->context);

	if (ctx == NULL ) {
		return (TEE_ERROR_BAD_STATE);
	}

	tee_memory* mem = tee_memory_create(ctx);

	if (mem == NULL ) {
		return (TEE_ERROR_OUT_OF_MEMORY);
	}

	mem->context = ctx;
	mem->flags = operation->flags;
	operation->memid = mem->_id;
	mem->paddr = operation->paddr;
	mem->size = operation->size;
	mem->vaddr = 0;

	return (TEE_SUCCESS);
}

uint32_t tee_init_context(TZ_TEE_INIT_CTX* operation) {
	tee_context* ctx = tee_context_create();

	tee_debug("MAGIC ctx value: 0x%x", operation->context);

	if (operation->context != 0xDEADC) {
		return (TEE_ERROR_COMMUNICATION);
	}

	if (ctx != NULL ) {
		operation->context = ctx->_id;
		tee_info("Created context with id: 0x%x", operation->context);
		return (TEE_SUCCESS);
	}

	tee_error("failed to create context (OOM)");
	return (TEE_ERROR_OUT_OF_MEMORY);
}

uint32_t tee_finalize_context(TZ_TEE_FIN_CTX* operation) {

	tee_info("Finalizing context with id: 0x%x", operation->context);

	tee_context* ctx = tee_context_find(operation->context);

	if (ctx != NULL ) {
		tee_context_remove(ctx);
		return (TEE_SUCCESS);
	}

	tee_error("failed to finalize context");
	return (TEE_ERROR_BAD_PARAMETERS);
}

uint32_t tee_open_session(TZ_TEE_OPEN_SESSION* operation, TZ_TEE_SPACE* tee) {
	tee_info("Opening session in ctx with id: 0x%x", operation->context);

	tee_context* ctx = tee_context_find(operation->context);

	if (ctx == NULL ) {
		tee_error("Invalid context id! %x", operation->context);
		operation->returnOrigin = TEE_ORIGIN_TEE;
		return (TEE_ERROR_COMMUNICATION);
	}

	tee_session* session = tee_session_create(ctx);

	if (session == NULL ) {
		tee_error("failed to create session");
		operation->returnOrigin = TEE_ORIGIN_TEE;
		return (TEE_ERROR_OUT_OF_MEMORY);
	}

	// TODO we need a dynamic loader ...

	struct user_process_t *trusted_app = get_process_by_uuid((TASK_UUID*) &operation->uuid);

	if (trusted_app == NULL ) {
		tee_error("UUID not found!");
		operation->returnOrigin = TEE_ORIGIN_TEE;
		return (TEE_ERROR_ITEM_NOT_FOUND);
	}

	tee->params.openSession.session = session->_id;

	session->tee_application = trusted_app;
	trusted_app->tee_context.tee_rpc = (uintptr_t)tee;
	tee_info("Going to trusted app: %s", trusted_app->name);
	switch_to_thread(trusted_app->tee_context.tee_handler);
	tee_info("Back from trusted app: %s", trusted_app->name);
	return (tee->ret);
}

uint32_t tee_close_session(TZ_TEE_CLOSE_SESSION* operation, TZ_TEE_SPACE* tee) {

	tee_session* session = tee_session_find(operation->session);

	if (session == NULL ) {
		tee_error("failed to create session");
		return (TEE_ERROR_OUT_OF_MEMORY);
	}

	struct user_process_t *trusted_app = session->tee_application;

	if (trusted_app == NULL ) {
		tee_error("Trustlet not found!");
		return (TEE_ERROR_ITEM_NOT_FOUND);
	}

	trusted_app->tee_context.tee_rpc = (uintptr_t)tee;

	switch_to_thread(trusted_app->tee_context.tee_handler);

	return (tee->ret);
}

uint32_t tee_invoke(TZ_TEE_INVOKE_CMD* operation, TZ_TEE_SPACE* tee) {

	tee_session* session = tee_session_find(operation->session);

	if (session == NULL ) {
		tee_error("failed to find session");
		return (TEE_ERROR_OUT_OF_MEMORY);
	}

	struct user_process_t *trusted_app = session->tee_application;

	if (trusted_app == NULL ) {
		tee_error("Trustlet not found!");
		return (TEE_ERROR_ITEM_NOT_FOUND);
	}

	trusted_app->tee_context.tee_rpc = (uintptr_t)tee;

	switch_to_thread(trusted_app->tee_context.tee_handler);

	return (tee->ret);
}

void tee_task_entry() {
	TZ_TEE_SPACE space;
	TZ_TEE_SPACE* tee;
	//void* ptee;
	if (tee_thread) {
		tee_error("tee_task_entry started a second time!");
		kpanic();
	}
	tee_thread = get_current_thread();
	while (1) {
		tee = mon_get_tee_space();
		if (tee == NULL ) {
			get_current_thread()->state = BLOCKED;
			yield();
			continue;
		}

		//ptee = v_to_p(tee);

		//tee_info("physical tee @ 0x%x", ptee);

		// Copy memory to trusted only memory
		memcpy(&space, tee, sizeof(TZ_TEE_SPACE));
		//tee_info("LOCAL TEE SPACE");
		//kprintHex(&space, sizeof(TZ_TEE_SPACE));

		uint32_t result = TEEC_ERROR_COMMUNICATION;

		tee_debug("TEE Request 0x%x", space.op);

		switch (space.op) {
		case TZ_TEE_OP_REGISTER_MEM:
			tee_debug("TZ_TEE_OP_REGISTER_MEM OPERATION");
			result = tee_register_mem(&(space.params.regMem));
			break;
		case TZ_TEE_OP_RELEASE_MEM:
			tee_debug("TZ_TEE_OP_RELEASE_MEM OPERATION");
			result = tee_release_mem(&(space.params.regMem));
			break;
		case TZ_TEE_OP_INIT_CTX:
			tee_debug("TZ_TEE_OP_INIT_CTX OPERATION");
			result = tee_init_context(&(space.params.initCtx));
			break;
		case TZ_TEE_OP_FIN_CTX:
			tee_debug("TZ_TEE_OP_FIN_CTX OPERATION");
			result = tee_finalize_context(&(space.params.finCtx));
			break;
		case TZ_TEE_OP_OPEN_SESSION:
			tee_debug("TZ_TEE_OP_OPEN_SESSION OPERATION");
			result = tee_open_session(&(space.params.openSession), &space);
			break;
		case TZ_TEE_OP_CLOSE_SESSION:
			tee_debug("TZ_TEE_OP_CLOSE_SESSION OPERATION");
			result = tee_close_session(&(space.params.closeSession), &space);
			break;
		case TZ_TEE_OP_INVOKE_CMD:
			tee_debug("TZ_TEE_OP_INVOKE_CMD OPERATION");
			result = tee_invoke(&(space.params.invokeCommand), &space);
			break;
		default:
			tee_info("UNKOWN TEE OPERATION (0x%x)", space.op);
			break;
		}
		space.ret = result;
		/*tee_info("space.params.initCtx.context: 0x%x",
		 space.params.initCtx.context);
		 tee_info("tee->params.initCtx.context: 0x%x",
		 tee->params.initCtx.context);
		 tee_info("memcpy");*/
		//tee_info("LOCAL TEE SPACE");
		//kprintHex(&space, sizeof(TZ_TEE_SPACE));
		memcpy(tee, &space, sizeof(TZ_TEE_SPACE));
		/*tee_info("space.params.initCtx.context: 0x%x",
		 space.params.initCtx.context);
		 tee_info("tee->params.initCtx.context: 0x%x",
		 tee->params.initCtx.context);*/
		get_current_thread()->state = BLOCKED;

		mon_tee_response();

		//return_to_ns();
	}
}
