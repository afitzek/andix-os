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
 * Startup code
 *
 *  Created on: Sep 7, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <platform/platform.h>

	.section .reset, "ax"
	.code 32

.macro debug_a
		B	.
.endm

		//----------------------------------------------------------------------
		// Standard prologue for the monitor service routines.
		//
.macro iaik_msr_prologue pc_arm_adjust=0
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

.macro	swi_entry
	STMFD 	sp!, {r0-r12, lr}
.endm

.macro swi_return
	LDMFD   sp!, {r0-r12, pc}^
.endm

.macro dbg_print
	LDR r1, __test_addr
	LDR r2, =0x41
	STR r2, [r1,#0x40]
.endm

.global _reset
_reset:
	MRC p15, 0, r1, c0, c0, 5
	AND r1, r1, #0x3
	CMP r1, #0
	BEQ __initialize
__wait_loop:
	B	.

__initialize:
	//B	.
	// save boot information
	MOV r6, r1
	MOV r7, r2
	dbg_print
	// Stack setup
	LDR r1, =svc_stack
	//ADR r1, _reset
	BIC	r0, r0, #MODE_BITS
	ORR r0, r0, #SVC_MODE
	MSR CPSR, r0
	MOV sp, r1
	// Find board information
	ADR	r0, _reset
	LDR r3, =__board_infos
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	MOV r1, #0 // table position
	B   __bi_search
__stop:
	B	__stop
__bi_search:
	LDR r2, __board_infos_size
	CMP r1, r2
	BGE __stop // board not supported stop ...
	LDR r2, [r0, r1] // sysID
	ADD r1, r1, #4
	LDR r3, [r0, r1] // phys target
	ADD r1, r1, #4
	LDR r4, [r0, r1] // early io
	ADD r1, r1, #4
	CMP r2, r6
	BNE __bi_search
	STR r3, __phys_target_addr
	STR r4, __phys_early_io
__setup_mmu:
	ADR	r0, _reset
	STR r0, __phys_entry_addr
	BIC r0, r0, #0xFF00
	BIC r0, r0, #0xFF
	STR r0, __phys_load_addr

	LDR r0, __reset_addr // __entry_addr
	BIC r0, r0, #0xFF00
	BIC r0, r0, #0xFF
	STR r0, __virt_load_addr

	LDR r0, =_end
	LDR r1, =_code
	SUB r0, r0, r1
	STR r0, __kernel_b_size
	MOV r0, r0, LSR #20
	ADD r0, r0, #2
	STR r0, __kernel_mb_size
/*	ADR r0, __entry_reloc
	LDR r3, __phys_load_addr
	LDR r4, __phys_target_addr
	CMP r3, r4
	BGE __load_ge
	SUB r3, r4, r3
	ADD r0, r0, r3
	B   __reloc_go_off
__load_ge:
	SUB r3, r3, r4
	SUB r0, r0, r3
__reloc_go_off:
	BX	r0
__entry_reloc:*/
	// Clear TTBR1
	ADR	r0, _reset
	LDR r3, __ttbr1_addr
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	MOV r1, #0
	MOV	r3, #0
LCC:CMP	r3, #4096
	BGE	DCC
	STR	r1, [r0], #4
	ADD r3, r3, #1
	B LCC
DCC:
	// Clear TTBR0
//	ADR	r2, _reset
//	LDR r3, __ttbr0_addr
//	LDR r4, __reset_addr
//	SUB r3, r3, r4
//	ADD r2, r2, r3

//	MOV r1, #0
//	MOV	r3, #0
//LC: CMP	r3, #2048
//	BGE	DC
//	STR	r1, [r2], #4
//	ADD r3, r3, #1
//	B LC
//DC:
	// Create 1:1 mapping 70000000 -> C0000000
	ADR	r0, _reset
	LDR r3, __ttbr1_addr
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	//LDR r0, =0x70204000
	//SUB r0, r0, #24576			// start of TTBR1
	LDR r1, __virt_load_addr
	LDR r2, __mb_mask
	AND r1, r1, r2
	MOV r1, r1, LSR #18
	ADD r0, r0, r1
	//ADD r0, r0, #12288				// start of 0xC0000000
	LDR r1, __phys_target_addr 			// physical addr 0x7....
	ADD	r1, r1, #0x02
	ORR	r1, r1, #3 << 10
	// r1 = entry value str to current r0! 512 times
	MOV	r2, #0
	LDR r3, __kernel_mb_size
LR:	CMP	r2, r3 // #12
	BGE	DR
	STR	r1, [r0], #4
	//ADD r0, r0, #4 				// next page entry
	ADD r1, r1, #0x100000			// add 1 MB to physical addr
	ADD r2, r2, #1
	B LR
DR: // Create 1:1 mapping 70000000 -> B0000000
	//ADR	r0, __ttbr0
	//SUB r0, r0, #40960			// start of TTBR0
	ADR	r0, _reset
	//LDR r3, __ttbr0_addr
	LDR r3, __ttbr1_addr			// exchange ....
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	//LDR r0, =0x70208000
	//ADD r0, r0, #7168		uncomment again ...		// start of 0x70000000
	LDR r1, __phys_load_addr
	LDR r2, __mb_mask
	AND r1, r1, r2
	MOV r1, r1, LSR #18
	ADD r0, r0, r1
	//BL print_r
	//ADD r0, r0, #11264
	LDR r1, __phys_target_addr 			// physical addr
	ADD	r1, r1, #0x02
	ORR	r1, r1, #3 << 10
	// r1 = entry value str to current r0! 512 times
	MOV	r2, #0
	LDR r3, __kernel_mb_size
LI:	CMP	r2, r3 // #12
	BGE	DI
	STR	r1, [r0], #4
	//ADD r0, r0, #4 				// next page entry
	ADD r1, r1, #0x100000			// add 1 MB to physical addr
	ADD r2, r2, #1
	B LI
DI: // Map IO 0x53F00000 -> 0x53F00000
	//ADR	r0, __ttbr0
	//SUB r0, r0, #40960			// start of page table
	ADR	r0, _reset
	LDR r3, __ttbr1_addr
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	//LDR r0, =0x70208000
	//LDR r1, =0x14FC
	LDR r1, __phys_early_io
	LDR r2, __mb_mask
	AND r1, r1, r2
	MOV r1, r1, LSR #18
	ADD r0, r0, r1
	LDR r1, __phys_early_io 			// early output ... physical addr
	LDR r2, __mb_mask
	AND r1, r1, r2
	MOV r2, #0xFF
	MOV r2, r2, LSL #4
	ADD r2, #0xF
	MOV r2, r2, LSL #20
	AND r1, r1, r2
	ADD	r1, r1, #0x02
	ORR	r1, r1, #3 << 10
	STR	r1, [r0], #4
	// relocate to target memory
__start_reloc:
	// Check reloc conditions
	LDR r0, __phys_load_addr
	LDR r1, __phys_target_addr
	LDR r2, __kernel_b_size
	CMP	r0, r1
	BGE	__done_reloc
__reloc_copy:
	// Perform copy ...
	LDR     r3, [r0], #4
    STR     r3, [r1], #4
    SUBS    r2, r2, #4
    BNE     __reloc_copy
__done_reloc:
	//ADR	r0, __ttbr0
	//SUB r0, r0, #40960			// start of TTBR0
	ADR	r0, _reset
	LDR r3, __ttbr1_addr			// ttbr0
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	LDR r1, __phys_load_addr
	LDR r2, __phys_target_addr
	SUB r2, r2, r1
	ADD r0, r0, r2
	MCR p15, 0, r0, c2, c0, 0		// Set TTBR0
	//ADR	r0, __ttbr1
	//SUB r0, r0, #24576			// start of TTBR1
	/*ADR	r0, _reset
	LDR r3, __ttbr1_addr
	LDR r4, __reset_addr
	SUB r3, r3, r4
	ADD r0, r0, r3
	LDR r1, __phys_load_addr
	LDR r2, __phys_target_addr
	SUB r2, r2, r1
	ADD r0, r0, r2*/
	//ADD r0, r0, #8192
	MCR p15, 0, r0, c2, c0, 1		// SET TTBR1
	MOV r0, #1
	MCR p15, 0, r0, c2, c0, 2		// SET TTBRC
	LDR r0, =0x55555555
	MCR p15, 0, r0, c3, c0, 0		// Domain Access
	MOV r0, #0
	MCR p15, 0, r0, c8, c7, 0
	//LDR r1, =__svc_vector
	//MCR p15, 0, r1, c12, c0, 0
	MRC    p15, 0, r1, c1, c0, 0
	ORR    r1, r1, #1
	BIC    r1, r1, #(1 << 1)
	//ORR    r1, r1, #(1 << 2)
	ORR    r1, r1, #(1 << 3)
	ORR    r1, r1, #(1 << 11)
	ORR    r1, r1, #(1 << 12)
	BIC    r1, r1, #(1 << 13)
	//ORR    r1, r1, #(1 << 22)
	//ORR    r1, r1, #(1 << 23)
	BIC    r1, r1, #(1 << 29)
	//dbg_print
	MCR    p15, 0, r1, c1, c0, 0	// activate VM
	dbg_print
	dbg_print
	dbg_print
//__setup_vector:

//	MCR p15, 0, r1, c12, c0, 0
goc:
	MOV r0, r7
	MOV r1, r6
	LDR	pc, __entry_addr

.align 5
.global __svc_vector
__svc_vector:
	/*
	 * SVC vector table
	 */
	LDR		pc, __reset_addr 	// Reset
	LDR		pc, __undef_addr	// undefined instruction
	LDR		pc, __swi_addr		// software unterrupt
	LDR		pc, __pref_ab_addr	// prefetch abort
	LDR		pc, __data_ab_addr	// data abort
	B		.					// reserved
	LDR		pc, __irq_addr		// IRQ
	LDR		pc, __fiq_addr		// FIQ

.global _no_handler
_no_handler:
	B	_no_handler

.global __phys_entry_addr
.global __entry_addr
.global __phys_load_addr
.global __phys_target_addr
.global __virt_load_addr
.global __kernel_mb_size

__phys_entry_addr:	.WORD	entry
__phys_load_addr:	.WORD	0xA0000000
__phys_target_addr:	.WORD	0xAAAAAAAA //0xB0000000 // BOARD SPECIFIC
__virt_load_addr:	.WORD	0xA0000000
__phys_early_io:	.WORD	0xAAAAAAAA //0x53F00000 // BOARD SPECIFIC
__reloc_ttbr1:		.WORD	0xA0000000
__entry_addr:		.WORD	entry
__kernel_b_size:	.WORD	0x0
__board_infos_size:	.WORD	0x24
__kernel_mb_size:	.WORD	0x0
__reset_addr:		.WORD	_reset
__undef_addr:		.WORD	_undef
__swi_addr:			.WORD	_swi
__pref_ab_addr:		.WORD	_pref_ab
__data_ab_addr:		.WORD	_data_ab
__irq_addr:			.WORD	_irq
__fiq_addr:			.WORD	_fiq
__ttbr1_addr:		.WORD	__ttbr1
__test_addr:		.WORD	0x021E8000
__mb_mask:			.WORD	0xFFE00000
//__ttbr0_addr:		.WORD	__ttbr0

__board_infos:
// IMX53 QSB board
.WORD	0x00000cc9
.WORD	0xB0000000
.WORD	0x53F00000

// VEXPRESS A9
.WORD	0x000008e0
.WORD	0x60010000
.WORD	0x10009000

// IMX6Q SabreLite
.WORD	0x00000eb9
.WORD	0x40000000
.WORD	0x021E8000

.global __reset_addr
.global __undef_addr
.global __swi_addr
.global __pref_ab_addr
.global __data_ab_addr
.global __irq_addr
.global __fiq_addr

.global _swi
_swi:
	swi_entry

	LDR r0,[lr,#-4]
    BIC r0,r0,#0xff000000
	MOV r1, sp
	BL	dispatch_swi

	swi_return

.global _irq
_irq:
	iaik_msr_prologue 0x4

	MOV r0, sp
	BL	pIRQ

	swi_return

.global _fiq
_fiq:
	iaik_msr_prologue 0x4

	MOV r0, sp
	BL	pIRQ

	swi_return

.global print_r
print_r:
	# prolog
    stmdb sp!, {r4-r11}
    mov	r8, r0
    ldr	r5, __test_addr
	and	r8, r8, #-268435456
	lsr	r8, r8, #28
	cmp	r8, #9
	bhi	__i000
	add	r9, r8, #48
	b	__i001
__i000:
	add	r9, r8, #55
__i001:
	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #251658240
  	lsr	r8, r8, #24
 	cmp	r8, #9
 	bhi	__i002
  	add	r9, r8, #48
  	b	__i003
__i002:
  	add	r9, r8, #55
__i003:
	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF00000
  	lsr	r8, r8, #20
 	cmp	r8, #9
 	bhi	__i004
  	add	r9, r8, #48
  	b	__i005
__i004:
  	add	r9, r8, #55
__i005:
	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF0000
  	lsr	r8, r8, #16
 	cmp	r8, #9
 	bhi	__i006
  	add	r9, r8, #48
  	b	__i007
__i006:
  	add	r9, r8, #55
__i007:
	str	r9, [r5,#0x40]
  	str	r8, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF000
  	lsr	r8, r8, #12
 	cmp	r8, #9
 	bhi	__i008
  	add	r9, r8, #48
  	b	__i009
__i008:
  	add	r9, r8, #55
__i009:
  	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF00
  	lsr	r8, r8, #8
 	cmp	r8, #9
 	bhi	__i010
  	add	r9, r8, #48
  	b	__i011
__i010:
  	add	r9, r8, #55
__i011:
  	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF0
  	lsr	r8, r8, #4
 	cmp	r8, #9
 	bhi	__i012
  	add	r9, r8, #48
  	b	__i013
__i012:
  	add	r9, r8, #55
__i013:
  	str	r9, [r5,#0x40]
  	mov	r8, r0
  	and	r8, r8, #0xF
 	cmp	r8, #9
 	bhi	__i014
  	add	r9, r8, #48
  	b	__i015
__i014:
  	add	r9, r8, #55
__i015:
  	str	r9, [r5,#0x40]
  	MOV r9, #10
  	str	r9, [r5,#0x40]
 	ldmia sp!, {r4-r11}
    mov pc, lr

.global _pref_ab
_pref_ab:
	//B	_no_handler
	iaik_msr_prologue 0x8

	MOV r0, sp
	BL	pPA
	B	.

	iaik_msr_epilogue


.global _undef
_undef:
	//B	_no_handler
	iaik_msr_prologue 0

	MOV r0, sp
	BL	pUA
	B	.

	iaik_msr_epilogue

.global _data_ab
_data_ab:
	//B	_no_handler
	iaik_msr_prologue 0x8

	MOV r0, sp
	BL	pDA
	B	.

	iaik_msr_epilogue

// Handler Stack setup
.global _svc_stack_addr
_svc_stack_addr:
.4byte svc_stack
/*
.global _irq_stack_addr
_irq_stack_addr:
.4byte irq_stack

.global _da_stack_addr
_da_stack_addr:
.4byte da_stack

.global _fiq_stack_addr
_fiq_stack_addr:
.4byte fiq_stack

.global _sys_stack_addr
_sys_stack_addr:
.4byte sys_stack*/


.section .bss
// Stacks grow down, so label is at the end!
.space SYS_STACK_SIZE
.global svc_stack
svc_stack:
/*.space DA_STACK_SIZE
.global da_stack
da_stack:
.space IRQ_STACK_SIZE
.global irq_stack
irq_stack:
.space FIQ_STACK_SIZE
.global fiq_stack
fiq_stack:
.space SYS_STACK_SIZE
.global sys_stack
sys_stack:*/
.align 14
.global __ttbr1
__ttbr1:
	.space 16384
//.align 14
//.global __ttbr0
//__ttbr0:
//	.space 16384
