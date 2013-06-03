/*
 * common.h
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <common/list.h>
#include <common/atags.h>
#include <common/time.h>
#include <common/mutex.h>
#include <common/string.h>
#include <common/typedefs.h>
#include <common/debug.h>
#include <common/queue.h>

uint32_t getCPSR();
void *memset(void *s, int c, int n);
char* getModeString(uint32_t cpsr);

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define roundup(x, y)		((((x) + ((y) - 1)) / (y)) * (y))
uint8_t is_kernel_addr(uint32_t value);
uint8_t is_valid_addr(void* ptr);
uint8_t is_valid_user_addr(void* ptr);
uint8_t is_valid_kernel_addr(void* ptr);
uint8_t is_valid_nonsecure_ram_addr(void* ptr);

#endif /* COMMON_H_ */
