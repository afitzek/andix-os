/*
 * tee_operations.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_operations.h>
#include <tee_client_api.h>
#include <tz_application_mod/tee_memory.h>

int tee_op_dummy(TZ_TEE_SPACE* com_mem) {
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_init_post(TZ_TEE_SPACE* com_mem) {
	tee_context* new_context = tee_context_add();
	if (new_context == NULL) {
		printk(KERN_ERR "tee_context_add: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}
	// save tz context id
	new_context->tz_id = com_mem->params.initCtx.context;

	// return kernel context id
	com_mem->params.initCtx.context = new_context->id;
	return (TEE_EVENT_RET_SUCCESS);
}

static const op_event tee_ctx_init = {
	.pre = &tee_op_dummy,
	.post = &tee_ctx_init_post
};

int tee_ctx_finalize_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;

	context = tee_context_find_by_id(com_mem->params.finCtx.context);

	if (context == NULL) {
		printk(KERN_ERR "tee_ctx_finalize_pre: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	// translate context id
	com_mem->params.finCtx.context = context->tz_id;
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_ctx_finalize_post(TZ_TEE_SPACE* com_mem) {
	tee_context* kill_context = NULL;
	tee_session* session = NULL;
	tee_shared_memory* mem = NULL;

	kill_context = tee_context_find_by_tzid(com_mem->params.finCtx.context);

	if (kill_context == NULL) {
		return (TEE_EVENT_RET_SUCCESS);
	}

	// Cleanup sessions
	session = tee_session_find_by_ctx(kill_context);
	while (session != NULL) {
		tee_session_free(session);
		session = tee_session_find_by_ctx(kill_context);
	}

	// Cleanup memories
	mem = tee_memory_find_by_ctx(kill_context);
	while (mem != NULL) {
		tee_memory_free(mem);
		mem = tee_memory_find_by_ctx(kill_context);
	}

	tee_context_free(kill_context);

	com_mem->params.finCtx.context = 0;

	return (TEE_EVENT_RET_SUCCESS);
}

static const op_event tee_ctx_finalize = {
	.pre = &tee_ctx_finalize_pre,
	.post = &tee_ctx_finalize_post
};

int tee_mem_reg_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	if (com_mem->params.regMem.size > TEEC_CONFIG_SHAREDMEM_MAX_SIZE) {
		printk(KERN_ERR "tee_mem_reg_pre: shared memory too big!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	context = tee_context_find_by_id(com_mem->params.regMem.context);

	if (context == NULL) {
		printk(KERN_ERR "tee_mem_reg_pre: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_add(context);

	if (mem == NULL) {
		printk(KERN_ERR "tee_mem_reg_post: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->user_addr = (void*)com_mem->params.regMem.paddr;
	mem->size = com_mem->params.regMem.size;
	mem->flags = com_mem->params.regMem.flags;
	mem->state = TEE_MEM_STATE_UNMAPPED;
	mem->com_vaddr = kmalloc(com_mem->params.regMem.size, GFP_KERNEL);

	if (mem->com_vaddr == NULL) {
		tee_memory_free(mem);
		printk(KERN_ERR "tee_mem_reg_post: Out of memory");
		com_mem->ret = TEEC_ERROR_OUT_OF_MEMORY;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->com_paddr = (void*)virt_to_phys(mem->com_vaddr);

	com_mem->params.regMem.paddr = (uint32_t)mem->com_paddr;

	// translate context id
	com_mem->params.regMem.context = context->tz_id;

	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_reg_post(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	context = tee_context_find_by_tzid(com_mem->params.regMem.context);

	if (context == NULL) {
		printk(KERN_ERR "tee_mem_reg_post: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_find_by_paddr((void*)com_mem->params.regMem.paddr);

	if (mem == NULL) {
		printk(KERN_ERR "tee_mem_reg_post: failed to find shared memory!");
		com_mem->ret = TEEC_ERROR_GENERIC;
		return (TEE_EVENT_RET_ERROR);
	}

	mem->state = TEE_MEM_STATE_READY;

	return (TEE_EVENT_RET_SUCCESS);
}

static const op_event tee_mem_reg = {
	.pre = &tee_mem_reg_pre,
	.post = &tee_mem_reg_post
};

int tee_mem_rel_pre(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	context = tee_context_find_by_id(com_mem->params.regMem.context);

	if (context == NULL) {
		printk(KERN_ERR "tee_mem_reg_post: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_find_by_id(com_mem->params.regMem.memid);

	if (mem == NULL) {
		printk(KERN_ERR "tee_mem_rel_pre: no such memory!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	// translate mem id and context id
	com_mem->params.regMem.memid = mem->tz_id;
	com_mem->params.regMem.context = context->tz_id;
	return (TEE_EVENT_RET_SUCCESS);
}

int tee_mem_rel_post(TZ_TEE_SPACE* com_mem) {
	tee_context* context = NULL;
	tee_shared_memory* mem = NULL;

	context = tee_context_find_by_tzid(com_mem->params.regMem.context);

	if (context == NULL) {
		printk(KERN_ERR "tee_mem_rel_post: no such context!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	mem = tee_memory_find_by_tzid(com_mem->params.regMem.memid);

	if (mem == NULL) {
		printk(KERN_ERR "tee_mem_rel_post: no such memory!");
		com_mem->ret = TEEC_ERROR_BAD_PARAMETERS;
		return (TEE_EVENT_RET_ERROR);
	}

	kfree(mem->com_vaddr);

	tee_memory_free(mem);

	// translate mem id and context id
	com_mem->params.regMem.memid = mem->id;
	com_mem->params.regMem.context = context->id;
	return (TEE_EVENT_RET_SUCCESS);
}

static const op_event tee_mem_rel = {
	.pre = &tee_mem_rel_pre,
	.post = &tee_mem_rel_post
};

int tee_session_open_pre(TZ_TEE_SPACE* com_mem) {
	return 0;
}

int tee_session_close(TZ_TEE_SPACE* com_mem) {
	return 0;
}
