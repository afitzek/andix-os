/*
 * cpu.h
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef CPU_H_
#define CPU_H_

struct cpu_state
{
    // r0 bis r12 sowie r15
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7;
    uint32_t r8, r9, r10, r11, r12, r14;
} __attribute__((packed));

#endif /* CPU_H_ */
