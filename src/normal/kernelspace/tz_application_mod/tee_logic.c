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
#include <tz_application_mod/shared_mem.h>

void print_event_info(op_event* event, const char* name) {
printk(KERN_INFO " TEE Event: %s 0x%x\n",
		name, (unsigned int)event);
printk(KERN_INFO " TEE Event (PRE): %s->pre 0x%x\n",
		name, (unsigned int)event->pre);
printk(KERN_INFO " TEE Event (POST): %s->post 0x%x\n",
		name, (unsigned int)event->post);
}

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

print_event_info(&tee_ctx_init, "tee_ctx_init");

tee_ctx_finalize.pre = &tee_ctx_finalize_pre;
tee_ctx_finalize.post = &tee_ctx_finalize_post;

print_event_info(&tee_ctx_finalize, "tee_ctx_finalize");

tee_mem_reg.pre = &tee_mem_reg_pre;
tee_mem_reg.post = &tee_mem_reg_post;

print_event_info(&tee_mem_reg, "tee_mem_reg");

tee_mem_rel.pre = &tee_mem_rel_pre;
tee_mem_rel.post = &tee_mem_rel_post;

print_event_info(&tee_mem_rel, "tee_mem_rel");

tee_session_open.pre = &tee_session_open_pre;
tee_session_open.post = &tee_session_open_post;

print_event_info(&tee_session_open, "tee_session_open");

tee_session_close.pre = &tee_session_close_pre;
tee_session_close.post = &tee_session_close_post;

print_event_info(&tee_session_close, "tee_session_close");

tee_invoke.pre = &tee_invoke_pre;
tee_invoke.post = &tee_invoke_post;

print_event_info(&tee_invoke, "tee_invoke");

return (0);
}

int tz_process_ctrl_mem(TZ_TEE_SPACE* teespace, TZ_CTLR_SPACE* ctrlspace) {
int result = 0;
uint32_t ctrl_physical = virt_to_phys((void*) ctrlspace);
uint32_t tee_physical = virt_to_phys((void*) teespace);
while (1) {

	// Push ctrl struct to userspace daemon
	//push_ctrl_task_from_s();

	while (poll_ctrl_task_to_s() == 0) {
		// wait until ctrl task was proccessed
		schedule();
	}

	// CALL Monitor with CTRL mem response
	CP15DMB;
	CP15DSB;
	CP15ISB;
	result = __smc_2(SMC_PROCESS_CMEM, tee_physical, ctrl_physical);

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
struct page* tee_pages;
struct page* ctrl_pages;
TZ_TEE_SPACE *teemem;
TZ_CTLR_SPACE *ctrlspace;
void* ctrl_physical;
void* tee_physical;

tee_pages = allocate_mapable_memory(sizeof(TZ_TEE_SPACE), &tee_physical,
		&teemem);

if (tee_pages == NULL) {
	printk(KERN_ERR "tee_process: out of memory!\n");
	return (-1);
}

ctrl_pages = allocate_mapable_memory(sizeof(TZ_CTLR_SPACE), &ctrl_physical,
		&ctrlspace);

if (ctrl_pages == NULL) {
	free_mapable_memory(tee_pages, sizeof(TZ_TEE_SPACE));
	printk(KERN_ERR "tee_process: out of memory!\n");
	return (-1);
}

memset(ctrlspace, 0, sizeof(TZ_CTLR_SPACE));
memset(teemem, 0, sizeof(TZ_TEE_SPACE));

get_from_user(teemem, userspace);

switch (teemem->op) {
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
case TZ_TEE_OP_OPEN_SESSION:
	event = &tee_session_open;
	break;
case TZ_TEE_OP_CLOSE_SESSION:
	event = &tee_session_close;
	break;
case TZ_TEE_OP_INVOKE_CMD:
	event = &tee_invoke;
	break;
}

// Perform normal world operations
if (event == NULL) {
	printk(KERN_ERR "tee_process: no event found\n");
	teemem->ret = TEEC_ERROR_COMMUNICATION;
	goto err_ret;
}

printk(KERN_INFO "tee_process: event 0x%x\n", (unsigned int)event);
printk(KERN_INFO "tee_process: event->pre 0x%x\n", (unsigned int)event->pre);
printk(KERN_INFO "tee_process: event->post 0x%x\n", (unsigned int)event->post);

if (event->pre != NULL) {
	result = event->pre(teemem);
}

if (result != TEE_EVENT_RET_SUCCESS) {
	printk(KERN_ERR "tee_process: pre event trigger failed!\n");
	teemem->ret = TEEC_ERROR_COMMUNICATION;
	goto err_ret;
}
//printk(KERN_INFO "LOCALCOPY\n");
//kprintHex(teemem, sizeof(TZ_TEE_SPACE));

//push_tee(&localcopy);

// call TZ
CP15DMB;
CP15DSB;
CP15ISB;
//flush_cache(tee_pages);
//flush_cache(ctrl_pages);
flush_cache_all();
invalidate_clean(teemem, sizeof(TZ_TEE_SPACE));
//printk(KERN_INFO "SHARED MEM\n");
//kprintHex(get_shared_tee_mem(), sizeof(TZ_TEE_SPACE));
tz_result = __smc_2(SMC_PROCESS_TMEM, (uint32_t) tee_physical,
		(uint32_t) ctrl_physical);
invalidate(teemem, sizeof(TZ_TEE_SPACE));
//tee_invalidate(get_shared_tee_mem(), sizeof(TZ_TEE_SPACE));
//printk(KERN_INFO "SHARED MEM\n");
//kprintHex(get_shared_tee_mem(), sizeof(TZ_TEE_SPACE));
CP15DMB;
CP15DSB;
CP15ISB;

//poll_tee(&localcopy);

//printk(KERN_ERR "tee_process: tx ctx id 0x%x\n",
//		localcopy.params.initCtx.context);
//printk(KERN_INFO "LOCALCOPY\n");
//kprintHex(teemem, sizeof(TZ_TEE_SPACE));

if (tz_result == CTRL_STRUCT) {
	tz_result = tz_process_ctrl_mem(teemem, ctrlspace);
}

if (event->post != NULL) {
	result = event->post(teemem);
}

if (result != TEE_EVENT_RET_SUCCESS) {
	printk(KERN_ERR "tee_process: post event trigger failed!\n");
	teemem->ret = TEEC_ERROR_COMMUNICATION;
	goto err_ret;
}

free_mapable_memory(tee_pages, sizeof(TZ_TEE_SPACE));
free_mapable_memory(ctrl_pages, sizeof(TZ_CTLR_SPACE));
send_to_user(teemem, userspace);
return (tz_result);
err_ret: free_mapable_memory(ctrl_pages, sizeof(TZ_CTLR_SPACE));
free_mapable_memory(tee_pages, sizeof(TZ_TEE_SPACE));
send_to_user(teemem, userspace);
return (-1);
}
