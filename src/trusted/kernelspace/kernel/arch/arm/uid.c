/*
 * uid.c
 *
 *  Created on: Jan 18, 2012
 *      Author: Andreas Fitzek
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/typedefs.h>
#include <kprintf.h>

uint32_t getUDID() {
	// TODO: we need to get some unique ID per device as salt!
	uint32_t result;
	uint32_t main_id;
	uint32_t silicon_id;
	// These values are not device unique!!
	__asm__ __volatile__("MRC p15, 0, %0, c0, c0, 0": "=r" (main_id)::"memory");
	__asm__ __volatile__("MRC p15, 1, %0, c0, c0, 7": "=r" (silicon_id)::"memory");

	result = main_id ^ silicon_id;
	return (result);
}
