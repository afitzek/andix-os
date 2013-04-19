/*
 * tee_memregion.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_MEMREGION_H_
#define TEE_MEMREGION_H_

#include <common.h>
#include <tee/tee_context.h>
#include <task/task.h>

typedef struct {
	task_t*		task;
	uint32_t	vaddr;
} tee_mapped_mem;

typedef struct {
	uint32_t _id;
	tee_context* context;
	uint32_t	flags;
	uint32_t	paddr;
	uint32_t	vaddr;
	uint32_t	size;
	list*		mappedtasks;
} tee_memory;

void tee_memory_initialize();
tee_memory* tee_memory_create(tee_context* context);
void tee_memory_destroy(tee_memory* memory);
tee_memory* tee_memory_find(uint32_t id);
tee_memory* tee_memory_find_by_ctx(tee_context* context);
uint32_t	tee_memory_map_to_task(task_t *task, tee_memory* memory);


#endif /* TEE_MEMREGION_H_ */
