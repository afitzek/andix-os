/**
 * @file user_process.h
 * @brief
 *  Created on: Jan 17, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#ifndef USER_PROCESS_H_
#define USER_PROCESS_H_

#include <task/thread.h>
#include <common/locked_list.h>
#include <task/uuid.h>

typedef struct {
	int32_t 			user_fd;
	void* 				data;
} task_file_handle_t;

struct thread_local_info_t {
	uint32_t some1;
	uint32_t some2;
	uint32_t tls_start;
};

struct user_process_t{
	char 				name[TASKNAME_SIZE];
	tid_t				pid; 				// process ID
	uint32_t			membitmap[800];		// page bitmap for userspace, --> umm.c
	locked_list			files;				// containing *task_file_handle_t
	locked_list			threads;			// containing thread_t

	uint32_t			userPD;				// physical user page directory address
	uint32_t			vuserPD;			// virtual user page directory address mapped into kernel
	uint32_t			vheap;				// current heap break

	struct elf_TLS_segment_t {				// TLS (thread local storage) template, stored by elf parser
		uint32_t 		start;				// start of the TLS template in process memory
		uint32_t		size;
	} tls_template;

	struct tee_context_t {					// for TEE and Trustlets
		TASK_UUID			uuid;
		TRUSTLET_STATE		trustlet_state;
		uintptr_t			tee_rpc;
		struct thread_t		*tee_handler;	// Thread to wake up on TEE requests
	} tee_context;
};



#define THREAD_LOCAL_PAGES_SPACING 64
#define THREAD_POINTER_TLS_OFFSET 8		// gcc's tls starts 8 bytes after the thread pointer (don't know why).

#define DEFAULT_USER_STACK_PAGES 8

#define THREAD_VMEM_START 	0x60000000	// page aligned
#define THREAD_VMEM_END 	0x6FFFFFC0	// ~256MB THREAD_VMEM_START + 0x3FFFFF * THREAD_LOCAL_PAGES_SPACING


struct user_process_t *create_user_process(uint8_t* elf_start, uint32_t elf_size, const char *name);
struct thread_t *create_user_thread(struct user_process_t *process);
void proc_add_fhandle(struct user_process_t *proc, task_file_handle_t* hdl);
void proc_rem_fhandle(struct user_process_t *proc, task_file_handle_t* hdl);
task_file_handle_t* proc_get_fhandle(struct user_process_t *proc, int32_t fd);
int32_t proc_get_next_fd(struct user_process_t *proc);
#endif /* USER_PROCESS_H_ */
