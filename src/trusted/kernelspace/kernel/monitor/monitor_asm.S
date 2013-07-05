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
 * monitor.S
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <platform/platform.h>

.section .text, "ax"
.code 32
.align 5
.global __monitor_vector
__monitor_vector:
	/*
	 * Monitor vector table
	 */
	LDR		pc, __mon_reset 	// Reset
	LDR		pc, __mon_undef		// undefined instruction
	LDR		pc, __mon_smc		// software unterrupt
	LDR		pc, __mon_pabt		// prefetch abort
	LDR		pc, __mon_dabt		// data abort
	B		.					// reserved
	LDR		pc, __mon_irq		// IRQ
	LDR		pc, __mon_fiq		// FIQ

__mon_reset:       .word __mon_no_handler
__mon_undef:       .word __mon_no_handler
__mon_smc:         .word __mon_smc_handler
__mon_pabt:        .word __mon_data_ab
__mon_dabt:        .word __mon_data_ab
__mon_irq:         .word __mon_irq_handler
__mon_fiq:         .word __mon_fiq_handler

__mon_no_handler:
	B		.

		//----------------------------------------------------------------------
		// Standard prologue for the monitor service routines.
		//
.macro iaik_msr_prologue pc_arm_adjust=#0
        SRSDB sp!, #ABT_MODE
        STMFD sp!, {r0-r12}           /* Save monitor core context */

        MRC   p15, 0, r0, c1, c1, 0   /* Read secure configuration register */
        STMFD sp!, {r0}
        TST   r0, #SCR_NS
        BICNE r0, #SCR_NS             /* Clear SCR.NS bit */
        MCRNE p15, 0, r0, c1, c1, 0   /* Write secure configuration register */
.endm

        //----------------------------------------------------------------------
        // Standard epilogue for the monitor service routines.
        //
        .macro iaik_msr_epilogue
        LDMFD sp!, {r0}                /* Read save value of secure configuration register */
        TST   r0,  #SCR_NS
        MCRNE p15, 0, r0, c1, c1, 0    /* Restore secure configuration register */

        LDMFD sp!, {r0-r12}            /* Load monitor core context */
        RFEIA sp!                      /* Return from exception */
        .endm

.macro	mon_save_context pc_ret_off=#0
	ISB
	DSB
	DMB
	// Store return state
	SRSDB 	sp!, #0x16

	// Store register context r0 - r12
	STMFD 	sp!, {r0-r12}

	// READ SCR
	MRC 	p15, 0, r0, c1, c1, 0
	// STORE SCR
	STMFD 	sp!, {r0}
	TST		r0, #1
	BICNE	r0, #1
	MCRNE	p15, 0, r0, c1, c1, 0
.endm

.macro mon_restore_context
	ISB
	DSB
	DMB
	LDMFD	sp!, {r0}
	TST 	r0, #1
	MCRNE	p15, 0, r0, c1, c1, 0

	LDMFD	sp!, {r0-r12}
	RFEIA	sp!
	ISB
	DSB
.endm

__mon_smc_handler:

	mon_save_context
	MCR 	p15, 0, r0, c7, c10, 4 // DSB
	MCR 	p15, 0, r0, c7, c10, 5 // DMB
	MOV		r0, sp
	BL 		mon_smc_handler
	MCR 	p15, 0, r0, c7, c10, 4 // DSB
	MCR 	p15, 0, r0, c7, c10, 5 // DMB
	mon_restore_context

.global __mon_data_ab
__mon_data_ab:
	SUB    lr, lr, #8
	iaik_msr_prologue #8

	MOV r0, sp
	BL	pDA
	B	.

	iaik_msr_epilogue

__mon_irq_handler:
	SUB    lr, lr, #4
	mon_save_context  #4

	MOV r0, sp
	BL	pIRQ

	mon_restore_context

__mon_fiq_handler:
	SUB    lr, lr, #4
	mon_save_context #4

	MOV r0, sp
	BL	pIRQ

	mon_restore_context

        .global monitor_save_sys_regs
        .global monitor_restore_sys_regs

        //----------------------------------------------------------------------
        // Saves the current system registers.
        //
        // C prototype: void monitor_save_sys_regs(monitor_sys_regs_t *sysregs)
        .text
        .arm
monitor_save_sys_regs:
        CPS   #SVC_MODE
        MRS   r1,  SPSR
        STMIA r0!, {r1, sp, lr}

        CPS   #ABT_MODE
        MRS   r1,  SPSR
        STMIA r0!, {r1, sp, lr}

        CPS   #UND_MODE
        MRS   r1,  SPSR
        STMIA r0!, {r1, sp, lr}

        CPS #IRQ_MODE
        MRS   r1,  SPSR
        STMIA r0!, {r1, sp, lr}

        CPS   #FIQ_MODE
        MRS   r1,  SPSR
        STMIA r0!, {r1, r8-r12, sp, lr}

        CPS   #SYS_MODE
        STMIA r0!, {sp, lr}

        CPS   #MON_MODE
        BX    LR
        .type monitor_save_sys_regs, function
        .size monitor_save_sys_regs, . - monitor_save_sys_regs

        //----------------------------------------------------------------------
        // Restores the current system registers.
        //
        //
        // C prototype: void monitor_restore_sys_regs(monitor_sys_regs_t *sysregs)
        .text
        .arm
monitor_restore_sys_regs:
        CPS   #SVC_MODE
        LDMIA r0!, {r1, sp, lr}
        MSR   SPSR, r1

        CPS   #ABT_MODE
        LDMIA r0!, {r1, sp, lr}
        MSR   SPSR, r1

        CPS   #UND_MODE
        LDMIA r0!, {r1, sp, lr}
        MSR   SPSR, r1

        CPS   #IRQ_MODE
        LDMIA r0!, {r1, sp, lr}
        MSR   SPSR, r1

        CPS   #FIQ_MODE
        STMIA r0!, {r1, r8-r12, sp, lr}
        MSR   SPSR, r1

        CPS   #SYS_MODE
        LDMIA r0!, {sp, lr}

        CPS   #MON_MODE
        BX    LR
        .type monitor_restore_sys_regs, function
        .size monitor_restore_sys_regs, . - monitor_restore_sys_regs
