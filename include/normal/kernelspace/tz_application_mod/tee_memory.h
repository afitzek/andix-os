/*
 * tee_memory.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_MEMORY_H_
#define TEE_MEMORY_H_

#define TEE_MEM_STATE_UNMAPPED		(0)
#define TEE_MEM_STATE_READY			(1)

typedef struct {
	uint32_t id;
	tee_context* ctx;
	uint32_t tz_id;
	void* user_addr;
	void* com_vaddr;
	void* com_paddr;
	uint32_t state;
	uint32_t flags;
	size_t size;
} tee_shared_memory;

int tee_memory_init();
tee_shared_memory* tee_memory_add(tee_context* ctx);
void tee_memory_free(tee_shared_memory* session);
tee_shared_memory* tee_memory_find_by_id(uint32_t id);
tee_shared_memory* tee_memory_find_by_tzid(uint32_t tzid);
tee_shared_memory* tee_memory_find_by_ctx(tee_context* ctx);
tee_shared_memory* tee_memory_find_by_paddr(void* paddr);

#endif /* TEE_MEMORY_H_ */
