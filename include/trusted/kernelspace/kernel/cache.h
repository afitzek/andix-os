/*
 * cache.h
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <common/typedefs.h>

void invalidate_dcache_all(void);
void flush_dcache_all(void);
void invalidate_dcache_range(uint32_t start, uint32_t stop);
void flush_dcache_range(uint32_t start, uint32_t stop);
void flush_cache(uint32_t start, uint32_t size);
void invalidate_tlb(void);
void invalidate_icache_all(void);
void v7_flush_dcache_all(void);

uint32_t getUDID();

#define CP15ISB	asm volatile ("ISB")
#define CP15DSB	asm volatile ("DSB")
#define CP15DMB	asm volatile ("DMB")

#endif /* CACHE_H_ */
