/**
 * @file thread.h
 * @brief
 *  Created on: Jan 22, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#ifndef THREAD_H_
#define THREAD_H_

#define TASKNAME_SIZE 50

#define PROCESS_TASK "NS_PROCESSOR"
#define SERVICE_TASK "NS_SERVICE"
#define TEE_TASK "TEE_TASK"

#include <monitor/monitor.h>

typedef enum {
	SECURE,
	NONSECURE
} EXEC_CONTEXT_t;

typedef enum {
	NONE = 0,
	NEW,
	CREATING,
	CREATED,
	PERFORMING,
	DESTROYING,
	DESTROYED
} TRUSTLET_STATE;

struct stack_info_t {
	void *base;
	void *limit;
	void *sp;
};

typedef enum {
	READY = 0,
	RUNNING,
	BLOCKED,
	SLEEPING,
	DEAD
} thread_state_t;

struct thread_t {
	mon_context_t 		context;			// saved/restored at monitor entry/exit
	mon_sys_context_t 	sys_context;		// saved/restored on task switch
	EXEC_CONTEXT_t		exec_context;
	tid_t				tid; 				// Task/thread ID
//	const char 			*name;				// TODO deprecated
	thread_state_t			state;
	struct stack_info_t kernel_stack;
	struct stack_info_t user_stack;
	struct user_process_t	*process;
	struct thread_local_info_t *thread_pointer;	// thread_info in user space containing TLS
	void 				*tls_start;
	uint32_t			tls_size;
};

struct thread_t *create_kernel_thread(uint32_t mode, EXEC_CONTEXT_t world);
void thread_set_name(struct thread_t *thread, const char *name);
void thread_set_entry(struct thread_t *thread, void *entry);
int print_thread(struct thread_t *thread, void *none);
char* thread_state_str(thread_state_t state);

#define KERNEL_STACK_SIZE 0x4000

#endif /* THREAD_H_ */
