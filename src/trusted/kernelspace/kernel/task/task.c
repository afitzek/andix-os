/*
 * task.c
 *
 *  Created on: Nov 26, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <task/task.h>
#include <mm/mm.h>

uint32_t next_tid = 0;

task_t* current_task = NULL;
task_t* idle_task = NULL;
task_t* nonsecure_task;
task_t* sysd_task;

list* task_list = NULL;

void nonsecure_set_cp_access(uint32_t cp) {
	cp &= 0x000FFFFF;

	asm volatile ("mcr p15, 0, %[cp], c1, c1, 2\n"
			: : [cp]"r"(cp) : "memory");
}

void set_uuid_emtpy(TASK_UUID *uuid) {
	uuid->timeHiAndVersion = 0;
	uuid->timeMid = 0;
	uuid->timeLow = 0;
	for (int i = 0; i < sizeof(uuid->clockSeqAndNode); i++) {
		uuid->clockSeqAndNode[i] = 0;
	}
}

uint8_t match_uuids(TASK_UUID *a, TASK_UUID *b) {
	if (a->timeHiAndVersion != b->timeHiAndVersion) {
		return (0);
	}
	if (a->timeMid != b->timeMid) {
		return (0);
	}
	if (a->timeLow != b->timeLow) {
		return (0);
	}
	for (int i = 0; i < sizeof(a->clockSeqAndNode); i++) {
		if (a->clockSeqAndNode[i] != b->clockSeqAndNode[i]) {
			return (0);
		}
	}

	return (1);
}

void cpy_uuid(TASK_UUID *dst, TASK_UUID *src) {
	dst->timeHiAndVersion = src->timeHiAndVersion;
	dst->timeMid = src->timeMid;
	dst->timeLow = src->timeLow;
	for (int i = 0; i < sizeof(dst->clockSeqAndNode); i++) {
		dst->clockSeqAndNode[i] = src->clockSeqAndNode[i];
	}
}

uint8_t is_uuid_empty(TASK_UUID *uuid) {

	if (uuid->timeHiAndVersion != 0) {
		return (0);
	}

	if (uuid->timeMid != 0) {
		return (0);
	}

	if (uuid->timeLow != 0) {
		return (0);
	}

	for (int i = 0; i < sizeof(uuid->clockSeqAndNode); i++) {
		if (uuid->clockSeqAndNode[i] != 0) {
			return (0);
		}
	}

	return (1);
}

void init_task() {
	next_tid = 0;
	current_task = NULL;
	nonsecure_task = NULL;
	sysd_task = NULL;
	task_list = (list*) kmalloc(sizeof(list));
	task_list->next = task_list;
	task_list->prev = task_list;
	task_list->data = NULL;
}

void add_task(task_t* task) {
	list_add(task_list, (uintptr_t) task);
	//list_dump(task_list);
}

void rem_task(task_t* task) {
	list* entry = list_find_data(task_list, (uintptr_t) task);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

task_t* get_task_by_uuid(TASK_UUID* uuid) {
	list* pos;
	list* next;
	task_t* task;
	if (is_uuid_empty(uuid)) {
		return (NULL );
	}
	list_for_each_safe(pos, next, task_list)
	{
		if (pos->data != NULL ) {
			task = (task_t*) pos->data;
			if (match_uuids(&(task->uuid), uuid)) {
				break;
			}
			task = NULL;
		}
	}

	return (task);
}

task_t* get_task_by_id(tid_t tid) {
	list* pos;
	list* next;
	task_t* task;
	list_for_each_safe(pos, next, task_list)
	{
		if (pos->data != NULL ) {
			task = (task_t*) pos->data;
			if (task != NULL && task->tid == tid) {
				break;
			}
			task = NULL;
		}
	}

	return (task);
}

task_t* get_task_by_name(char* name) {
	list* pos;
	list* next;
	task_t* task;
	list_for_each_safe(pos, next, task_list)
	{
		if (pos->data != NULL ) {
			task = (task_t*) pos->data;
			if (task != NULL
					&& strncmp(task->name, name, strlen(task->name)) == 0) {
				break;
			}
			task = NULL;
		}
	}

	return (task);
}

task_t* get_current_task() {
	return (current_task);
}

tid_t get_current_task_id() {
	return (current_task->tid);
}

void set_current_task(task_t* task) {
	if (current_task != NULL ) {
		if (current_task->state == RUNNING) {
			current_task->state = READY;
		}
	}
	current_task = task;
	task->state = RUNNING;
}

task_t* create_kernel_task(uint32_t mode, EXEC_CONTEXT_t context) {
	task_t* task = (task_t*) kmalloc(sizeof(task_t));

	if (task == NULL ) {
		task_error("Failed to allocate memory for task structure!");
		return (task);
	}

	memset(task, 0, sizeof(task_t));

	if (context == SECURE) {
		task->context.scr = SCR_EA;
		// we need a stack if we run in the secure context
		task->kernelSP = (uint32_t) map_stack_mem(0x4000);
		task->kernelStartSP = task->kernelSP;
	} else {
		task->context.scr = SCR_NS | SCR_EA | SCR_FIQ | SCR_FW;
	}

	// Clear standard registers
	for (int n = 0; n < 13; ++n) {
		task->context.r[n] = 0;
	}

	task->exec_context = context;
	task->tid = next_tid;
	next_tid++;
	task->context.cpsr = mode | PSR_F | PSR_I | PSR_A;
	task->state = READY;
	task->files = (list*) kmalloc(sizeof(list));
	task->files->next = task->files;
	task->files->prev = task->files;
	task->files->data = NULL;
	task->tee_rpc = NULL;

	task->sys_context.svc_spsr = mode | PSR_F | PSR_I | PSR_A;
	task->sys_context.abt_spsr = mode | PSR_F | PSR_I | PSR_A;
	task->sys_context.und_spsr = mode | PSR_F | PSR_I | PSR_A;
	task->sys_context.irq_spsr = mode | PSR_F | PSR_I | PSR_A;
	task->sys_context.fiq_spsr = mode | PSR_F | PSR_I | PSR_A;
	task->sys_context.abt_sp = getABTSP();
	// TODO: create task mode stack pointer ....

	task_info("new task created: %d @ 0x%x", task->tid, task);

	return (task);
}

void task_add_fhandle(task_t* task, task_file_handle_t* hdl) {
	list_add(task->files, (uintptr_t) hdl);
}

void task_rem_fhandle(task_t* task, task_file_handle_t* hdl) {
	list* entry = list_find_data(task->files, (uintptr_t) hdl);
	if (entry != NULL ) {
		list_remove(entry);
	}
}

task_file_handle_t* task_get_fhandle(task_t* task, int32_t fd) {
	list* pos;
	list* next;
	task_file_handle_t * hdl;
	list_for_each_safe(pos, next, task->files)
	{
		if (pos->data != NULL ) {
			hdl = (task_file_handle_t*) pos->data;
			if (hdl != NULL && hdl->user_fd == fd) {
				break;
			}
			hdl = NULL;
		}
	}

	return (hdl);
}

int32_t task_get_next_fd(task_t* task) {
	list* pos;
	list* next;
	int32_t fd = 3;
	task_file_handle_t * hdl;
	for (fd = 3; fd < 0xFFFF; fd++) {
		int32_t found = 0;
		list_for_each_safe(pos, next, task->files)
		{
			if (pos->data != NULL ) {
				hdl = (task_file_handle_t*) pos->data;
				if (hdl != NULL && hdl->user_fd == fd) {
					found = 1;
					break;
				}
				hdl = NULL;
			}
		}

		if (found == 0) {
			break;
		}
	}
	return (fd);
}

void task_set_name(task_t* task, char* name) {
	if (strlen(name) < TASKNAME_SIZE) {
		strncpy(task->name, name, TASKNAME_SIZE);
		task_info("task [%d] named: %s", task->tid, task->name);
	}
}

void set_idle_task(task_t* task) {
	idle_task = task;
}

task_t* get_idle_task() {
	return (idle_task);
}

void set_nonsecure_task(task_t* task) {
	nonsecure_task = task;
}

task_t* get_nonsecure_task() {
	return (nonsecure_task);
}

void set_sysd_task(task_t* task) {
	sysd_task = task;
}

task_t* get_sysd_task() {
	return (sysd_task);
}

task_t* task_get_next_ready() {
	list* pos;
	list* next;
	list* start = NULL;
	task_t* task;
	task_t* curr = get_current_task();
	if (curr == NULL ) {
		list_for_each_safe(pos, next, task_list)
		{
			if (pos->data != NULL ) {
				task = (task_t*) pos->data;
				if (task != NULL && task->tid == curr->tid) {
					start = pos;
					break;
				}
				task = NULL;
			}
		}
		task = NULL;

		if (start == NULL ) {
			list_for_each_safe(pos, next, task_list)
			{
				if (pos->data != NULL ) {
					task = (task_t*) pos->data;
					if (task != NULL && task->state == READY) {
						break;
					}
					task = NULL;
				}
			}
		} else {
			list_for_each_safe(pos, next, start)
			{
				if (pos->data != NULL ) {
					task = (task_t*) pos->data;
					if (task != NULL && task->state == READY) {
						break;
					}
					task = NULL;
				}
			}
		}
	} else {
		list_for_each_safe(pos, next, task_list)
		{
			if (pos->data != NULL ) {
				task = (task_t*) pos->data;
				if (task != NULL && task->state == READY) {
					break;
				}
				task = NULL;
			}
		}
	}

	if (task == NULL ) {
		task = get_idle_task();
	}

	return (task);
}

char* task_get_state_name(TASK_STATE state) {
	switch (state) {
	case READY:
		return ("READY");
	case RUNNING:
		return ("RUNNING");
	case BLOCKED:
		return ("BLOCKED");
	default:
		return ("UNKOWN");
	}
}

void print_tasks() {
	list* pos;
	list* next;
	task_t* task;
	task_info("Available Tasks:");

	list_for_each_safe(pos, next, task_list)
	{
		if (pos->data != NULL ) {
			task = (task_t*) pos->data;
			if (is_uuid_empty(&(task->uuid))) {
				task_info(
						"%d [%d - (%s)]: %s", task->tid, task->state, task_get_state_name(task->state), task->name);
			} else {
				task_info(
						"%d [%d - (%s)]: %s | UUID %x-%x-%x-%x%x-%x%x%x%x%x%x", task->tid, task->state, task_get_state_name(task->state), task->name, task->uuid.timeLow, task->uuid.timeMid, task->uuid.timeHiAndVersion, task->uuid.clockSeqAndNode[0], task->uuid.clockSeqAndNode[1], task->uuid.clockSeqAndNode[2], task->uuid.clockSeqAndNode[3], task->uuid.clockSeqAndNode[4], task->uuid.clockSeqAndNode[5], task->uuid.clockSeqAndNode[6], task->uuid.clockSeqAndNode[7]);
			}
			task = NULL;
		} else {
			task_error("NULL TASK!!");
		}
	}
	task_info("-------------");
}
