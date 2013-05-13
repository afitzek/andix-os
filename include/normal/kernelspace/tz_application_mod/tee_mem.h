/*
 * tee_mem.h
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#ifndef TEE_MEM_H_
#define TEE_MEM_H_

#include <tz_application_mod/andixtee.h>

int push_tee_to_userspace(TZ_TEE_SPACE *userspace);
int push_tee_to_com(TZ_TEE_SPACE *userspace);
int register_tee_mem_in_tz(TZ_TEE_SPACE *com);
int unregister_tee_mem_from_tz(void);
TZ_TEE_SPACE *get_shared_tee_mem(void);
void cleanup_tee(void);
int initialize_tee(void);
int push_tee(TZ_TEE_SPACE* kspace);
int poll_tee(TZ_TEE_SPACE* kspace);
void tee_flush_cache(void);
void tee_invalidate(void* memory, uint32_t size);

#endif /* TEE_MEM_H_ */
