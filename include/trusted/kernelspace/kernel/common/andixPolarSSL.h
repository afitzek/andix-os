/*
 * AndixPolar.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ANDIXPOLAR_H_
#define ANDIXPOLAR_H_

#include <common/string.h>
#include <common/typedefs.h>
#include <kprintf.h>
#include <mm/mm.h>

#define printf(...) kprintf(__VA_ARGS__)
#define malloc(size) kmalloc(size)
#define free(ptr) kfree((uintptr_t)ptr)

#endif /* ANDIXPOLAR_H_ */
