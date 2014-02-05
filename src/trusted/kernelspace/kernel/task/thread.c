/**
 * @file thread.c
 * @brief
 *  Created on: Jan 22, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */


#include <task/thread.h>
#include <scheduler.h>
#include <mm/mm.h>

static uint32_t current_tid = 1;

struct thread_t *create_kernel_thread(uint32_t mode, EXEC_CONTEXT_t world) {
	struct thread_t *thread = (struct thread_t*) kmalloc(sizeof(struct thread_t));

	if (thread == NULL ) {
		task_error("Failed to allocate memory for new thread!");
		return thread;
	}

	// clear all
	memset(thread, 0, sizeof(struct thread_t));

	if (world == SECURE) {
		thread->context.scr = SCR_EA;
		// we need a stack if we run in the secure context
		thread->kernel_stack.base = (uint32_t) map_stack_mem(KERNEL_STACK_SIZE);
		thread->kernel_stack.sp = thread->kernel_stack.base;
		thread->kernel_stack.limit = thread->kernel_stack.base - KERNEL_STACK_SIZE;
	} else {
		thread->context.scr = SCR_NS | SCR_EA | SCR_FW | SCR_FIQ | SCR_AW; //
	}

	thread->tid = ++current_tid;
	thread->exec_context = world;
	thread->context.cpsr = mode | PSR_F | PSR_I | PSR_A;
	thread->state = READY;

	thread->sys_context.svc_spsr = mode | PSR_F | PSR_I | PSR_A;
	thread->sys_context.abt_spsr = mode | PSR_F | PSR_I | PSR_A;
	thread->sys_context.und_spsr = mode | PSR_F | PSR_I | PSR_A;
	thread->sys_context.irq_spsr = mode | PSR_F | PSR_I | PSR_A;
	thread->sys_context.fiq_spsr = mode | PSR_F | PSR_I | PSR_A;
	thread->sys_context.abt_sp = getABTSP();
	// TODO: create thread mode stack pointer ....

	task_debug("new thread created: %d @ 0x%x", thread->tid, thread);

	return thread;
}

// TODO deprecated
//void thread_set_name(struct thread_t *thread, const char *name) {
//	thread->name = name;
//}

void thread_set_entry(struct thread_t *thread, void *entry) {
	thread->context.pc = entry;
}

char* thread_state_str(thread_state_t state) {
	switch (state) {
	case READY:
		return "READY";
	case RUNNING:
		return "RUNNING";
	case BLOCKED:
		return "BLOCKED";
	case SLEEPING:
		return "SLEEPING";
	case DEAD:
		return "DEAD";
	default:
		return "UNKOWN";
	}
}

int print_thread(struct thread_t *thread, void *none) {
	if (thread == NULL)
		return -1;
	task_info("T %d [%d - (%s)]", thread->tid, thread->state, thread_state_str(thread->state));
	if (thread->process) {
		task_info("\tP %d %s", thread->process->pid, thread->process->name);
		TASK_UUID *uuid = &thread->process->tee_context.uuid;
		if (!is_uuid_empty(uuid)) {
			task_info("\tUUID %x-%x-%x-%x%x-%x%x%x%x%x%x", uuid->timeLow, uuid->timeMid, uuid->timeHiAndVersion,
					uuid->clockSeqAndNode[7], uuid->clockSeqAndNode[6], uuid->clockSeqAndNode[5], uuid->clockSeqAndNode[4],
					uuid->clockSeqAndNode[3], uuid->clockSeqAndNode[2], uuid->clockSeqAndNode[1], uuid->clockSeqAndNode[0]);
		}
	}
	return 0;
}

