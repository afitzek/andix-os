/**
 * @file user_process.c
 * @brief
 *  Created on: Jan 20, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */


#include <mm/mm.h>
#include <task/user_process.h>
#include <loader.h>
#include <scheduler.h>

static uint32_t current_pid = 1;

static void *find_free_thread_block(struct user_process_t *process) {
	uint8_t *p = THREAD_VMEM_START;
	for (; p < THREAD_VMEM_END; p += THREAD_LOCAL_PAGES_SPACING) {
		// iterate over all available thread local memory blocks
		// p points to the start of the block, while the stacks starts at it's end (minus guard page).
		// So we have to check the end, to see if this block is in use.
		void *first_stack_page = p + (THREAD_LOCAL_PAGES_SPACING - 2) * SMALL_PAGE_SIZE;
		if (!mmu_translate(first_stack_page, NULL)) {	// not translateable, so let's say not mapped.
			return p;
		}
	}
	return NULL;	// Sorry sir, nohave! ;)
}

static void release_thread_block(struct user_process_t *process, void *base) {
	uint8_t *p = base;
	uint32_t pa;
	for (int i = 0; i < THREAD_LOCAL_PAGES_SPACING; i++) {
		// for each possibly mapped page, try to unmap.
		pa = unmap_memory_from_pd((uint32_t) p, process->userPD);
		if (pa)
			pmm_free_page(pa);
		p += SMALL_PAGE_SIZE;
	}
}

struct thread_t *create_user_thread(struct user_process_t *process) {
	void *base = find_free_thread_block(process);
	if (base == NULL) {
		task_error("No free space for new thread!");
		return NULL;
	}

	struct thread_t *thread = create_kernel_thread(USR_MODE, SECURE);
	if (thread == NULL) {
		release_thread_block(process, base);
		return thread;
	}

	kernel_mem_info_t info;
	info.ap = AP_SVC_RW_USR_RW;
	info.execute = EXEC_NON;
	info.bufferable = 1;
	info.cacheable = 1;
	info.nonsecure = 0;
	info.shareable = 0;
	info.type = SMALL_PAGE;


	void *stack_limit = base;
	thread->thread_pointer = base - THREAD_POINTER_TLS_OFFSET;
	// allocate the TLS block for the thread and fill it.
	if (process->tls_template.start && process->tls_template.size) {
		void *start = process->tls_template.start;
		uint32_t size = process->tls_template.size;
		uint32_t pages = needed_pages(0, size);
		for (uint32_t i = 0; i < pages; i++) {
			info.paddr = (uint32_t)pmm_allocate_page();
			if (info.paddr == 0) {
				kfree(thread);
				release_thread_block(process, base);
				return NULL;
			}
			info.vaddr = base + i * SMALL_PAGE_SIZE;
			map_user_memory(process->vuserPD, &info);
		}

		stack_limit += pages + 1;		// the stack can grow towards the tls until it reaches the guard page.
		thread->tls_start = base;
		thread->tls_size = size;
		memcpy(thread->tls_start, start, size);		// copy TLS template
		memset(thread->tls_start + size, 0, pages * SMALL_PAGE_SIZE - size);	// clear the rest.
		task_debug("Mapped TLS at %x size %x", base, size);
	}

	// stack upper end (full descending stack), 1 guard page
	// the sp actually points to the beginning of the guard page, it is decremented before pushing (full stack).
	// Stacks have to be 8-byte aligned, see http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka4127.html
	void *stack_base = base + (THREAD_LOCAL_PAGES_SPACING -1) * SMALL_PAGE_SIZE;
	void *first_page = stack_base - DEFAULT_USER_STACK_PAGES * SMALL_PAGE_SIZE;
	for (uint32_t i = 0; i < DEFAULT_USER_STACK_PAGES; i++) {
		info.paddr = (uint32_t)pmm_allocate_page();
		if (info.paddr == 0) {
			kfree(thread);
			release_thread_block(process, base);
			return NULL;
		}
		info.vaddr = first_page + i * SMALL_PAGE_SIZE;
		map_user_memory(process->vuserPD, &info);
	}
	// clear stack
	memset(first_page, 0, DEFAULT_USER_STACK_PAGES * SMALL_PAGE_SIZE);

