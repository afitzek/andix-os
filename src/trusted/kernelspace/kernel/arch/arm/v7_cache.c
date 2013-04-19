/*
 * v7_cache.c
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <cache.h>

#define CACHE_LEVEL_1		0x0
#define CACHE_LEVEL_2		0x1

/**
 * Set Cache Size Selection Register
 * @param level The cache level (1 (0) or 2 (1))
 * @param type The cache type
 */
void set_cssr(uint8_t level, uint8_t type) {
	uint32_t cssr = (level << 1) | (type & 0x1);
	asm volatile("MCR p15, 2, %0, c0, c0, 0"::"r"(cssr));
}

/**
 * Get Cache Size ID Register
 */
uint32_t get_csir() {
	uint32_t csir;
	asm volatile("MRC p15, 1, %0, c0, c0, 0":"=r"(csir));
	return (csir);
}

/**
 * Get Cache Level ID Register
 */
uint32_t get_clir() {
	uint32_t clir;
	asm volatile("MRC p15, 1, %0, c0, c0, 1":"=r"(clir));
	return (clir);
}

/* Invalidate TLB */
void invalidate_tlb()
{
	/* Invalidate entire unified TLB */
	asm volatile ("MCR p15, 0, %0, c8, c7, 0" : : "r" (0));
	/* Invalidate entire data TLB */
	asm volatile ("MCR p15, 0, %0, c8, c6, 0" : : "r" (0));
	/* Invalidate entire instruction TLB */
	asm volatile ("MCR p15, 0, %0, c8, c5, 0" : : "r" (0));
	/* Full system DSB - make sure that the invalidation is complete */
	CP15DSB;
	/* Full system ISB - make sure the instruction stream sees it */
	CP15ISB;
}

void invalidate_dcache_all(void) {

}

void flush_dcache_all(void);
void invalidate_dcache_range(uint32_t start, uint32_t stop);
void flush_dcache_range(uint32_t start, uint32_t stop);
void flush_cache(uint32_t start, uint32_t size);

void invalidate_icache_all(void) {
	/*
	 * Invalidate all instruction caches to PoU.
	 * Also flushes branch target cache.
	 */
	asm volatile ("MCR p15, 0, %0, c7, c5, 0" : : "r" (0));

	/* Invalidate entire branch predictor array */
	asm volatile ("MCR p15, 0, %0, c7, c5, 6" : : "r" (0));

	/* Full system DSB - make sure that the invalidation is complete */
	CP15DSB;

	/* ISB - make sure the instruction stream sees it */
	CP15ISB;
}
