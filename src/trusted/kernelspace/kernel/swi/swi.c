/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/*
 * swi.c
 *
 *  Created on: Dec 3, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <swi/swi.h>
#include <devices/random/random.h>

void dump_cpu_state(uint32_t swi, cpu_state_t* cpu) {
	swi_debug("SCR: 0x%x   PC: 0x%x", swi, cpu->lr);
	swi_debug(
			"R0: 0x%x    R1: 0x%x    R2: 0x%x", cpu->r[0], cpu->r[1], cpu->r[2]);
	swi_debug(
			"R3: 0x%x    R4: 0x%x    R5: 0x%x", cpu->r[3], cpu->r[4], cpu->r[5]);
	swi_debug(
			"R6: 0x%x    R7: 0x%x    R8: 0x%x", cpu->r[6], cpu->r[7], cpu->r[8]);
	swi_debug(
			"R9: 0x%x   R10: 0x%x   R11: 0x%x", cpu->r[9], cpu->r[10], cpu->r[11]);
	swi_debug("R12: 0x%x", cpu->r[12]);
}

void dispatch_swi(uint32_t swi, cpu_state_t* cpu) {
	swi_debug("====================================");
	swi_debug("SWI CALL");
	//dump_cpu_state(swi, cpu);
	swi_debug(
			"SWI %x R0: %x R1: %x R2: %x R12: %x", swi, cpu->r[0], cpu->r[1], cpu->r[2], cpu->r[12]);

	switch (cpu->r[12]) {
	// ====================================================================
	// Newlib SWI
	// ====================================================================
	case SWI_WRITE:
		cpu->r[0] = swi_write(cpu->r[0], (uint8_t*) cpu->r[1], cpu->r[2]);
		break;
	case SWI_SBRK:
		cpu->r[0] = swi_sbrk(cpu->r[0]);
		break;
	case SWI_HEND:
		cpu->r[0] = swi_heapend();
		break;
	case SWI_OPEN:
		cpu->r[0] = swi_open((char*) cpu->r[0], (int) cpu->r[1],
				(int) cpu->r[2]);
		break;
	case SWI_CLOSE:
		cpu->r[0] = swi_close((int) cpu->r[0]);
		break;
	case SWI_READ:
		cpu->r[0] = swi_read(cpu->r[0], (uint8_t*) cpu->r[1], cpu->r[2]);
		break;
	case SWI_LSEEK:
		cpu->r[0] = swi_lseek(cpu->r[0], cpu->r[1], cpu->r[2]);
		break;
	case SWI_FSTAT:
		cpu->r[0] = swi_fstat(cpu->r[0]);
		break;
		// ====================================================================
		// Secure Calls
		// ====================================================================
	case SWI_SECURE_REQUEST:
		swi_get_secure_request((TA_RPC*) cpu->r[0]);
		break;
		// ====================================================================
	case SWI_SECURE_SET_UUID:
		swi_set_uuid((TASK_UUID*) cpu->r[0]);
		break;
	case SWI_GET_RAND:
		cpu->r[0] = random_next32();
		break;
		// ====================================================================
		// Fallback
		// ====================================================================
	default:
		swi_info("Unknown SWI CALL: %d", cpu->r[12]);
		break;
	}

	//swi_debug("SWI RETURN");
	//swi_debug("====================================");
}
