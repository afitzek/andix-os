/*
 * tee_operations.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_operations.h>
#include <tee_client_api.h>
#include <tz_application_mod/tee_memory.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <tz_application_mod/shared_mem.h>
#include <client_error_constants.h>
#include <tz_application_mod/utils.h>

op_event tee_ctx_init;
op_event tee_ctx_finalize;
op_event tee_mem_reg;
op_event tee_mem_rel;
op_event tee_session_open;
op_event tee_session_close;
op_event tee_invoke;

int tee_op_dummy(TZ_TEE_SPACE* com_mem) {
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_init_pre(TZ_TEE_SPACE* com_mem) {
	printk(KERN_INFO "tee_ctx_init_pre: CTX Addr 0x%x",
			(unsigned int)&(com_mem->params.initCtx.context));
	com_mem->params.initCtx.context = 0xDEADC;
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_init_post(TZ_TEE_SPACE* com_mem) {
	tee_context* new_context = tee_context_add();
	if (new_context == NULL ) {
		printk(KERN_ERR "tee_context_add: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}
	// save tz context id
	new_context->tz_id = com_mem->params.initCtx.context;

	// return kernel context id
	com_mem->params.initCtx.context = new_context->id;

	printk(KERN_INFO "tee_ctx_init_post: TZ CTX ID 0x%x -> NORM CTX ID 0x%x",
			new_context->tz_id, new_context->id);

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_finalize_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;

	context = tee_context_find_by_id(com_mem->params.finCtx.context);

	if (context == NULL ) {
		printk(KERN_ERR "tee_ctx_finalize_pre: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	// translate context id
	com_mem->params.finCtx.context = context->tz_id;

	printk(KERN_INFO "tee_ctx_init_post: NORM CTX ID 0x%x -> TZ CTX ID 0x%x",
			context->id, com_mem->params.finCtx.context);

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_finalize_post(TZ_TEE_SPACE* com_mem) {
	tee_context* kill_context = NULL;
	tee_session* session = NULL;
	tee_shared_memory* mem = NULL;

	kill_context = tee_context_find_by_tzid(com_mem->params.finCtx.context);

	if (kill_context == NULL ) {
		return (TEE_EVENT_RET_SUCCESS);
	}

	// Cleanup sessions
	session = tee_session_find_by_ctx(kill_context);
	while (session != NULL ) {
		tee_session_free(session);
		session = tee_session_find_by_ctx(kill_context);
	}

	// Cleanup memories
	mem = tee_memory_find_by_ctx(kill_context);
	while (mem != NULL ) {
		tee_memory_free(mem);
		mem = tee_memory_find_by_ctx(kill_context);
	}

	tee_context_free(kill_context);

	com_mem->params.finCtx.context = 0;

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_reg_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	if (com_mem->params.regMem.size > TEEC_CONFIG_SHAREDMEM_MAX_SIZE) {
		printk(KERN_ERR "tee_mem_reg_pre: shared memory too big!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	context = tee_context_find_by_id(com_mem->params.regMem.context);

	if (context == NULL ) {
		printk(KERN_ERR "tee_mem_reg_pre: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_add(context);

	if (mem == NULL ) {
		printk(KERN_ERR "tee_mem_reg_post: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->user_addr = (void*) com_mem->params.regMem.paddr;
	mem->size = com_mem->params.regMem.size;
	mem->flags = com_mem->params.regMem.flags;
	mem->state = TEE_MEM_STATE_UNMAPPED;
	mem->com_vaddr = kmalloc(com_mem->params.regMem.size, GFP_KERNEL);

	if (mem->com_vaddr == NULL ) {
		tee_memory_free(mem);
		printk(KERN_ERR "tee_mem_reg_post: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->com_paddr = (void*) virt_to_phys(mem->com_vaddr);

	com_mem->params.regMem.paddr = (uint32_t) mem->com_paddr;

	// translate context id
	com_mem->params.regMem.context = context->tz_id;

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_reg_post(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	context = tee_context_find_by_tzid(com_mem->params.regMem.context);

	if (context == NULL ) {
		printk(KERN_ERR "tee_mem_reg_post: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_find_by_paddr((void*) com_mem->params.regMem.paddr);

	if (mem == NULL ) {
		printk(KERN_ERR "tee_mem_reg_post: failed to find shared memory!");
		com_mem->ret = TEEC_ERROR_GENERIC;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->state = TEE_MEM_STATE_READY;
	mem->tz_id = com_mem->params.regMem.memid;
	com_mem->params.regMem.memid = mem->id;

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_rel_pre(TZ_TEE_SPACE* com_mem) {
	//tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	// Context is not available!!
	/*context = tee_context_find_by_id(com_mem->params.regMem.context);

	 if (context == NULL) {
	 printk(KERN_ERR "tee_mem_reg_post: no such context!");
	 com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
	 return (TEE_EVENT_RET_ERROR);
	 }*/

	mem = tee_memory_find_by_id(com_mem->params.regMem.memid);

	if (mem == NULL ) {
		printk(KERN_ERR "tee_mem_rel_pre: no such memory!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	if (mem->user_addr != (void*) com_mem->params.regMem.paddr) {
		printk(KERN_ERR "tee_mem_rel_pre: Memory ID not matching address");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}
	if (mem->size != com_mem->params.regMem.size) {
		printk(KERN_ERR "tee_mem_rel_pre: Memory ID not matching size");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}
	if (mem->flags != com_mem->params.regMem.flags) {
		printk(KERN_ERR "tee_mem_rel_pre: Memory ID not matching flags");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	// translate mem id and context id
	com_mem->params.regMem.memid = mem->tz_id;
	//com_mem->params.regMem.context = context->tz_id;
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_rel_post(TZ_TEE_SPACE* com_mem) {
	//tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	// Context is not available!!
	/*context = tee_context_find_by_tzid(com_mem->params.regMem.context);

	 if (context == NULL) {
	 printk(KERN_ERR "tee_mem_rel_post: no such context!");
	 com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
	 return (TEE_EVENT_RET_ERROR);
	 }*/

	mem = tee_memory_find_by_tzid(com_mem->params.regMem.memid);

	if (mem == NULL ) {
		printk(KERN_ERR "tee_mem_rel_post: no such memory!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	kfree(mem->com_vaddr);

	tee_memory_free(mem);

	// translate mem id and context id
	com_mem->params.regMem.memid = mem->id;
	//com_mem->params.regMem.context = context->id;
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_translate_parameter_to_tz(TEECOM_Operation* operation, uint32_t *result) {
	int pidx = 0;
	uint32_t paramType;
	tee_shared_memory* mem;
	for (pidx = 0; pidx < 4; pidx++) {
		paramType = TEE_PARAM_TYPE_GET(operation->paramTypes, pidx);
		switch (paramType) {
		case TEEC_VALUE_INOUT:
			printk(KERN_INFO "VALUE PARAM A 0x%x B 0x%x",
					operation->params[pidx].value.a,
					operation->params[pidx].value.b);
			break;
		case TEEC_MEMREF_PARTIAL_INOUT:
		case TEEC_MEMREF_PARTIAL_INPUT:
		case TEEC_MEMREF_PARTIAL_OUTPUT:
		case TEEC_MEMREF_WHOLE:
			mem = tee_memory_find_by_id(operation->params[pidx].memref.memid);
			if (mem == NULL ) {
				printk(KERN_ERR "tee_translate_parameter_to_tz: no such memory! %x",
						operation->params[pidx].memref.memid);
				(*result) = TEEC_ERROR_BAD_PARAMETERS;
				return (TEE_EVENT_RET_ERROR);
			}
			if (copy_from_user(mem->com_vaddr, mem->user_addr, mem->size)
					!= 0) {
				printk(KERN_ERR "tee_translate_parameter_to_tz: "
						"failed to copy from user");
				(*result) = TEEC_ERROR_COMMUNICATION;
				return (TEE_EVENT_RET_ERROR);
			}
			invalidate_clean(mem->com_vaddr, mem->size);
			operation->params[pidx].memref.memid = mem->tz_id;
			break;
		}
	}
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_translate_parameter_from_tz(TEECOM_Operation* operation,
		uint32_t *result) {
	int pidx = 0;
	uint32_t paramType;
	tee_shared_memory* mem;
	for (pidx = 0; pidx < 4; pidx++) {
		paramType = TEE_PARAM_TYPE_GET(operation->paramTypes, pidx);
		switch (paramType) {
		case TEEC_MEMREF_PARTIAL_INOUT:
		case TEEC_MEMREF_PARTIAL_INPUT:
		case TEEC_MEMREF_PARTIAL_OUTPUT:
		case TEEC_MEMREF_WHOLE:
			mem = tee_memory_find_by_tzid(operation->params[pidx].memref.memid);
			if (mem == NULL ) {
				printk(KERN_ERR "tee_translate_parameter_from_tz: no such memory! %x",
						operation->params[pidx].memref.memid);
				(*result) = TEEC_ERROR_BAD_PARAMETERS;
				return (TEE_EVENT_RET_ERROR);
			}
			invalidate(mem->com_vaddr, mem->size);
			if (copy_to_user(mem->user_addr, mem->com_vaddr, mem->size) != 0) {
				printk(KERN_ERR "tee_translate_parameter_from_tz: "
						"failed to copy to user");
				(*result) = TEEC_ERROR_COMMUNICATION;
				return (TEE_EVENT_RET_ERROR);
			}
			operation->params[pidx].memref.memid = mem->id;
			break;
		}
	}
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_session_open_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_session* session = NULL;

	context = tee_context_find_by_id(com_mem->params.openSession.context);

	if (context == NULL ) {
		printk(KERN_ERR "tee_session_open_pre: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	session = tee_session_add(context);

	if (session == NULL ) {
		printk(KERN_ERR "tee_session_open_pre: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	com_mem->params.openSession.tmpid = session->id;
	com_mem->params.openSession.context = context->tz_id;

	if (com_mem->params.openSession.operation.valid == 1) {
		return (tee_translate_parameter_to_tz(
				&com_mem->params.openSession.operation, &com_mem->ret));
	}

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_session_open_post(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_session* session = NULL;

	context = tee_context_find_by_tzid(com_mem->params.openSession.context);

	if (context == NULL ) {
		printk(KERN_ERR "tee_session_open_post: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	session = tee_session_find_by_id(com_mem->params.openSession.tmpid);

	if (session == NULL ) {
		printk(KERN_ERR "tee_session_open_post: invalid session id");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	session->tz_id = com_mem->params.openSession.session;
	com_mem->params.openSession.session = session->id;
	com_mem->params.openSession.context = context->id;

	printk(KERN_INFO "tee_session_open_post: NORM CTX ID 0x%x -> TZ CTX ID 0x%x",
			session->id, session->tz_id);

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_session_close_pre(TZ_TEE_SPACE* com_mem) {
	tee_session* session = NULL;

	session = tee_session_find_by_id(com_mem->params.closeSession.session);

	if (session == NULL ) {
		printk(KERN_ERR "tee_session_close_pre: invalid session id 0x%x",
				com_mem->params.closeSession.session);
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	com_mem->params.closeSession.session = session->tz_id;

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_session_close_post(TZ_TEE_SPACE* com_mem) {
	tee_session* session = NULL;

	session = tee_session_find_by_tzid(com_mem->params.closeSession.session);

	if (session == NULL ) {
		printk(KERN_ERR "tee_session_close_post: invalid session id0x%x",
				com_mem->params.closeSession.session);
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	com_mem->params.closeSession.session = session->id;

	tee_session_free(session);

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_invoke_pre(TZ_TEE_SPACE* com_mem) {
	tee_session* session = NULL;

	session = tee_session_find_by_id(com_mem->params.invokeCommand.session);

	if (session == NULL ) {
		printk(KERN_ERR "tee_invoke_pre: no such session! %x",
				com_mem->params.invokeCommand.session);
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	com_mem->params.invokeCommand.session = session->tz_id;

	return (tee_translate_parameter_to_tz(
			&com_mem->params.invokeCommand.operation, &com_mem->ret));
}

int tee_invoke_post(TZ_TEE_SPACE* com_mem) {
	tee_session* session = NULL;

	session = tee_session_find_by_tzid(com_mem->params.invokeCommand.session);

	if (session == NULL ) {
		printk(KERN_ERR "tee_invoke_post: invalid session id %x",
				com_mem->params.invokeCommand.session);
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	com_mem->params.invokeCommand.session = session->id;

	return (tee_translate_parameter_from_tz(
			&com_mem->params.invokeCommand.operation, &com_mem->ret));
}

