/*
 * tzmem_controller.h
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#ifndef TZMEM_CONTROLLER_H_
#define TZMEM_CONTROLLER_H_

#include <common.h>

/**
 * \addtogroup tzmem_ctrl
 *	\{
 */

/**
 * Lock memory region from nonsecure access
 */
#define TZ_MEM_IOCTL_LOCK_MEMORY (0x1)

/**
 * Unlock memory region from nonsecure access
 */
#define TZ_MEM_IOCTL_UNLOCK_MEMORY (0x2)

/**
 * Dump info about tz memcontroller
 */
#define TZ_MEM_IOCTL_DUMP (0x3)

/**
 * ioctl parameter data structure for tz mem controller
 */
struct tz_mem_ioparam {
	void* physical_addr;
	uint32_t size;
};

typedef struct tz_mem_ioparam tz_mem_ioparam_t;

/**
 * Initialize the tz memory system
 *
 * @return HAL_*
 */
int32_t tzmem_init();

/**
 * Lock physical memory from nonsecure world
 *
 * @param paddr The physical addr
 * @param size The size of the memory area
 * @return HAL_*
 */
int32_t tzmem_lock(uintptr_t* paddr, uint32_t size);

/**
 * Lock physical memory from nonsecure world
 *
 * @param paddr The physical addr
 * @param size The size of the memory area
 * @return HAL_*
 */
int32_t tzmem_unlock(uintptr_t* paddr, uint32_t size);

/**
 * Dump information about tz memory controller
 */
void tzmem_dump();

/** \} */

#endif /* TZMEM_CONTROLLER_H_ */
