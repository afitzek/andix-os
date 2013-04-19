/*
 * umm.h
 *
 *  Created on: Jan 5, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef UMM_H_
#define UMM_H_

#include <task/task.h>

uintptr_t create_page_directory(uintptr_t* ptd);
void map_initial_user_stack(uintptr_t vptd);
void unmap_mem_from_task(uint8_t* vaddr, uint32_t size, task_t* task);
uint8_t* map_mem_to_task(uint8_t* paddr, uint32_t size, task_t* task);
void free_task_mapped_page(void* page, task_t *task);
void* allocate_task_mapped_page_frames(uint32_t numpages, task_t *task);
void map_user_memory(uintptr_t vptd, kernel_mem_info_t *section);

#endif /* UMM_H_ */
