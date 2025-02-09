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
 * swi_newlib.c
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <swi/swi.h>
#include <mm/mm.h>
#include <task/user_process.h>
#include <scheduler.h>
#include <fs/fs.h>
#include <devices/random/random.h>

int32_t swi_fstat(int file) {
	struct user_process_t *proc = get_current_thread()->process;
	fs_stat stat;

	if (proc == NULL ) {
		kpanic();
	}

	if (file == 0 || file == 1 || file == 2) {
		return (-1);
	}

	task_file_handle_t* hdl = proc_get_fhandle(proc, file);

	if (hdl == NULL ) {
		return (-1);
	}

	if (hdl->data == NULL ) {
		return (-1);
	}

	if (fs_fstat(hdl->data, &stat) != 0) {
		return (-1);
	}
	return (stat.st_size);
}

int32_t swi_lseek(int32_t file, uint32_t ptr, int32_t dir) {

	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	if (file == 0 || file == 1 || file == 2) {
		return (-1);
	}

	task_file_handle_t* hdl = proc_get_fhandle(proc, file);

	if (hdl == NULL ) {
		return (-1);
	}

	if (hdl->data == NULL ) {
		return (-1);
	}

	if (dir == SEEK_CUR) {
		return (fs_lseek(hdl->data, ptr, REE_SEEK_CUR));
	} else if (dir == SEEK_SET) {
		return (fs_lseek(hdl->data, ptr, REE_SEEK_SET));
	} else {
		return (-1);
	}
}

int32_t swi_write(uint32_t socket, uint8_t* buffer, uint32_t size) {
	uint8_t* tmp;
	if (!is_valid_user_addr(buffer)) {
		return (-1);
	}

	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	if (socket == 0 || socket == 3) {
		return (-1);
	}

	if (socket == 1) {
		tmp = (uint8_t*)kmalloc(size + 1);
		memset(tmp, 0, size + 1);
		memcpy(tmp, buffer, size);
		userspace_out("STDOUT: %s", tmp);
		kfree((uintptr_t)tmp);
		return (size);
	}

	if (socket == 2) {
		tmp = (uint8_t*)kmalloc(size + 1);
		memset(tmp, 0, size + 1);
		memcpy(tmp, buffer, size);
		userspace_out("STDERR: %s", buffer);
		kfree((uintptr_t)tmp);
		return (size);
	}

	task_file_handle_t* hdl = proc_get_fhandle(proc, socket);

	if (hdl == NULL ) {
		return (-1);
	}

	if (hdl->data == NULL ) {
		return (-1);
	}

	return (fs_write(hdl->data, buffer, size));
}

int32_t swi_read(uint32_t socket, uint8_t* buffer, uint32_t size) {
	uint32_t idx = 0;
	if (!is_valid_user_addr(buffer)) {
		return (-1);
	}

	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	if (socket == 1 || socket == 2) {
		return (-1);
	}

	if (socket == 0) {
		memset(buffer, 0, size);
		userspace_out("STDIN:%s" ,"");
		getinput((char*) buffer, size);
		userspace_out("GOT %s" ,buffer);
		return (size);
	}

	if(socket == 3) {
		swi_info("Random filling buffer @ 0x%x (%d)", buffer, size);
		for(idx = 0; idx < size; idx++) {
			buffer[idx] = random_next8();
		}
		kprintHex(buffer, size);
	}

	task_file_handle_t* hdl = proc_get_fhandle(proc, socket);

	if (hdl == NULL ) {
		return (-1);
	}

	if (hdl->data == NULL ) {
		return (-1);
	}

	return (fs_read(hdl->data, buffer, size));
}

uint32_t swi_heapend() {
	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	return ((uint32_t) proc->vheap);
}

int32_t swi_close(int file) {
	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	task_file_handle_t* hdl = proc_get_fhandle(proc, file);

	if (hdl == NULL ) {
		return (0);
	}

	if (hdl->data != NULL ) {
		fs_close((cfs_t*) hdl->data);
		kfree(hdl->data);
		hdl->data = NULL;
	}

	proc_rem_fhandle(proc, hdl);

	kfree((void*) hdl);

	return (0);
}

int32_t swi_open(char *name, int flags, int mode) {
	struct user_process_t *proc = get_current_thread()->process;

	if (!is_valid_user_addr(name)) {
		return (-1);
	}

	if (proc == NULL ) {
		kpanic();
	}

	if(strncmp(name, "/dev/urandom", strlen("/dev/urandom")) == 0) {
		return (3);
	}

	task_file_handle_t* hdl = (task_file_handle_t*) kmalloc(
			sizeof(task_file_handle_t));

	if (hdl == NULL ) {
		return (-1);
	}

	hdl->data = kmalloc(sizeof(cfs_t));

	if (hdl->data == NULL ) {
		goto error;
	}

	swi_info("Open %s [Flags 0x%x]", name, flags);

	if (fs_open((uint8_t*) &proc->tee_context.uuid, sizeof(TASK_UUID), (uint8_t*) name,
			strlen(name), flags, mode, hdl->data) != 0) {
		goto error;
	}

	hdl->user_fd = proc_get_next_fd(proc);

	proc_add_fhandle(proc, hdl);

	return (hdl->user_fd);

	error: if (hdl != NULL ) {
		if (hdl->data != NULL ) {
			kfree(hdl->data);
		}
		kfree((uintptr_t) hdl);
	}

	return (-1);
}

int32_t swi_sbrk(int32_t incr) {
	struct user_process_t *proc = get_current_thread()->process;

	if (proc == NULL ) {
		kpanic();
	}

	uint32_t heap = (uint32_t) proc->vheap;
	int32_t tmp_incr = incr;
	uint32_t left_on_page = 0xFFF - (heap & 0xFFF);
	kernel_mem_info_t mem_info;
	mem_info.execute = EXEC_NON;
	mem_info.ap = AP_SVC_RW_USR_RW;
	mem_info.bufferable = 1;
	mem_info.cacheable = 1;
	mem_info.domain = 0;
	mem_info.nonsecure = 0;
	mem_info.shareable = 0;
	mem_info.tex = 0;
	mem_info.type = SMALL_PAGE;
	tmp_incr = tmp_incr - left_on_page;

	while (tmp_incr > 0) {
		uint32_t virtual_page_addr = (heap & 0xFFFFF000) + 0x1000 - 1;
		uint32_t physical_page_addr = (uint32_t) pmm_allocate_page();

		if (physical_page_addr == 0) {
			// TODO: OUT OF MEMORY!!
			// SWAP MEM
			kpanic();
		}

		mem_info.paddr = physical_page_addr;
		mem_info.vaddr = virtual_page_addr;
		if (map_memory_v(proc->vuserPD, &mem_info) != 0) {
			kpanic();
		}

		tmp_incr = tmp_incr - 0x1000;
		heap += 0x1000;
	}

	proc->vheap = (uintptr_t) ((uint32_t) proc->vheap + incr);

	dump_mmu(0x0, 0x20000, proc->vuserPD);

	main_debug("VHEAP: 0x%x INCR: 0x%x", proc->vheap, incr);

	return (0);
}

uint32_t swi_get_pid() {
	return (get_current_process_id());
}
