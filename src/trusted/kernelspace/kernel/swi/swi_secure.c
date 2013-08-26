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
 * swi_secure.c
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <swi/swi.h>
#include <tee/tee_constants.h>
#include <tee/tee_context.h>
#include <tee/tee_session.h>
#include <tee/tee_memregion.h>
#include <tee/constants/tee_client_constants.h>
#include <mm/mm.h>
#include <scheduler.h>

tee_temp_memory* swi_map_tee_temp_ref(TEEC_TmpMemoryReference* tmp,
		uint32_t paramType, task_t* task) {
	if (!is_valid_nonsecure_ram_addr(tmp->buffer)) {
		return (NULL );
	}

	// map to kernel
	tee_temp_memory* mem = tee_tmp_mem_create();

	if (mem == NULL ) {
		tee_error("Out of memory");
		return (NULL );
	}

	mem->paddr = tmp->buffer;
	mem->size = tmp->size;
	mem->task_id = task->tid;
	mem->vaddr = map_phys_mem(mem->paddr, mem->size, AP_SVC_RW_USR_NO, 0, 0, 0);
	mem->uaddr = allocate_map_mem_to_task(mem->size, task);

	if (mem->uaddr == NULL ) {
		tee_error("Out of memory");
		return (NULL );
	}

	if (paramType == TEEC_MEMREF_TEMP_INOUT
			|| paramType == TEEC_MEMREF_TEMP_INPUT) {
		// this only works if task == current task!!
		memcpy(mem->uaddr, mem->vaddr, mem->size);
	}

	return (mem);
}

void swi_unmap_tee_temp_ref(tee_temp_memory* mem, uint32_t paramType,
		task_t* task) {
	uint32_t freed;
	if (paramType == TEEC_MEMREF_TEMP_INOUT
			|| paramType == TEEC_MEMREF_TEMP_OUTPUT) {
		// this only works if task == current task!!
		memcpy(mem->vaddr, mem->uaddr, mem->size);
	}

	free_mem_from_task(mem->uaddr, mem->size, task);

	freed = 0;

	while (freed < mem->size) {
		unmap_kernel_memory((uint32_t) mem->vaddr);
		mem->vaddr += SMALL_PAGE_SIZE;
		freed += SMALL_PAGE_SIZE;
	}

	tee_tmp_mem_free(mem);
}

int parameters_to_userspace(TEECOM_Operation* comOp, TA_RPC *rpc, task_t* task) {
	int pidx = 0;
	uint32_t paramType = 0;
	uint32_t paddr;
	uint32_t size;
	uint32_t freed;
	uint8_t* v_mem_addr;
	tee_memory* memory = NULL;
	tee_temp_memory* tmpmem = NULL;
	if (comOp->valid == 1) {
		rpc->paramTypes = comOp->paramTypes;
		for (pidx = 0; pidx < 4; pidx++) {
			paramType = TEE_PARAM_TYPE_GET(comOp->paramTypes, pidx);
			switch (paramType) {
			case TEEC_NONE:
				rpc->tee_param[pidx].value.a = 0;
				rpc->tee_param[pidx].value.b = 0;
				break;
			case TEEC_VALUE_INPUT:
			case TEEC_VALUE_INOUT:
				swi_info("VALUE PARAM A 0x%x B 0x%x",
						comOp->params[pidx].value.a,
						comOp->params[pidx].value.b);
				rpc->tee_param[pidx].value.a = comOp->params[pidx].value.a;
				rpc->tee_param[pidx].value.b = comOp->params[pidx].value.b;
				break;
			case TEEC_MEMREF_TEMP_INOUT:
			case TEEC_MEMREF_TEMP_INPUT:
			case TEEC_MEMREF_TEMP_OUTPUT:

				// check ram addr
				// map to kernel
				// allocate task mem
				// copy

				tmpmem = swi_map_tee_temp_ref(&comOp->params[pidx].tmpref,
						paramType, task);

				if (tmpmem == NULL ) {
					return (TEEC_ERROR_COMMUNICATION);
				}

				rpc->tee_param[pidx].memref.buffer = tmpmem->uaddr;
				rpc->tee_param[pidx].memref.size = tmpmem->size;

				//comOp->params[pidx].tmpref.buffer
				//rpc->tee_param[pidx].memref.buffer;
				//rpc->tee_param[pidx].memref.size;
				break;
			case TEEC_MEMREF_PARTIAL_INOUT:
			case TEEC_MEMREF_PARTIAL_INPUT:
			case TEEC_MEMREF_WHOLE:
			case TEEC_MEMREF_PARTIAL_OUTPUT:
				memory = tee_memory_find(comOp->params[pidx].memref.memid);

				if (memory == NULL ) {
					tee_error("Failed to find memory with id: %d",
							comOp->params[pidx].memref.memid);
					return (TEEC_ERROR_BAD_PARAMETERS);
				}
				paddr = memory->paddr + comOp->params[pidx].memref.offset;
				size = comOp->params[pidx].memref.size;

				if (paddr + size > memory->paddr + memory->size) {
					tee_error("Memory parameters are invalid!");
					return (TEEC_ERROR_BAD_PARAMETERS);
				}

				rpc->tee_param[pidx].memref.buffer = allocate_map_mem_to_task(
						size, task);

				if (rpc->tee_param[pidx].memref.buffer == NULL ) {
					tee_error("Out of memory");
					return (TEEC_ERROR_OUT_OF_MEMORY);
				}

				rpc->tee_param[pidx].memref.size = size;

				if (paramType != TEEC_MEMREF_PARTIAL_OUTPUT) {

					v_mem_addr = (uint8_t*) map_phys_mem(
							(uintptr_t) memory->paddr, memory->size,
							AP_SVC_RW_USR_NO, 0, 0, 0);

					if (v_mem_addr == NULL ) {
						free_mem_from_task(rpc->tee_param[pidx].memref.buffer,
								size, task);
						tee_error("Out of memory");
						return (TEEC_ERROR_OUT_OF_MEMORY);
					}

					memcpy(rpc->tee_param[pidx].memref.buffer, v_mem_addr,
							size);

					freed = 0;

					while (freed < memory->size) {
						unmap_kernel_memory((uint32_t) v_mem_addr);
						v_mem_addr += SMALL_PAGE_SIZE;
						freed += SMALL_PAGE_SIZE;
					}
				}
				break;
			default:
				tee_error("Not supported!")
				;
				return (TEEC_ERROR_BAD_PARAMETERS);
			}
		}
	} else {
		rpc->paramTypes = 0;
		for (pidx = 0; pidx < 4; pidx++) {
			rpc->tee_param[pidx].value.a = 0;
			rpc->tee_param[pidx].value.b = 0;
		}
	}
	return (TEEC_SUCCESS);
}

