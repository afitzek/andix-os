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
 * scheduler.c
 *
 *  Created on: Jan 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <scheduler.h>
#include <common/locked_list.h>
#include <monitor/monitor.h>

void init_scheduler() {
	scheduler_debug("Scheduler initialization");
}

void switch_to_thread(struct thread_t *thread) {
	__smc_1(SSC_TASK_SWITCH, (uint32_t)thread);
}

void return_to_ns() {
	__smc_1(SSC_NONS_SERVICE, 0);
}

void yield() {
	__smc_1(SSC_TASK_SCHEDULE, 0);
}


static struct thread_t *current_thread = NULL;
static struct thread_t *idle_thread = NULL;
static struct thread_t *nonsecure_thread = NULL;
static locked_list thread_list;

struct thread_t *get_current_thread() {
	return current_thread;
}

tid_t get_current_thread_id() {
	return current_thread ? current_thread->tid : 0;
}

tid_t get_current_process_id() {
	return current_thread && current_thread->process ? current_thread->process->pid : 0;
}

void sched_thread_switched(struct thread_t *thread) {
	if (current_thread != NULL ) {
		if (current_thread->state == RUNNING) {
			current_thread->state = READY;
		}
	}
	current_thread = thread;
	thread->state = RUNNING;
}

void set_nonsecure_thread(struct thread_t *t) {
	nonsecure_thread = t;
}

struct thread_t *get_nonsecure_thread() {
	return nonsecure_thread;
}

void sched_init() {
	locked_list_init(&thread_list);
}

void sched_add_thread(struct thread_t *thread) {
	task_debug("adding thread 0x%x %d to scheduler", thread, thread->tid);
	locked_list_add(&thread_list, thread);
}

void sched_rm_thread(struct thread_t *thread) {
	task_debug("removing thread 0x%x %d from scheduler", thread, thread->tid);
	locked_list_remove_match(&thread_list, thread);
}

struct thread_t *sched_get_next_ready_thread(void) {
	// this is the completely unfair scheduler ;)
	struct locked_list_iterator tl_iter;
	struct thread_t *th;

	locked_list_iter_init(&tl_iter, &thread_list);
	// The iterator is not safe against list modifications!
	while ((th = locked_list_iter_next(&tl_iter))) {
		task_debug("sched checking thread 0x%x %d %s", th, th->tid, thread_state_str(th->state));
		if (th->state == READY) {
			break;
		}
//		if (th->state == SLEEPING && th->waketime <= getsystime()) {
//		TODO
//		}
	}
	task_debug("Next thread to run is 0x%x %d", th, th ? th->tid : 0);
	return th;	// NULL if none ready
}

struct user_process_t *get_process_by_uuid(TASK_UUID* uuid) {
	struct locked_list_iterator tl_iter;
	struct thread_t *th;
	if (is_uuid_empty(uuid))
		return NULL;

	locked_list_iter_init(&tl_iter, &thread_list);
	mutex_lock(&thread_list.mutex);
	while ((th = locked_list_iter_next(&tl_iter))) {
		if (th->process && !is_uuid_empty(&th->process->tee_context.uuid) &&
				match_uuids(&th->process->tee_context.uuid, uuid))
			break;
	}
	mutex_unlock(&thread_list.mutex);
	return th ? th->process : NULL;
}

void print_threads() {
	task_info("Available Threads:");
	locked_list_for_each(&thread_list, print_thread, NULL);
	task_info("-------------");
}
