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

void idle_task_entry();

/**
 * Entrypoint for main kernel task
 */
void entry_main_task();

void init_task();
task_t* get_current_task();
tid_t get_current_task_id();
void set_current_task(task_t* task);
task_t* create_kernel_task(uint32_t mode, EXEC_CONTEXT_t context);
void nonsecure_set_cp_access(uint32_t cp);
void task_set_name(task_t* task, char* name);
int uuid_parse(const char *in, TASK_UUID* uuid);

void set_idle_task(task_t* task);
void init_early_task();
void set_nonsecure_task(task_t* task);
task_t* get_nonsecure_task();

void add_task(task_t* task);
void rem_task(task_t* task);
task_t* get_task_by_id(tid_t tid);
task_t* get_task_by_name(char* name);
task_t* get_task_by_uuid(TASK_UUID* uuid);
task_t* task_get_next_ready();

void set_uuid_emtpy(TASK_UUID *uuid);
void cpy_uuid(TASK_UUID *dst, TASK_UUID *src);
uint8_t is_uuid_empty(TASK_UUID *uuid);
uint8_t match_uuids(TASK_UUID *a, TASK_UUID *b);
void print_tasks();

void init_userspace_task(task_t* task);
task_t* prepare_static_userspace_task(uintptr_t start, uintptr_t end,
		char* name, char* uuid_string);

void process_entry();

void task_add_fhandle(task_t* task, task_file_handle_t* hdl);
task_file_handle_t* task_get_fhandle(task_t* task, int32_t fd);
int32_t task_get_next_fd(task_t* task);
void task_rem_fhandle(task_t* task, task_file_handle_t* hdl);
void set_sysd_task(task_t* task);
task_t* get_sysd_task();

#endif /* TASK_H_ */