int parameters_from_userspace(TEECOM_Operation* comOp, TA_RPC *rpc,
		task_t* task) {
	int pidx = 0;
	uint32_t paramType = 0;
	uint32_t paddr;
	uint32_t size = 0;
	uint32_t freed;
	uint8_t* v_mem_addr;
	tee_memory* memory = NULL;
	tee_temp_memory* tmpmem = NULL;
	if (comOp->valid == 1) {
		for (pidx = 0; pidx < 4; pidx++) {
			paramType = TEE_PARAM_TYPE_GET(comOp->paramTypes, pidx);
			switch (paramType) {
			case TEEC_NONE:
				rpc->tee_param[pidx].value.a = 0;
				rpc->tee_param[pidx].value.b = 0;
				break;
			case TEEC_VALUE_OUTPUT:
			case TEEC_VALUE_INOUT:
				comOp->params[pidx].value.a = rpc->tee_param[pidx].value.a;
				comOp->params[pidx].value.b = rpc->tee_param[pidx].value.b;
				break;
			case TEEC_MEMREF_TEMP_INOUT:
			case TEEC_MEMREF_TEMP_INPUT:
			case TEEC_MEMREF_TEMP_OUTPUT:

				tmpmem = tee_tmp_find_by_vaddr_and_task(
						(uint32_t)rpc->tee_param[pidx].memref.buffer,
						task->tid);

				if (tmpmem == NULL ) {
					tee_error("Failed to find memory with @ %x",
							paddr);
					return (TEEC_ERROR_BAD_PARAMETERS);
				}

				swi_unmap_tee_temp_ref(tmpmem, paramType, task);

				break;
			case TEEC_MEMREF_PARTIAL_INOUT:
			case TEEC_MEMREF_PARTIAL_INPUT:
			case TEEC_MEMREF_WHOLE:
			case TEEC_MEMREF_PARTIAL_OUTPUT:
				memory = tee_memory_find(comOp->params[pidx].memref.memid);

				if (memory == NULL ) {
					tee_error("Failed to find memory with id: %d",
							comOp->params[pidx].memref.memid);
					return (TEEC_ERROR_BAD_PARAMETERS);
				}

				size = comOp->params[pidx].memref.size;

				if (paramType != TEEC_MEMREF_PARTIAL_INPUT) {

					v_mem_addr = (uint8_t*) map_phys_mem(
							(uintptr_t) memory->paddr, memory->size,
							AP_SVC_RW_USR_NO, 0, 0, 0);

					if (v_mem_addr == NULL ) {
						free_mem_from_task(rpc->tee_param[pidx].memref.buffer,
								size, task);
						tee_error("Out of memory");
						return (TEEC_ERROR_OUT_OF_MEMORY);
					}

					memcpy(v_mem_addr, rpc->tee_param[pidx].memref.buffer,
							size);

					freed = 0;

					while (freed < memory->size) {
						unmap_kernel_memory((uint32_t) v_mem_addr);
						v_mem_addr += SMALL_PAGE_SIZE;
						freed += SMALL_PAGE_SIZE;
					}
				}

				free_mem_from_task(rpc->tee_param[pidx].memref.buffer, size,
						task);
				break;
			}
		}
	}

	return (TEEC_SUCCESS);
}

