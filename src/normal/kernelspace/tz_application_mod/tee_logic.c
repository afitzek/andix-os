/*
 * tee_logic.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_logic.h>
#include <communication_types.h>
#include <tz_application_mod/tee_operations.h>
#include <tee_client_api.h>
#include <tz_application_mod/smc.h>
#include <tz_application_mod/tee_mem.h>
#include <linux/sched.h>
#include <asm-generic/cacheflush.h>
#include <tz_application_mod/tee_mem.h>

int tee_init() {
	if (tee_context_init() != 0) {
		return (-1);
	}

	if (tee_session_init() != 0) {
		return (-1);
	}

	if (tee_memory_init() != 0) {
		return (-1);
	}

	printk(KERN_INFO "TEE RPC Events:\n");

	tee_ctx_init.pre = &tee_ctx_init_pre;
	tee_ctx_init.post = &tee_ctx_init_post;

	printk(KERN_INFO "tee_init: tee_ctx_init 0x%x\n",
			(unsigned int)&tee_ctx_init);
	printk(KERN_INFO "tee_init: tee_ctx_init->pre 0x%x\n",
			(unsigned int)tee_ctx_init.pre);
	printk(KERN_INFO "tee_init: tee_ctx_init->post 0x%x\n",
			(unsigned int)tee_ctx_init.post);

	tee_ctx_finalize.pre = &tee_ctx_finalize_pre;
	tee_ctx_finalize.post = &tee_ctx_finalize_post;

	printk(KERN_INFO "tee_init: tee_ctx_finalize 0x%x\n",
			(unsigned int)&tee_ctx_finalize);
	printk(KERN_INFO "tee_init: tee_ctx_finalize->pre 0x%x\n",
			(unsigned int)tee_ctx_finalize.pre);
	printk(KERN_INFO "tee_init: tee_ctx_finalize->post 0x%x\n",
			(unsigned int)tee_ctx_finalize.post);

	tee_mem_reg.pre = &tee_mem_reg_pre;
	tee_mem_reg.post = &tee_mem_reg_post;

	printk(KERN_INFO "tee_init: tee_mem_reg 0x%x\n",
			(unsigned int)&tee_mem_reg);
	printk(KERN_INFO "tee_init: tee_mem_reg->pre 0x%x\n",
			(unsigned int)tee_mem_reg.pre);
	printk(KERN_INFO "tee_init: tee_mem_reg->post 0x%x\n",
			(unsigned int)tee_mem_reg.post);

	tee_mem_rel.pre = &tee_mem_rel_pre;
	tee_mem_rel.post = &tee_mem_rel_post;

	printk(KERN_INFO "tee_init: tee_mem_rel 0x%x\n",
			(unsigned int)&tee_mem_rel);
	printk(KERN_INFO "tee_init: tee_mem_rel->pre 0x%x\n",
			(unsigned int)tee_mem_rel.pre);
	printk(KERN_INFO "tee_init: tee_mem_rel->post 0x%x\n",
			(unsigned int)tee_mem_rel.post);

	return (0);
}

int tz_process_ctrl_mem() {
	int result = 0;

	while (1) {

		// Push ctrl struct to userspace daemon
		push_ctrl_task_from_s();

		while (poll_ctrl_task_to_s() == 0) {
			// wait until ctrl task was proccessed
			schedule();
		}

		// CALL Monitor with CTRL mem response
		CP15DMB;
		CP15DSB;
		CP15ISB;
		result = __smc_1(SMC_PROCESS_CMEM, 0);

		if (result == TEE_STRUCT) {
			// is response is TEE break
			break;
		}
	}

	return (result);
}

int tee_process(TZ_TEE_SPACE* userspace) {
	int result = TEE_EVENT_RET_SUCCESS;
	int tz_result = 0;
	const op_event* event = NULL;
	TZ_TEE_SPACE localcopy;

	get_from_user(&localcopy, userspace);

	switch (localcopy.op) {
	case TZ_TEE_OP_INIT_CTX:
		event = &tee_ctx_init;
		break;
	case TZ_TEE_OP_FIN_CTX:
		event = &tee_ctx_finalize;
		break;
	case TZ_TEE_OP_REGISTER_MEM:
		event = &tee_mem_reg;
		break;
	case TZ_TEE_OP_RELEASE_MEM:
		event = &tee_mem_rel;
		break;
	}

	// Perform normal world operations
	if (event == NULL) {
		printk(KERN_ERR "tee_process: no event found\n");
		localcopy.ret = TEEC_ERROR_COMMUNICATION;
		goto err_ret;
	}

	printk(KERN_INFO "tee_process: event 0x%x\n", (unsigned int)event);
	printk(KERN_INFO "tee_process: event->pre 0x%x\n", (unsigned int)event->pre);
	printk(KERN_INFO "tee_process: event->post 0x%x\n", (unsigned int)event->post);

	if (event->pre != NULL) {
		result = event->pre(&localcopy);
	}

	if (result != TEE_EVENT_RET_SUCCESS) {
		printk(KERN_ERR "tee_process: pre event trigger failed!\n");
		localcopy.ret = TEEC_ERROR_COMMUNICATION;
		goto err_ret;
	}
	printk(KERN_INFO "LOCALCOPY\n");
	kprintHex(&localcopy, sizeof(TZ_TEE_SPACE));

	push_tee(&localcopy);

	// call TZ
	CP15DMB;
	CP15DSB;
	CP15ISB;
	//tee_flush_cache();
	flush_cache_all();
	printk(KERN_INFO "SHARED MEM\n");
	kprintHex(get_shared_tee_mem(), sizeof(TZ_TEE_SPACE));
	tz_result = __smc_1(SMC_PROCESS_TMEM, 0);
	tee_invalidate();
	printk(KERN_INFO "SHARED MEM\n");
	kprintHex(get_shared_tee_mem(), sizeof(TZ_TEE_SPACE));
	CP15DMB;
	CP15DSB;
	CP15ISB;

	poll_tee(&localcopy);

	//printk(KERN_ERR "tee_process: tx ctx id 0x%x\n",
	//		localcopy.params.initCtx.context);
	printk(KERN_INFO "LOCALCOPY\n");
	kprintHex(&localcopy, sizeof(TZ_TEE_SPACE));

	if (tz_result == CTRL_STRUCT) {
		tz_result = tz_process_ctrl_mem();
	}

	if (event->post != NULL) {
		result = event->post(&localcopy);
	}

	if (result != TEE_EVENT_RET_SUCCESS) {
		printk(KERN_ERR "tee_process: post event trigger failed!\n");
		localcopy.ret = TEEC_ERROR_COMMUNICATION;
		goto err_ret;
	}

	send_to_user(&localcopy, userspace);
	return (tz_result);
	err_ret: send_to_user(&localcopy, userspace);
	return (-1);
}
