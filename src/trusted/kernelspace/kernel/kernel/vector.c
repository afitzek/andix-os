/*
 * vector.c
 *
 *  Created on: Dec 1, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <vectors.h>
#include <mm/mm.h>


void setup_mode_stacks() {
	vector_info("==== STACKS ====");

	uintptr_t sp = map_stack_mem(STACK_SIZE);
	vector_info("FIQ Stack: 0x%x", sp);
	setFIQSP(sp);
	sp = map_stack_mem(STACK_SIZE);
	vector_info("IRQ Stack: 0x%x", sp);
	setIRQSP(sp);
	sp = map_stack_mem(STACK_SIZE);
	setABTSP(sp);
	vector_info("ABT Stack: 0x%x", getABTSP());
	sp = map_stack_mem(STACK_SIZE);
	vector_info("UND Stack: 0x%x", sp);
	setUNDSP(sp);
	sp = map_stack_mem(STACK_SIZE);
	vector_info("SYS Stack: 0x%x", sp);
	setSYSSP(sp);

	gotoSVCMode();
}