void swi_sysd_install(uint8_t* mem, uint32_t length) {
	task_info("KERNEL INSTALLING 0x%X", mem);
}

void swi_set_uuid(TASK_UUID* uuid) {
	if (!is_valid_user_addr(uuid)) {
		// TODO: KILL TASK!
		return;
	}

	task_t* current = get_current_task();
	cpy_uuid(&(current->uuid), uuid);
}

void swi_get_secure_request(TA_RPC* rpc) {
	tee_session* session = NULL;
	//tee_context* context = NULL;
	uint8_t procceed = 0;
	uint32_t result;

	if (!is_valid_user_addr(rpc)) {
		// TODO: KILL TASK!
		return;
	}

	task_t* task = get_current_task();

	if (!is_valid_kernel_addr(task)) {
		// TODO: kernel panic!
		swi_error("CALLED swi_get_secure_request with current task is NULL!");
	}

// TODO: process answer
	if (task->tee_rpc != NULL ) {
		TZ_TEE_SPACE* tee = (TZ_TEE_SPACE*) task->tee_rpc;
		switch (tee->op) {
		case TZ_TEE_OP_OPEN_SESSION:
			session = tee_session_find(tee->params.openSession.session);
			if (session == NULL ) {
				tee->ret = TEE_ERROR_GENERIC;
				tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
				tee_error("Failed to find session!");
				break;
			}

			tee->ret = parameters_from_userspace(
					&tee->params.openSession.operation, rpc,
					get_current_task());

			if (tee->ret != TEEC_SUCCESS) {
				tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
				tee_error("Failed to copy parameters from userspace");
			} else {
				session->tee_session_handle = (uint32_t) rpc->sessionContext;
				tee->ret = rpc->result;
				tee->params.openSession.returnOrigin = TEE_ORIGIN_TRUSTED_APP;
				tee_info("Created session %d", session->_id);
			}
			break;
		case TZ_TEE_OP_INVOKE_CMD:
			session = tee_session_find(tee->params.invokeCommand.session);
			if (session == NULL ) {
				tee->ret = TEE_ERROR_GENERIC;
				tee->params.invokeCommand.returnOrigin = TEE_ORIGIN_TEE;
				tee_error("Failed to find session!");
				break;
			}

			tee->ret = parameters_from_userspace(
					&tee->params.invokeCommand.operation, rpc,
					get_current_task());

			if (tee->ret != TEEC_SUCCESS) {
				tee->params.invokeCommand.returnOrigin = TEE_ORIGIN_TEE;
				tee_error("Failed to copy parameters from userspace");
			} else {
				tee->ret = rpc->result;
				tee->params.invokeCommand.returnOrigin = TEE_ORIGIN_TRUSTED_APP;
			}
			break;
		}
	}

	task->tee_rpc = NULL;

	do {
		// block current task ...
		get_current_task()->state = BLOCKED;
		task_t* tee_task = get_task_by_name(TEE_TASK);
		// wait for TEE request ...
		switch_to_task(tee_task);
		procceed = 0;
		if (task->tee_rpc != NULL ) {
			TZ_TEE_SPACE* tee = (TZ_TEE_SPACE*) task->tee_rpc;
			switch (tee->op) {
			case TZ_TEE_OP_OPEN_SESSION:
				rpc->operation = TA_OPEN_SESSION;

				result = parameters_to_userspace(
						&tee->params.openSession.operation, rpc,
						get_current_task());

				if (result != TEEC_SUCCESS) {
					tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
					tee->ret = result;
					tee_error("Failed to copy parameters from userspace");
					break;
				}

				procceed = 1;

				break;
			case TZ_TEE_OP_CLOSE_SESSION:
				rpc->operation = TA_CLOSE_SESSION;

				session = tee_session_find(tee->params.closeSession.session);
				if (session == NULL ) {
					tee->ret = TEE_ERROR_GENERIC;
					tee_error("Failed to find session!");
					break;
				}

				rpc->sessionContext = (void*) session->tee_session_handle;
				procceed = 1;

				break;
			case TZ_TEE_OP_INVOKE_CMD:
				rpc->operation = TA_INVOKE;

				session = tee_session_find(tee->params.invokeCommand.session);
				if (session == NULL ) {
					tee->ret = TEE_ERROR_GENERIC;
					tee_error("Failed to find session!");
					break;
				}

				result = parameters_to_userspace(
						&tee->params.invokeCommand.operation, rpc,
						get_current_task());

				if (result != TEEC_SUCCESS) {
					tee->params.openSession.returnOrigin = TEE_ORIGIN_TEE;
					tee->ret = result;
					tee_error("Failed to copy parameters from userspace");
					break;
				}

				rpc->sessionContext = (void*) session->tee_session_handle;
				rpc->commandID = tee->params.invokeCommand.command;
				procceed = 1;
				break;
			}
		}
	} while (procceed == 0);
}
