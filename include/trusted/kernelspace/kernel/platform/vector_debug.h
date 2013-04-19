/*
 * vector_debug.h
 *
 *  Created on: Oct 1, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef VECTOR_DEBUG_H_
#define VECTOR_DEBUG_H_

#include <common.h>

typedef struct {
    uint32_t scr;
    uint32_t r[13];
    uint32_t pc;
    uint32_t cpsr;
} core_reg;

#endif /* VECTOR_DEBUG_H_ */
