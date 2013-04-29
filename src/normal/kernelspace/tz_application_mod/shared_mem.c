/*
 * shared_mem.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <tz_application_mod/andixtee.h>

int needed_log_pages(size_t memsize) {
	int pgcount = memsize / PAGE_SIZE;
	int log_alloc = 0;
	int pgcount_alloc = ipow(2, log_alloc);  // 2 ^ 0

	while (pgcount_alloc < pgcount) {
		log_alloc++;
		pgcount_alloc = ipow(2, log_alloc);
	}

	return (log_alloc);
}
