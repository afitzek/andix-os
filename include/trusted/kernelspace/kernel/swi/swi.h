/*
 * swi.h
 *
 *  Created on: Dec 3, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SWI_H_
#define SWI_H_

#include <common/typedefs.h>
#include <kprintf.h>
#include <monitor/monitor.h>
#include <task/task.h>
#include <swi/swi_newlib.h>
#include <swi/swi_secure.h>

#define	SEEK_SET	0
#define	SEEK_CUR	1
#define	SEEK_END	2

#define SWI_WRITE	0x01
#define SWI_READ	0x02
#define SWI_SBRK	0x03
#define SWI_HEND	0x04
#define SWI_OPEN	0x05
#define SWI_CLOSE	0x06
#define SWI_LSEEK	0x07
#define SWI_FSTAT   0x08

#define SWI_SECURE_REQUEST  0x81
#define SWI_SECURE_SET_UUID 0x82

typedef struct cpu_state {
	uint32_t r[13];
	uint32_t lr;
} cpu_state_t;

void dispatch_swi(uint32_t swi, cpu_state_t* cpu);

#endif /* SWI_H_ */
