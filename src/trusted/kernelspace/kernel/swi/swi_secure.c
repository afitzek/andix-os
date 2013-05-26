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

int parameters_to_userspace(TEECOM_Operation* comOp, TA_RPC *rpc, task_t* task) {
	int pidx = 0;
	uint32_t paramType = 0;
	uint32_t paddr;
	uint32_t size;
	uint32_t freed;
	uint8_t* v_mem_addr;
	tee_memory* memory = NULL;
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
	//uint32_t paddr;
	uint32_t size = 0;
	uint32_t freed;
	uint8_t* v_mem_addr;
	tee_memory* memory = NULL;

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
