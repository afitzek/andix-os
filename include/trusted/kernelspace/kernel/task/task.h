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
 * task.h
 *
 *  Created on: Nov 26, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TASK_H_
#define TASK_H_

#include <monitor/monitor.h>
#include <common.h>

/**
 * \addtogroup task
 *	\{
 */

#define TASKNAME_SIZE 50

#define PROCESS_TASK "NS_PROCESSOR"
#define SERVICE_TASK "NS_SERVICE"
#define TEE_TASK "TEE_TASK"

typedef enum {
	SECURE,
	NONSECURE
} EXEC_CONTEXT_t;

typedef enum {
	RUNNING,
	READY,
	BLOCKED
} TASK_STATE;

typedef enum {
	NEW,
	CREATING,
	CREATED,
	PERFORMING,
	DESTROYING,
	DESTROYED
} TRUSTLET_STATE;

#define UUID_STR_LEN 40

typedef struct
{
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
}
TASK_UUID;

typedef struct {
	mon_context_t 		context;
	mon_sys_context_t 	sys_context;
	char 				name[TASKNAME_SIZE];
	tid_t				tid; // TASK ID
	EXEC_CONTEXT_t		exec_context;
	TASK_STATE			state;
	TASK_UUID			uuid;
	TRUSTLET_STATE		trustlet_state;
	uint32_t			membitmap[800];
	list				*files;
	// Kernel stuff
	uint32_t			kernelStartSP;
	uint32_t			kernelSP;
	// Userspace stuff
	uint32_t			userSP;
	uintptr_t			userPD;
	uintptr_t			vuserPD;
	uintptr_t			vheap;
	uintptr_t			tee_rpc;
} task_t;

typedef struct {
	int32_t 			user_fd;
	void* 				data;
} task_file_handle_t;

#define USER_START_MAPPED_MEM 0x70001000

/**
 * Entry point of idle task
 */
void idle_task_entry();

/**
 * Entrypoint for main kernel task
 */
void entry_main_task();

/**
 * Initialize task system
 */
void init_task();

/**
 * Get current task
 */
task_t* get_current_task();

/**
 * Get current task id
 */
tid_t get_current_task_id();

/**
 * Sets current task
 */
void set_current_task(task_t* task);

/**
 * Creates new kernel task
 */
task_t* create_kernel_task(uint32_t mode, EXEC_CONTEXT_t context);

/**
 * Sets nonsecure cp access
 */
void nonsecure_set_cp_access(uint32_t cp);

/**
 * Sets the name of a task
 */
void task_set_name(task_t* task, char* name);

/**
 * Parse UUID from in to uuid
 */
int uuid_parse(const char *in, TASK_UUID* uuid);

/**
 * Sets the idle task
 */
void set_idle_task(task_t* task);

/**
 * Early task initailization
 */
void init_early_task();

/**
 * Sets the nonsecure task
 */
void set_nonsecure_task(task_t* task);

/**
 * Gets the nonsecure task
 */
task_t* get_nonsecure_task();

/**
 * Add task to task lsit
 */
void add_task(task_t* task);

/**
 * Remove task from task list
 */
void rem_task(task_t* task);

/**
 * Get task by id
 */
task_t* get_task_by_id(tid_t tid);

/**
 * Get task by name
 */
task_t* get_task_by_name(char* name);

/**
 * Get task by uuid
 */
task_t* get_task_by_uuid(TASK_UUID* uuid);

/**
 * Gets next ready task
 */
task_t* task_get_next_ready();

/**
 * Sets uuid empty
 */
void set_uuid_emtpy(TASK_UUID *uuid);
/**
 * Copies UUID
 */
void cpy_uuid(TASK_UUID *dst, TASK_UUID *src);

/**
 * Checks if uuid is empty
 */
uint8_t is_uuid_empty(TASK_UUID *uuid);

/**
 * Match uuids
 */
uint8_t match_uuids(TASK_UUID *a, TASK_UUID *b);

/**
 * Print available tasks
 */
void print_tasks();

/**
 * Initialize a new userspace task
 */
void init_userspace_task(task_t* task);

/**
 * Prepares userspace task from elf memory block
 * @param start The start address
 * @param end The end address
 * @param name THe name of the new task
 * @param uuid_string The uuid of the new task
 */
task_t* prepare_static_userspace_task(uintptr_t start, uintptr_t end,
		char* name, char* uuid_string);


/* \} group */

/**
 * \addtogroup process
 *	\{
 */

/**
 * Entry point for kernel process task
 */
void process_entry();

/* \} group */

/**
 * \addtogroup task
 *	\{
 */

/**
 * Adds file handle to task
 */
void task_add_fhandle(task_t* task, task_file_handle_t* hdl);

/**
 * Gets file handle from task and fd
 */
task_file_handle_t* task_get_fhandle(task_t* task, int32_t fd);

/**
 * Gets next file file descriptor from task
 */
int32_t task_get_next_fd(task_t* task);

/**
 * Removes file handle from task
 */
void task_rem_fhandle(task_t* task, task_file_handle_t* hdl);

/**
 * Sets the system daemon task
 */
void set_sysd_task(task_t* task);


/**
 * Gets the system daemon task
 */
task_t* get_sysd_task();

/* \} group */

#endif /* TASK_H_ */