	thread->user_stack.base = stack_base;
	thread->user_stack.limit = stack_limit;
	thread->user_stack.sp = stack_base;
	task_debug("Mapping thread stack from %x to %x.", stack_base, first_page);

	return thread;
}

struct user_process_t *create_user_process(uint8_t* elf_start, uint32_t elf_size, const char *name) {
	struct user_process_t *process = (struct user_process_t*) kmalloc(sizeof(struct user_process_t));
	if (process == NULL) {
		task_error("Failed to allocate memory for process!");
		return process;
	}
	memset(process, 0, sizeof(struct user_process_t));

	if (process == NULL) {
		kfree(process);
		return process;
	}

	locked_list_init(&(process->files));
	locked_list_init(&(process->threads));
	process->pid = ++current_pid;

	process->vuserPD = create_page_directory(&process->userPD);

	memset(process->membitmap, 0xFFFFFFFF, sizeof(process->membitmap));	// set everything to free

	// Activate new user space page table to let elf loader and thread creator write into it.
	// This might be quite inefficient, because we loose the complete TLB of the current process :(
	// Could be speed-improved by mapping the new process userspace into the kernel.
	uint32_t* restore_upt = get_user_table();
	set_user_table(process->userPD);
	invalidate_tlb();

	void *entry = setup_elf(elf_start, elf_size, process);
	// Invalid ELF Image
	if (entry == 0xFFFFFFFF) {
		// TODO: free page directory!
		kfree(process);
		return NULL;
	}

	struct thread_t *mainthread = create_user_thread(process);
	if (mainthread == NULL) {
		// TODO its all hopeless! Free everything and fail gracefully
		task_error("Failed to create thread for new process, it's hopeless!");
		kpanic();
	}
	thread_set_entry(mainthread, entry);
	// restore current process userspace page table.
	set_user_table(restore_upt);
	invalidate_tlb();

	if (name) {
		strncpy(process->name, name, TASKNAME_SIZE);
	}
	mainthread->process = process;
	mainthread->state = READY;
	if (!locked_list_add(&process->threads, mainthread)) {
		task_error("Failed to add thread to list, hopeless!");
		kpanic();
	}
	sched_add_thread(mainthread);

	return process;
}


void proc_add_fhandle(struct user_process_t *proc, task_file_handle_t* hdl) {
	if (!locked_list_add(&proc->files, hdl)) {
		task_error("Failed to add file handle to list, hopeless!");
		kpanic();
	}
}

void proc_rem_fhandle(struct user_process_t *proc, task_file_handle_t* hdl) {
	locked_list_remove_match(&proc->files, hdl);
}

task_file_handle_t* proc_get_fhandle(struct user_process_t *proc, int32_t fd) {
	struct locked_list_iterator iter;
	task_file_handle_t *hdl;
	task_debug("Retrieving FD %d for process %s %d", fd, proc->name, proc->pid);
	mutex_lock(&proc->files.mutex);
	locked_list_iter_init(&iter, &proc->files);

	while ((hdl = locked_list_iter_next(&iter))) {
		if (hdl != NULL && hdl->user_fd == fd) {
			break;
		}
	}
	mutex_unlock(&proc->files.mutex);
	return hdl;
}

int32_t proc_get_next_fd(struct user_process_t *proc) {
	int32_t fd;
	task_file_handle_t * hdl;
	for (fd = 100; fd < 0xFFFF; fd++) {
		hdl = proc_get_fhandle(proc, fd);
		if (hdl == NULL) {
			task_debug("New FD for process %s %d: %d", proc->name, proc->pid, fd);
			return fd;
		}
	}
	task_error("No more fds!");
	kpanic();
	return 0;
}
