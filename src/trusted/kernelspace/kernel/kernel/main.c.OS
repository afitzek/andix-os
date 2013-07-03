	.syntax unified
	.cpu cortex-a8
	.eabi_attribute 6, 10
	.eabi_attribute 7, 65
	.eabi_attribute 8, 1
	.eabi_attribute 9, 2
	.fpu neon
	.eabi_attribute 10, 3
	.eabi_attribute 12, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.file	"kernel/main.c"
	.section	.debug_info,"",%progbits
.Lsection_info:
	.section	.debug_abbrev,"",%progbits
.Lsection_abbrev:
	.section	.debug_aranges,"",%progbits
	.section	.debug_macinfo,"",%progbits
	.section	.debug_line,"",%progbits
.Lsection_line:
	.section	.debug_loc,"",%progbits
	.section	.debug_pubtypes,"",%progbits
	.section	.debug_str,"MS",%progbits,1
.Linfo_string:
	.section	.debug_ranges,"",%progbits
.Ldebug_range:
	.section	.debug_loc,"",%progbits
.Lsection_debug_loc:
	.text
.Ltext_begin:
	.data
	.file	1 "kernel/main.c"
	.file	2 "/home/andy/UNI/DiplArbeit/andix/include/trusted/kernelspace/kernel/kprintf.h"
	.file	3 "/home/andy/UNI/DiplArbeit/andix/include/trusted/kernelspace/kernel/task/task.h"
	.text
	.globl	dummy_irq_handler
	.align	2
	.type	dummy_irq_handler,%function
dummy_irq_handler:
.Lfunc_begin0:
	.loc	1 48 0
	.loc	1 48 0
	push	{r4, r11, lr}
	add	r11, sp, #4
	sub	sp, sp, #12
	mov	r4, r0
.Ltmp0:
	.loc	1 49 0 prologue_end
	bl	gettaskname
	mov	r1, #49
	movw	r2, :lower16:.L__func__.dummy_irq_handler
	str	r1, [sp]
	movw	r1, :lower16:.L.str1
	movw	r3, :lower16:.L.str2
	str	r0, [sp, #4]
	movw	r0, :lower16:.L.str
	movt	r1, :upper16:.L.str1
	movt	r0, :upper16:.L.str
	movt	r2, :upper16:.L__func__.dummy_irq_handler
	movt	r3, :upper16:.L.str2
	bl	kprintf
	movw	r0, :lower16:.L.str3
	mov	r1, r4
	movt	r0, :upper16:.L.str3
	bl	kprintf
	movw	r0, :lower16:.L.str4
	movt	r0, :upper16:.L.str4
	bl	kprintf
	.loc	1 50 0
	mov	r0, r4
	bl	irq_clear
	.loc	1 51 0
	sub	sp, r11, #4
	pop	{r4, r11, pc}
.Ltmp1:
.Ltmp2:
	.size	dummy_irq_handler, .Ltmp2-dummy_irq_handler
.Lfunc_end0:

	.globl	entry
	.align	2
	.type	entry,%function
entry:
.Lfunc_begin1:
	.loc	1 58 0
	.loc	1 58 0
	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
	add	r11, sp, #28
	sub	sp, sp, #20
.Ltmp3:
	str	r0, [sp, #8]
.Ltmp4:
	.loc	1 69 0 prologue_end
	movw	r0, :lower16:__virt_load_addr
	movt	r0, :upper16:__virt_load_addr
	str	r1, [sp, #16]
	ldr	r0, [r0]
	.loc	1 74 0
	str	r0, [sp, #12]
	mov	r0, r1
.Ltmp5:
	bl	init_serial
	.loc	1 76 0
	bl	init_early_task
	.loc	1 79 0
	bl	gettaskname
	movw	r6, :lower16:.L.str
	movw	r10, :lower16:.L.str1
	movw	r7, :lower16:.L__func__.entry
	movw	r9, :lower16:.L.str2
	mov	r1, #80
	movt	r7, :upper16:.L__func__.entry
	movt	r9, :upper16:.L.str2
	movt	r6, :upper16:.L.str
	movt	r10, :upper16:.L.str1
	str	r1, [sp]
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r1, r10
	mov	r2, r7
	mov	r3, r9
	bl	kprintf
	movw	r8, :lower16:.L.str5
	movt	r8, :upper16:.L.str5
	mov	r0, r8
	bl	kprintf
	movw	r5, :lower16:.L.str4
	movt	r5, :upper16:.L.str4
	mov	r0, r5
	bl	kprintf
	.loc	1 81 0
	bl	gettaskname
	mov	r1, #81
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r4, :lower16:.L.str6
	movt	r4, :upper16:.L.str6
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 83 0
	bl	gettaskname
	mov	r1, #83
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str7
	movw	r1, :lower16:.L.str8
	movt	r0, :upper16:.L.str7
	movt	r1, :upper16:.L.str8
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 86 0
	bl	gettaskname
	mov	r1, #86
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str9
	movw	r1, :lower16:.L.str10
	movw	r2, :lower16:.L.str11
	movt	r0, :upper16:.L.str9
	movt	r1, :upper16:.L.str10
	movt	r2, :upper16:.L.str11
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 92 0
	bl	gettaskname
	mov	r1, #92
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str12
	movw	r1, :lower16:.L.str13
	movt	r0, :upper16:.L.str12
	movt	r1, :upper16:.L.str13
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 95 0
	bl	gettaskname
	mov	r1, #95
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str14
	movw	r1, :lower16:.L.str15
	movt	r0, :upper16:.L.str14
	movt	r1, :upper16:.L.str15
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 98 0
	bl	gettaskname
	mov	r1, #98
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 99 0
	bl	gettaskname
	mov	r1, #100
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r8
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 103 0
	bl	gettaskname
	mov	r1, #103
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 104 0
	bl	gettaskname
	mov	r1, #104
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str16
	movt	r0, :upper16:.L.str16
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 105 0
	bl	gettaskname
	mov	r1, #106
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str17
	movt	r0, :upper16:.L.str17
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 107 0
	bl	gettaskname
	mov	r1, #107
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 108 0
	bl	gettaskname
	mov	r1, #109
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str18
	movt	r0, :upper16:.L.str18
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 110 0
	bl	gettaskname
	mov	r1, #111
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str19
	movt	r0, :upper16:.L.str19
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 112 0
	bl	gettaskname
	mov	r1, #113
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str20
	movt	r0, :upper16:.L.str20
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 114 0
	bl	gettaskname
	mov	r1, #114
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str21
	movt	r0, :upper16:.L.str21
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 115 0
	bl	gettaskname
	mov	r1, #115
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 116 0
	bl	gettaskname
	mov	r1, #117
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str22
	movt	r0, :upper16:.L.str22
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 118 0
	bl	gettaskname
	mov	r1, #119
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str23
	movt	r0, :upper16:.L.str23
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 120 0
	bl	gettaskname
	mov	r1, #121
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str24
	movt	r0, :upper16:.L.str24
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 122 0
	bl	gettaskname
	mov	r1, #122
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str25
	movt	r0, :upper16:.L.str25
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 123 0
	bl	gettaskname
	mov	r1, #123
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 124 0
	bl	gettaskname
	mov	r1, #125
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str26
	movt	r0, :upper16:.L.str26
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 126 0
	bl	gettaskname
	mov	r1, #127
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str27
	movt	r0, :upper16:.L.str27
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 128 0
	bl	gettaskname
	mov	r1, #128
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 129 0
	bl	gettaskname
	mov	r1, #130
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	mov	r0, r8
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 136 0
	bl	gettaskname
	mov	r1, #136
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r4, :lower16:.L.str29
	movw	r0, :lower16:.L.str28
	movt	r4, :upper16:.L.str29
	movt	r0, :upper16:.L.str28
	mov	r1, r4
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 137 0
	bl	gettaskname
	movw	r8, :lower16:.L.str30
	mov	r1, #139
	movt	r8, :upper16:.L.str30
	str	r1, [sp]
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r1, r10
	mov	r2, r7
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:_code
	movw	r2, :lower16:_code_end
	movt	r1, :upper16:_code
	movt	r2, :upper16:_code_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str31
	movt	r0, :upper16:.L.str31
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 140 0
	bl	gettaskname
	mov	r1, #142
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:_data
	movw	r2, :lower16:_data_end
	movt	r1, :upper16:_data
	movt	r2, :upper16:_data_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str32
	movt	r0, :upper16:.L.str32
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 143 0
	bl	gettaskname
	mov	r1, #145
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:_rodata
	movw	r2, :lower16:_rodata_end
	movt	r1, :upper16:_rodata
	movt	r2, :upper16:_rodata_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str33
	movt	r0, :upper16:.L.str33
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 146 0
	bl	gettaskname
	mov	r1, #148
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:_bss
	movw	r2, :lower16:_bss_end
	movt	r1, :upper16:_bss
	movt	r2, :upper16:_bss_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str34
	movt	r0, :upper16:.L.str34
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 149 0
	bl	gettaskname
	mov	r1, #151
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:__symbol_table
	movw	r2, :lower16:__symbol_table_end
	movt	r1, :upper16:__symbol_table
	movt	r2, :upper16:__symbol_table_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str35
	movt	r0, :upper16:.L.str35
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 152 0
	bl	gettaskname
	mov	r1, #154
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:payload
	movw	r2, :lower16:payload_end
	movt	r1, :upper16:payload
	movt	r2, :upper16:payload_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str36
	movt	r0, :upper16:.L.str36
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 155 0
	bl	gettaskname
	mov	r1, #157
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r1, :lower16:dtb
	movw	r2, :lower16:dtb_end
	movt	r1, :upper16:dtb
	movt	r2, :upper16:dtb_end
	sub	r0, r2, r1
	asr	r3, r0, #31
	add	r3, r0, r3, lsr #30
	movw	r0, :lower16:.L.str37
	movt	r0, :upper16:.L.str37
	bfc	r3, #0, #2
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 158 0
	bl	gettaskname
	mov	r1, #158
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str38
	movw	r1, :lower16:svc_stack
	movt	r0, :upper16:.L.str38
	movt	r1, :upper16:svc_stack
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 159 0
	bl	gettaskname
	mov	r1, #159
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	.loc	1 69 0
	movw	r0, :lower16:__virt_load_addr
	movt	r0, :upper16:__virt_load_addr
	.loc	1 159 0
	ldr	r1, [r0]
	movw	r0, :lower16:.L.str39
	movt	r0, :upper16:.L.str39
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 160 0
	bl	gettaskname
	mov	r1, #160
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:__phys_target_addr
	movt	r0, :upper16:__phys_target_addr
	ldr	r1, [r0]
	movw	r0, :lower16:.L.str40
	movt	r0, :upper16:.L.str40
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 161 0
	bl	gettaskname
	mov	r1, #161
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:__kernel_mb_size
	movt	r0, :upper16:__kernel_mb_size
	ldr	r1, [r0]
	movw	r0, :lower16:.L.str41
	movt	r0, :upper16:.L.str41
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 162 0
	bl	gettaskname
	mov	r1, #162
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str42
	movw	r1, :lower16:_end
	movt	r0, :upper16:.L.str42
	movt	r1, :upper16:_end
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 164 0
	bl	gettaskname
	mov	r1, #164
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str43
	mov	r1, r4
	movt	r0, :upper16:.L.str43
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 170 0
	bl	gettaskname
	mov	r1, #170
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str44
	mov	r1, r4
	movt	r0, :upper16:.L.str44
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 172 0
	bl	gettaskname
	mov	r1, #172
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	ldr	r1, [sp, #16]
	movw	r0, :lower16:.L.str45
	movt	r0, :upper16:.L.str45
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 174 0
	bl	gettaskname
	mov	r1, #174
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	ldr	r4, [sp, #8]
.Ltmp6:
	movw	r0, :lower16:.L.str46
	movt	r0, :upper16:.L.str46
	mov	r1, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 176 0
	mov	r0, r4
	bl	map_atags
	mov	r6, r0
.Ltmp7:
	.loc	1 178 0
	bl	gettaskname
	mov	r1, #178
	mov	r2, r7
	str	r1, [sp]
	mov	r1, r10
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r9
	bl	kprintf
	movw	r0, :lower16:.L.str47
	mov	r1, r6
	movt	r0, :upper16:.L.str47
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 181 0
.Ltmp8:
	ldr	r4, [r6, #4]
.Ltmp9:
	.loc	1 182 0
	bl	gettaskname
	movw	r1, #1
	movt	r1, #21569
.Ltmp10:
	.loc	1 181 0
	cmp	r4, r1
	bne	.LBB1_2
	mov	r1, #182
	.loc	1 182 0
.Ltmp11:
	movw	r2, :lower16:.L__func__.entry
	str	r1, [sp]
	movw	r1, :lower16:.L.str1
	movw	r3, :lower16:.L.str2
	str	r0, [sp, #4]
	movw	r0, :lower16:.L.str30
	movt	r1, :upper16:.L.str1
	movt	r0, :upper16:.L.str30
	movt	r2, :upper16:.L__func__.entry
	movt	r3, :upper16:.L.str2
	bl	kprintf
	movw	r0, :lower16:.L.str48
	movt	r0, :upper16:.L.str48
	b	.LBB1_3
.Ltmp12:
.LBB1_2:
	mov	r1, #184
	.loc	1 184 0
.Ltmp13:
	movw	r2, :lower16:.L__func__.entry
	str	r1, [sp]
	movw	r1, :lower16:.L.str1
	movw	r3, :lower16:.L.str2
	str	r0, [sp, #4]
	movw	r0, :lower16:.L.str30
	movt	r1, :upper16:.L.str1
	movt	r0, :upper16:.L.str30
	movt	r2, :upper16:.L__func__.entry
	movt	r3, :upper16:.L.str2
	bl	kprintf
	movw	r0, :lower16:.L.str49
	movt	r0, :upper16:.L.str49
.Ltmp14:
.LBB1_3:
	bl	kprintf
	movw	r0, :lower16:.L.str4
	movt	r0, :upper16:.L.str4
	bl	kprintf
.Ltmp15:
	.loc	1 187 0
	bl	unmap_virtual_phys_kern
	.loc	1 189 0
	bl	gettaskname
	mov	r1, #189
	movw	r2, :lower16:.L__func__.entry
	str	r1, [sp]
	movw	r1, :lower16:.L.str1
	movw	r3, :lower16:.L.str2
	str	r0, [sp, #4]
	movw	r0, :lower16:.L.str
	movt	r1, :upper16:.L.str1
	movt	r0, :upper16:.L.str
	movt	r2, :upper16:.L__func__.entry
	movt	r3, :upper16:.L.str2
	mov	r8, r0
	mov	r5, r1
	mov	r10, r2
	mov	r4, r3
	bl	kprintf
	movw	r7, :lower16:.L.str29
	movw	r0, :lower16:.L.str50
	movt	r7, :upper16:.L.str29
	movt	r0, :upper16:.L.str50
	mov	r1, r7
	mov	r2, r7
	bl	kprintf
	movw	r0, :lower16:.L.str4
	movt	r0, :upper16:.L.str4
	mov	r9, r0
	bl	kprintf
	.loc	1 195 0
	bl	gettaskname
	mov	r1, #195
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str51
	mov	r1, r7
	movt	r0, :upper16:.L.str51
	mov	r2, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	mov	r0, r6
.Ltmp16:
	.loc	1 197 0
	str	r0, [sp, #8]
.Ltmp17:
	bl	init_vmm
	.loc	1 199 0
	bl	gettaskname
	mov	r1, #199
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str52
	mov	r1, r7
	movt	r0, :upper16:.L.str52
	mov	r2, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 201 0
	bl	gettaskname
	mov	r1, #201
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str53
	mov	r1, r7
	movt	r0, :upper16:.L.str53
	mov	r2, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 203 0
	movw	r0, :lower16:_code
	movw	r1, :lower16:_end
	movt	r0, :upper16:_code
	movt	r1, :upper16:_end
	bl	dump_kernel_mmu
	.loc	1 205 0
	bl	dump_mmu_full
	.loc	1 207 0
	bl	gettaskname
	mov	r1, #207
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str54
	mov	r1, r7
	movt	r0, :upper16:.L.str54
	mov	r2, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 209 0
	bl	gettaskname
	mov	r1, #209
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str55
	mov	r1, r7
	movt	r0, :upper16:.L.str55
	mov	r2, r1
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 211 0
	bl	setup_mode_stacks
	.loc	1 212 0
	bl	setup_mon_stacks
	.loc	1 214 0
	bl	gettaskname
	movw	r6, :lower16:.L.str30
	mov	r1, #214
	movt	r6, :upper16:.L.str30
	str	r1, [sp]
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r1, r5
	mov	r2, r10
	mov	r3, r4
	bl	kprintf
	movw	r7, :lower16:__svc_vector
	movw	r0, :lower16:.L.str56
	movt	r7, :upper16:__svc_vector
.Ltmp18:
	movt	r0, :upper16:.L.str56
	mov	r1, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 216 0
	@APP
	MCR p15, 0, r7, c12, c0, 0
	@NO_APP
	.loc	1 219 0
	bl	gettaskname
	mov	r1, #219
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r4
	bl	kprintf
	movw	r7, :lower16:__monitor_vector
.Ltmp19:
	movw	r0, :lower16:.L.str57
	movt	r7, :upper16:__monitor_vector
.Ltmp20:
	movt	r0, :upper16:.L.str57
	mov	r1, r7
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 221 0
	@APP
	MCR p15, 0, r7, c12, c0, 1
	@NO_APP
	.loc	1 224 0
	@APP
	MRC p15, 0, r7, c12, c0, 1
	@NO_APP
.Ltmp21:
	.loc	1 227 0
	bl	gettaskname
	mov	r1, #227
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r6
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str58
	mov	r1, r7
	movt	r0, :upper16:.L.str58
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 229 0
	bl	gettaskname
	mov	r1, #229
	mov	r7, r8
.Ltmp22:
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r7
	mov	r2, r10
	mov	r3, r4
	bl	kprintf
	.loc	1 189 0
	movw	r1, :lower16:.L.str29
	.loc	1 229 0
	movw	r0, :lower16:.L.str59
	.loc	1 189 0
	movt	r1, :upper16:.L.str29
	.loc	1 229 0
	movt	r0, :upper16:.L.str59
	mov	r2, r1
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	ldr	r6, [sp, #8]
.Ltmp23:
	.loc	1 231 0
	mov	r0, r6
	bl	atag_copy
	mov	r8, r0
.Ltmp24:
	.loc	1 234 0
	mov	r0, r6
	mov	r1, #256
	bl	unmap_memory
	.loc	1 237 0
	bl	gettaskname
	mov	r1, #237
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r7
	mov	r3, r4
	bl	kprintf
	movw	r0, :lower16:.L.str60
	mov	r1, r8
	movt	r0, :upper16:.L.str60
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 238 0
	mov	r0, r8
	bl	atag_dump
	.loc	1 245 0
	bl	gettaskname
	mov	r1, #245
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r7
	mov	r3, r4
	bl	kprintf
	.loc	1 189 0
	movw	r1, :lower16:.L.str29
	.loc	1 245 0
	movw	r0, :lower16:.L.str61
	.loc	1 189 0
	movt	r1, :upper16:.L.str29
	.loc	1 245 0
	movt	r0, :upper16:.L.str61
	mov	r2, r1
	bl	kprintf
	mov	r0, r9
	bl	kprintf
	.loc	1 247 0
	ldr	r0, [sp, #16]
	mov	r1, r8
	bl	hal_init
	.loc	1 249 0
	bl	clean_user
	.loc	1 251 0
	bl	random_init
	.loc	1 253 0
	bl	irq_init
	.loc	1 256 0
	bl	send_fiq_irq_to_monitor
	.loc	1 257 0
	bl	enable_irq
	.loc	1 258 0
	bl	enable_fiq
	.loc	1 260 0
.Ltmp25:
	bl	tzmem_init
	cmp	r0, #0
	beq	.LBB1_5
.Ltmp26:
	.loc	1 261 0
	bl	gettaskname
	movw	r1, #261
	movw	r2, :lower16:.L__func__.entry
	str	r1, [sp]
	movw	r1, :lower16:.L.str1
	movw	r3, :lower16:.L.str2
	str	r0, [sp, #4]
	movw	r0, :lower16:.L.str
	movt	r1, :upper16:.L.str1
	movt	r0, :upper16:.L.str
	movt	r2, :upper16:.L__func__.entry
	movt	r3, :upper16:.L.str2
	bl	kprintf
	movw	r0, :lower16:.L.str62
	movt	r0, :upper16:.L.str62
	bl	kprintf
	movw	r0, :lower16:.L.str4
	movt	r0, :upper16:.L.str4
	bl	kprintf
	b	.LBB1_8
.Ltmp27:
.LBB1_5:
	.loc	1 264 0
	bl	tzmem_dump
	.loc	1 266 0
.Ltmp28:
	bl	pmm_protect_secure_mem
	cmp	r0, #0
	beq	.LBB1_7
	.loc	1 267 0
.Ltmp29:
	bl	gettaskname
	movw	r9, :lower16:.L.str63
	movw	r5, :lower16:.L.str1
	movw	r6, :lower16:.L__func__.entry
	movw	r7, :lower16:.L.str2
	movw	r1, #267
	movt	r6, :upper16:.L__func__.entry
	movt	r7, :upper16:.L.str2
	movt	r9, :upper16:.L.str63
	movt	r5, :upper16:.L.str1
	str	r1, [sp]
	str	r0, [sp, #4]
	mov	r0, r9
	mov	r1, r5
	mov	r2, r6
	mov	r3, r7
	bl	kprintf
	movw	r8, :lower16:.L.str64
	movt	r8, :upper16:.L.str64
	mov	r0, r8
	bl	kprintf
	movw	r4, :lower16:.L.str4
	movt	r4, :upper16:.L.str4
	mov	r0, r4
	bl	kprintf
	.loc	1 268 0
	bl	gettaskname
	mov	r1, #268
	mov	r2, r6
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r9
	mov	r3, r7
	bl	kprintf
	movw	r0, :lower16:.L.str65
	movt	r0, :upper16:.L.str65
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	.loc	1 269 0
	bl	gettaskname
	movw	r1, #269
	mov	r2, r6
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r9
	mov	r3, r7
	bl	kprintf
	movw	r0, :lower16:.L.str66
	movt	r0, :upper16:.L.str66
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	.loc	1 270 0
	bl	gettaskname
	movw	r1, #270
	mov	r2, r6
	str	r1, [sp]
	mov	r1, r5
	str	r0, [sp, #4]
	mov	r0, r9
	mov	r3, r7
	bl	kprintf
	mov	r0, r8
	bl	kprintf
	mov	r0, r4
	bl	kprintf
	.loc	1 271 0
	bl	kpanic
.Ltmp30:
.LBB1_7:
	.loc	1 274 0
	bl	tzmem_dump
.Ltmp31:
.LBB1_8:
	.loc	1 277 0
	bl	gettaskname
	movw	r8, :lower16:.L.str
	movw	r9, :lower16:.L.str1
	movw	r10, :lower16:.L__func__.entry
	movw	r7, :lower16:.L.str2
	movw	r1, #277
	movt	r10, :upper16:.L__func__.entry
	movt	r7, :upper16:.L.str2
	movt	r8, :upper16:.L.str
	movt	r9, :upper16:.L.str1
	str	r1, [sp]
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r1, r9
	mov	r2, r10
	mov	r3, r7
	bl	kprintf
	movw	r4, :lower16:.L.str29
	movw	r0, :lower16:.L.str67
	movt	r4, :upper16:.L.str29
	movt	r0, :upper16:.L.str67
	mov	r1, r4
	mov	r2, r4
	bl	kprintf
	movw	r5, :lower16:.L.str4
	movt	r5, :upper16:.L.str4
	mov	r0, r5
	bl	kprintf
	.loc	1 283 0
	bl	gettaskname
	movw	r1, #283
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r9
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r7
	bl	kprintf
	movw	r0, :lower16:.L.str68
	mov	r1, r4
	movt	r0, :upper16:.L.str68
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 285 0
	bl	init_task
	.loc	1 286 0
	bl	init_scheduler
	mov	r0, #19
	mov	r1, #0
	.loc	1 288 0
	bl	create_kernel_task
	mov	r6, r0
.Ltmp32:
	.loc	1 289 0
	movw	r0, :lower16:entry_main_task
	.loc	1 290 0
	movw	r1, :lower16:.L.str69
	.loc	1 289 0
	movt	r0, :upper16:entry_main_task
	str	r0, [r6, #56]
	.loc	1 290 0
	movt	r1, :upper16:.L.str69
	mov	r0, r6
	bl	task_set_name
	.loc	1 291 0
	mov	r0, r6
	bl	add_task
	.loc	1 293 0
	bl	print_tasks
	.loc	1 295 0
	bl	gettaskname
	movw	r1, #295
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r9
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r7
	bl	kprintf
	.loc	1 160 0
	movw	r0, :lower16:__phys_target_addr
	movt	r0, :upper16:__phys_target_addr
	.loc	1 295 0
	ldr	r2, [r0]
	ldr	r0, [sp, #12]
	ldr	r1, [r0]
	movw	r0, :lower16:.L.str70
	movt	r0, :upper16:.L.str70
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 297 0
	mov	r0, r6
	bl	switch_to_task
	.loc	1 299 0
	bl	gettaskname
	movw	r1, #299
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r9
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r7
	bl	kprintf
	movw	r0, :lower16:.L.str71
	mov	r1, r4
	movt	r0, :upper16:.L.str71
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
	.loc	1 300 0
	bl	gettaskname
	movw	r1, #301
	mov	r2, r10
	str	r1, [sp]
	mov	r1, r9
	str	r0, [sp, #4]
	mov	r0, r8
	mov	r3, r7
	bl	kprintf
	movw	r0, :lower16:.L.str72
	mov	r1, r4
	movt	r0, :upper16:.L.str72
	mov	r2, r4
	bl	kprintf
	mov	r0, r5
	bl	kprintf
.Ltmp33:
.LBB1_9:
	.loc	1 302 0
	@APP
	NOP
	@NO_APP
	b	.LBB1_9
.Ltmp34:
.Ltmp35:
	.size	entry, .Ltmp35-entry
.Lfunc_end1:
	.file	4 "/home/andy/UNI/DiplArbeit/andix/include/trusted/kernelspace/kernel/ioctl/clock.h"

	.type	.L.str,%object
	.section	.rodata.str1.1,"aMS",%progbits,1
.L.str:
	.asciz	 "[INF][%s]->%s [%s(%d)] [%s]:"
	.size	.L.str, 29

	.type	.L.str1,%object
.L.str1:
	.asciz	 " main"
	.size	.L.str1, 6

	.type	.L__func__.dummy_irq_handler,%object
.L__func__.dummy_irq_handler:
	.asciz	 "dummy_irq_handler"
	.size	.L__func__.dummy_irq_handler, 18

	.type	.L.str2,%object
.L.str2:
	.asciz	 "kernel/main.c"
	.size	.L.str2, 14

	.type	.L.str3,%object
.L.str3:
	.asciz	 "dummy_irq_handler: %d"
	.size	.L.str3, 22

	.type	.L.str4,%object
.L.str4:
	.asciz	 "\n"
	.size	.L.str4, 2

	.type	.L__func__.entry,%object
.L__func__.entry:
	.asciz	 "entry"
	.size	.L__func__.entry, 6

	.type	.L.str5,%object
.L.str5:
	.asciz	 "============================================================================"
	.size	.L.str5, 77

	.type	.L.str6,%object
.L.str6:
	.zero	1
	.size	.L.str6, 1

	.type	.L.str7,%object
.L.str7:
	.asciz	 "      ANDIX (%s)"
	.size	.L.str7, 17

	.type	.L.str8,%object
.L.str8:
	.asciz	 "0.1.8821"
	.size	.L.str8, 9

	.type	.L.str9,%object
.L.str9:
	.asciz	 "      compiled by %s @ %s "
	.size	.L.str9, 27

	.type	.L.str10,%object
.L.str10:
	.asciz	 "andy"
	.size	.L.str10, 5

	.type	.L.str11,%object
.L.str11:
	.asciz	 "2013.07.03_17:24:57"
	.size	.L.str11, 20

	.type	.L.str12,%object
.L.str12:
	.asciz	 "      GIT Commit %s"
	.size	.L.str12, 20

	.type	.L.str13,%object
.L.str13:
	.asciz	 "09e1b096d7cf73212fa01d5f7ed97985a230a9e3"
	.size	.L.str13, 41

	.type	.L.str14,%object
.L.str14:
	.asciz	 "      build number: %s"
	.size	.L.str14, 23

	.type	.L.str15,%object
.L.str15:
	.asciz	 "8821"
	.size	.L.str15, 5

	.type	.L.str16,%object
.L.str16:
	.asciz	 "      ANDIX is an ARM TrustZone monitor operating system."
	.size	.L.str16, 58

	.type	.L.str17,%object
.L.str17:
	.asciz	 "      Copyright (C) 2013  Andreas Fitzek <andreas.fitzek[at]iaik.tugraz.at>"
	.size	.L.str17, 76

	.type	.L.str18,%object
.L.str18:
	.asciz	 "      This system is free software: you can redistribute it and/or modify"
	.size	.L.str18, 74

	.type	.L.str19,%object
.L.str19:
	.asciz	 "      it under the terms of the GNU General Public License as published by"
	.size	.L.str19, 75

	.type	.L.str20,%object
.L.str20:
	.asciz	 "      the Free Software Foundation, either version 3 of the License, or"
	.size	.L.str20, 72

	.type	.L.str21,%object
.L.str21:
	.asciz	 "      (at your option) any later version."
	.size	.L.str21, 42

	.type	.L.str22,%object
.L.str22:
	.asciz	 "      This program is distributed in the hope that it will be useful,"
	.size	.L.str22, 70

	.type	.L.str23,%object
.L.str23:
	.asciz	 "      but WITHOUT ANY WARRANTY; without even the implied warranty of"
	.size	.L.str23, 69

	.type	.L.str24,%object
.L.str24:
	.asciz	 "      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
	.size	.L.str24, 68

	.type	.L.str25,%object
.L.str25:
	.asciz	 "      GNU General Public License for more details."
	.size	.L.str25, 51

	.type	.L.str26,%object
.L.str26:
	.asciz	 "      You should have received a copy of the GNU General Public License"
	.size	.L.str26, 72

	.type	.L.str27,%object
.L.str27:
	.asciz	 "      along with this program.  If not, see <http://www.gnu.org/licenses/>."
	.size	.L.str27, 76

	.type	.L.str28,%object
.L.str28:
	.asciz	 "%s STATIC MEMORY LAYOUT %s"
	.size	.L.str28, 27

	.type	.L.str29,%object
.L.str29:
	.asciz	 "##########"
	.size	.L.str29, 11

	.type	.L.str30,%object
.L.str30:
	.asciz	 "[DBG][%s]->%s [%s(%d)] [%s]:"
	.size	.L.str30, 29

	.type	.L.str31,%object
.L.str31:
	.asciz	 "Code:      0x%x - 0x%x [%d byte]"
	.size	.L.str31, 33

	.type	.L.str32,%object
.L.str32:
	.asciz	 "Data:      0x%x - 0x%x [%d byte]"
	.size	.L.str32, 33

	.type	.L.str33,%object
.L.str33:
	.asciz	 "ROData:    0x%x - 0x%x [%d byte]"
	.size	.L.str33, 33

	.type	.L.str34,%object
.L.str34:
	.asciz	 "BSS:       0x%x - 0x%x [%d byte]"
	.size	.L.str34, 33

	.type	.L.str35,%object
.L.str35:
	.asciz	 "SYM:       0x%x - 0x%x [%d byte]"
	.size	.L.str35, 33

	.type	.L.str36,%object
.L.str36:
	.asciz	 "Payload:   0x%x - 0x%x [%d byte]"
	.size	.L.str36, 33

	.type	.L.str37,%object
.L.str37:
	.asciz	 "DTB:       0x%x - 0x%x [%d byte]"
	.size	.L.str37, 33

	.type	.L.str38,%object
.L.str38:
	.asciz	 "svc_stack: 0x%x"
	.size	.L.str38, 16

	.type	.L.str39,%object
.L.str39:
	.asciz	 "virt addr: 0x%x"
	.size	.L.str39, 16

	.type	.L.str40,%object
.L.str40:
	.asciz	 "phys addr: 0x%x"
	.size	.L.str40, 16

	.type	.L.str41,%object
.L.str41:
	.asciz	 "k MB size: 0x%x"
	.size	.L.str41, 16

	.type	.L.str42,%object
.L.str42:
	.asciz	 "_end:      0x%x"
	.size	.L.str42, 16

	.type	.L.str43,%object
.L.str43:
	.asciz	 "%s STATIC MEMORY LAYOUT [DONE] %s"
	.size	.L.str43, 34

	.type	.L.str44,%object
.L.str44:
	.asciz	 "%s BOOT PARAMETER %s"
	.size	.L.str44, 21

	.type	.L.str45,%object
.L.str45:
	.asciz	 "SysID: 0x%x"
	.size	.L.str45, 12

	.type	.L.str46,%object
.L.str46:
	.asciz	 "ATAGS @ physical 0x%x"
	.size	.L.str46, 22

	.type	.L.str47,%object
.L.str47:
	.asciz	 "ATAGS @ virtual 0x%x"
	.size	.L.str47, 21

	.type	.L.str48,%object
.L.str48:
	.asciz	 "ATAGS seem OK!"
	.size	.L.str48, 15

	.type	.L.str49,%object
.L.str49:
	.asciz	 "ATAGS not starting with ATAG_CORE!"
	.size	.L.str49, 35

	.type	.L.str50,%object
.L.str50:
	.asciz	 "%s BOOT PARAMETER [DONE] %s"
	.size	.L.str50, 28

	.type	.L.str51,%object
.L.str51:
	.asciz	 "%s MEMORY MANAGEMENT %s"
	.size	.L.str51, 24

	.type	.L.str52,%object
.L.str52:
	.asciz	 "%s MEMORY MANAGEMENT [DONE] %s"
	.size	.L.str52, 31

	.type	.L.str53,%object
.L.str53:
	.asciz	 "%s VIRTUAL MEMORY LAYOUT %s"
	.size	.L.str53, 28

	.type	.L.str54,%object
.L.str54:
	.asciz	 "%s VIRTUAL MEMORY LAYOUT [DONE] %s"
	.size	.L.str54, 35

	.type	.L.str55,%object
.L.str55:
	.asciz	 "%s EXCEPTION VECTORS %s"
	.size	.L.str55, 24

	.type	.L.str56,%object
.L.str56:
	.asciz	 "Setting SVC Exception vectors to: 0x%x"
	.size	.L.str56, 39

	.type	.L.str57,%object
.L.str57:
	.asciz	 "Setting MONITOR Exception vectors to: 0x%x"
	.size	.L.str57, 43

	.type	.L.str58,%object
.L.str58:
	.asciz	 "VECTORS: 0x%x"
	.size	.L.str58, 14

	.type	.L.str59,%object
.L.str59:
	.asciz	 "%s EXCEPTION VECTORS [DONE] %s"
	.size	.L.str59, 31

	.type	.L.str60,%object
.L.str60:
	.asciz	 "Atags copied to : @ 0x%x"
	.size	.L.str60, 25

	.type	.L.str61,%object
.L.str61:
	.asciz	 "%s HAL SYSTEM %s"
	.size	.L.str61, 17

	.type	.L.str62,%object
.L.str62:
	.asciz	 "No TZ memory controller booting anyway!"
	.size	.L.str62, 40

	.type	.L.str63,%object
.L.str63:
	.asciz	 "[ERR][%s]->%s [%s(%d)] [%s]:"
	.size	.L.str63, 29

	.type	.L.str64,%object
.L.str64:
	.asciz	 "====================================================="
	.size	.L.str64, 54

	.type	.L.str65,%object
.L.str65:
	.asciz	 "CANNOT PERFORM SECURE BOOT!"
	.size	.L.str65, 28

	.type	.L.str66,%object
.L.str66:
	.asciz	 "FAILED TO PROTECT ALL SECURE MEMORY!"
	.size	.L.str66, 37

	.type	.L.str67,%object
.L.str67:
	.asciz	 "%s HAL SYSTEM [DONE] %s"
	.size	.L.str67, 24

	.type	.L.str68,%object
.L.str68:
	.asciz	 "%s STARTING MAIN KERNEL TASK %s"
	.size	.L.str68, 32

	.type	.L.str69,%object
.L.str69:
	.asciz	 "MAIN KERNEL TASK"
	.size	.L.str69, 17

	.type	.L.str70,%object
.L.str70:
	.asciz	 "Protected MEM 0x%x @ 0x%x"
	.size	.L.str70, 26

	.type	.L.str71,%object
.L.str71:
	.asciz	 "%s STARTING MAIN KERNEL TASK [DONE] %s"
	.size	.L.str71, 39

	.type	.L.str72,%object
.L.str72:
	.asciz	 "%s SHOULD NOT BE HERE FREEZING SYSTEM ... %s"
	.size	.L.str72, 45

	.type	kputchar,%object
	.comm	kputchar,4,4
	.text
.Ltext_end:
	.data
.Ldata_end:
	.text
.Lsection_end1:
	.section	.debug_info,"",%progbits
.L.debug_info_begin0:
	.long	440
	.short	3
	.long	.L.debug_abbrev_begin
	.byte	4
	.byte	1
	.long	.Linfo_string0
	.short	12
	.long	.Linfo_string1
	.long	0
	.long	.Lsection_line
	.long	.Linfo_string2

	.byte	2
	.long	.Linfo_string4
	.byte	8
	.byte	1
	.byte	3

	.byte	4
	.long	34
	.byte	0
	.byte	5
	.long	41
	.byte	6
	.long	.Linfo_string3
	.long	48

	.byte	2
	.byte	30
	.byte	5
	.byte	3
	.long	kputchar
	.byte	7
	.long	.Linfo_string5
	.byte	1
	.byte	48


	.long	.Lfunc_begin0
	.long	.Lfunc_end0
	.byte	1
	.byte	91
	.byte	8
	.long	.Linfo_string9
	.byte	1
	.byte	48
	.long	340
	.long	.Ldebug_loc0
	.byte	0
	.byte	7
	.long	.Linfo_string6
	.byte	1
	.byte	58


	.long	.Lfunc_begin1
	.long	.Lfunc_end1
	.byte	1
	.byte	91
	.byte	8
	.long	.Linfo_string11
	.byte	1
	.byte	58
	.long	354
	.long	.Ldebug_loc3
	.byte	8
	.long	.Linfo_string14
	.byte	1
	.byte	58
	.long	354
	.long	.Ldebug_loc7
	.byte	9
	.long	.Linfo_string15
	.byte	1
	.byte	59
	.long	354
	.long	.Ldebug_loc11
	.byte	9
	.long	.Linfo_string16
	.byte	1
	.byte	61
	.long	354
	.long	.Ldebug_loc15
	.byte	9
	.long	.Linfo_string17
	.byte	1
	.byte	64
	.long	354
	.long	.Ldebug_loc25
	.byte	10
	.long	.Linfo_string18
	.byte	1
	.byte	63
	.long	354
	.byte	10
	.ascii	 "\377\377\377\377\377\377\377\377\377\001"
	.byte	9
	.long	.Linfo_string19
	.byte	1
	.byte	180
	.long	370
	.long	.Ldebug_loc28
	.byte	9
	.long	.Linfo_string21
	.byte	1
	.byte	65
	.long	354
	.long	.Ldebug_loc35
	.byte	9
	.long	.Linfo_string22
	.byte	1
	.byte	66
	.long	354
	.long	.Ldebug_loc37
	.byte	11
	.long	.Linfo_string23
	.byte	1
	.short	288
	.long	387
	.long	.Ldebug_loc39
	.byte	12
	.long	.Linfo_string25
	.byte	1
	.byte	67
	.long	392
	.byte	12
	.long	.Linfo_string26
	.byte	1
	.byte	68
	.long	397
	.byte	12
	.long	.Linfo_string28
	.byte	1
	.byte	69
	.long	392
	.byte	12
	.long	.Linfo_string29
	.byte	1
	.byte	70
	.long	432
	.byte	0
	.byte	13
	.byte	4
	.byte	3
	.byte	21
	.byte	14
	.long	.Linfo_string7
	.byte	0
	.byte	14
	.long	.Linfo_string8
	.byte	1
	.byte	0
	.byte	2
	.long	.Linfo_string10
	.byte	5
	.byte	4
	.byte	2
	.long	.Linfo_string12
	.byte	7
	.byte	4
	.byte	15
	.long	347
	.long	.Linfo_string13
	.byte	1
	.byte	24
	.byte	16
	.long	.Linfo_string20

	.byte	5
	.long	365
	.byte	17

	.byte	15
	.long	375
	.long	.Linfo_string24
	.byte	1
	.byte	72
	.byte	5
	.long	376
	.byte	5
	.long	354
	.byte	15
	.long	392
	.long	.Linfo_string27
	.byte	1
	.byte	27
	.byte	18
	.long	.Linfo_string29
	.byte	4
	.byte	4
	.byte	21
	.byte	19
	.long	.Linfo_string30
	.long	354
	.byte	4
	.byte	22
	.byte	2
	.byte	35
	.byte	0
	.byte	1
	.byte	0
	.byte	15
	.long	408
	.long	.Linfo_string31
	.byte	1
	.byte	25
	.byte	0
.L.debug_info_end0:
	.section	.debug_abbrev,"",%progbits
.L.debug_abbrev_begin:
	.byte	1
	.byte	17
	.byte	1
	.byte	37
	.byte	14
	.byte	19
	.byte	5
	.byte	3
	.byte	14
	.byte	17
	.byte	1
	.byte	16
	.byte	6
	.byte	27
	.byte	14
	.ascii	 "\341\177"
	.byte	25
	.byte	0
	.byte	0
	.byte	2
	.byte	36
	.byte	0
	.byte	3
	.byte	14
	.byte	62
	.byte	11
	.byte	11
	.byte	11
	.byte	0
	.byte	0
	.byte	3
	.byte	21
	.byte	1
	.byte	39
	.byte	25
	.byte	0
	.byte	0
	.byte	4
	.byte	5
	.byte	0
	.byte	73
	.byte	19
	.byte	0
	.byte	0
	.byte	5
	.byte	15
	.byte	0
	.byte	73
	.byte	19
	.byte	0
	.byte	0
	.byte	6
	.byte	52
	.byte	0
	.byte	3
	.byte	14
	.byte	73
	.byte	19
	.byte	63
	.byte	25
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	2
	.byte	10
	.byte	0
	.byte	0
	.byte	7
	.byte	46
	.byte	1
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	39
	.byte	25
	.byte	63
	.byte	25
	.byte	17
	.byte	1
	.byte	18
	.byte	1
	.byte	64
	.byte	10
	.byte	0
	.byte	0
	.byte	8
	.byte	5
	.byte	0
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	73
	.byte	19
	.byte	2
	.byte	6
	.byte	0
	.byte	0
	.byte	9
	.byte	52
	.byte	0
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	73
	.byte	19
	.byte	2
	.byte	6
	.byte	0
	.byte	0
	.byte	10
	.byte	52
	.byte	0
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	73
	.byte	19
	.byte	28
	.byte	10
	.byte	0
	.byte	0
	.byte	11
	.byte	52
	.byte	0
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	5
	.byte	73
	.byte	19
	.byte	2
	.byte	6
	.byte	0
	.byte	0
	.byte	12
	.byte	52
	.byte	0
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	73
	.byte	19
	.byte	0
	.byte	0
	.byte	13
	.byte	4
	.byte	1
	.byte	11
	.byte	11
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	0
	.byte	0
	.byte	14
	.byte	40
	.byte	0
	.byte	3
	.byte	14
	.byte	28
	.byte	13
	.byte	0
	.byte	0
	.byte	15
	.byte	22
	.byte	0
	.byte	73
	.byte	19
	.byte	3
	.byte	14
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	0
	.byte	0
	.byte	16
	.byte	19
	.byte	0
	.byte	3
	.byte	14
	.byte	60
	.byte	25
	.byte	0
	.byte	0
	.byte	17
	.byte	19
	.byte	0
	.byte	60
	.byte	25
	.byte	0
	.byte	0
	.byte	18
	.byte	19
	.byte	1
	.byte	3
	.byte	14
	.byte	11
	.byte	11
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	0
	.byte	0
	.byte	19
	.byte	13
	.byte	0
	.byte	3
	.byte	14
	.byte	73
	.byte	19
	.byte	58
	.byte	11
	.byte	59
	.byte	11
	.byte	56
	.byte	10
	.byte	50
	.byte	11
	.byte	0
	.byte	0
	.byte	0
.L.debug_abbrev_end:
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
	.long	.Lfunc_begin0
	.long	.Ltmp0
.Lset0 = .Ltmp37-.Ltmp36
	.short	.Lset0
.Ltmp36:
	.byte	80
.Ltmp37:
	.long	.Ltmp0
	.long	.Ltmp1
.Lset1 = .Ltmp39-.Ltmp38
	.short	.Lset1
.Ltmp38:
	.byte	84
.Ltmp39:
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Lfunc_begin1
	.long	.Ltmp4
.Lset2 = .Ltmp41-.Ltmp40
	.short	.Lset2
.Ltmp40:
	.byte	80
.Ltmp41:
	.long	.Ltmp4
	.long	.Ltmp6
.Lset3 = .Ltmp43-.Ltmp42
	.short	.Lset3
.Ltmp42:
	.byte	125
	.byte	8
.Ltmp43:
	.long	.Ltmp6
	.long	.Ltmp9
.Lset4 = .Ltmp45-.Ltmp44
	.short	.Lset4
.Ltmp44:
	.byte	84
.Ltmp45:
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Lfunc_begin1
	.long	.Ltmp5
.Lset5 = .Ltmp47-.Ltmp46
	.short	.Lset5
.Ltmp46:
	.byte	81
.Ltmp47:
	.long	.Ltmp5
	.long	.Lfunc_end1
.Lset6 = .Ltmp49-.Ltmp48
	.short	.Lset6
.Ltmp48:
	.byte	125
	.byte	16
.Ltmp49:
	.long	0
	.long	0
.Ldebug_loc11:
	.long	.Ltmp3
	.long	.Ltmp5
.Lset7 = .Ltmp51-.Ltmp50
	.short	.Lset7
.Ltmp50:
	.byte	81
.Ltmp51:
	.long	.Ltmp5
	.long	.Lfunc_end1
.Lset8 = .Ltmp53-.Ltmp52
	.short	.Lset8
.Ltmp52:
	.byte	125
	.byte	16
.Ltmp53:
	.long	0
	.long	0
.Ldebug_loc15:
	.long	.Ltmp3
	.long	.Ltmp4
.Lset9 = .Ltmp55-.Ltmp54
	.short	.Lset9
.Ltmp54:
	.byte	80
.Ltmp55:
	.long	.Ltmp4
	.long	.Ltmp6
.Lset10 = .Ltmp57-.Ltmp56
	.short	.Lset10
.Ltmp56:
	.byte	125
	.byte	8
.Ltmp57:
	.long	.Ltmp6
	.long	.Ltmp7
.Lset11 = .Ltmp59-.Ltmp58
	.short	.Lset11
.Ltmp58:
	.byte	84
.Ltmp59:
	.long	.Ltmp7
	.long	.Ltmp14
.Lset12 = .Ltmp61-.Ltmp60
	.short	.Lset12
.Ltmp60:
	.byte	86
.Ltmp61:
	.long	.Ltmp15
	.long	.Ltmp16
.Lset13 = .Ltmp63-.Ltmp62
	.short	.Lset13
.Ltmp62:
	.byte	86
.Ltmp63:
	.long	.Ltmp16
	.long	.Ltmp17
.Lset14 = .Ltmp65-.Ltmp64
	.short	.Lset14
.Ltmp64:
	.byte	80
.Ltmp65:
	.long	.Ltmp17
	.long	.Ltmp23
.Lset15 = .Ltmp67-.Ltmp66
	.short	.Lset15
.Ltmp66:
	.byte	125
	.byte	8
.Ltmp67:
	.long	.Ltmp23
	.long	.Ltmp24
.Lset16 = .Ltmp69-.Ltmp68
	.short	.Lset16
.Ltmp68:
	.byte	86
.Ltmp69:
	.long	.Ltmp24
	.long	.Ltmp26
.Lset17 = .Ltmp71-.Ltmp70
	.short	.Lset17
.Ltmp70:
	.byte	88
.Ltmp71:
	.long	0
	.long	0
.Ldebug_loc25:
	.long	.Ltmp4
	.long	.Ltmp21
.Lset18 = .Ltmp73-.Ltmp72
	.short	.Lset18
.Ltmp72:
	.byte	16
	.ascii	 "\377\377\377\377\377\377\377\377\377\001"
.Ltmp73:
	.long	.Ltmp21
	.long	.Ltmp22
.Lset19 = .Ltmp75-.Ltmp74
	.short	.Lset19
.Ltmp74:
	.byte	87
.Ltmp75:
	.long	0
	.long	0
.Ldebug_loc28:
	.long	.Ltmp7
	.long	.Ltmp14
.Lset20 = .Ltmp77-.Ltmp76
	.short	.Lset20
.Ltmp76:
	.byte	86
.Ltmp77:
	.long	.Ltmp15
	.long	.Ltmp16
.Lset21 = .Ltmp79-.Ltmp78
	.short	.Lset21
.Ltmp78:
	.byte	86
.Ltmp79:
	.long	.Ltmp16
	.long	.Ltmp17
.Lset22 = .Ltmp81-.Ltmp80
	.short	.Lset22
.Ltmp80:
	.byte	80
.Ltmp81:
	.long	.Ltmp17
	.long	.Ltmp23
.Lset23 = .Ltmp83-.Ltmp82
	.short	.Lset23
.Ltmp82:
	.byte	125
	.byte	8
.Ltmp83:
	.long	.Ltmp23
	.long	.Ltmp24
.Lset24 = .Ltmp85-.Ltmp84
	.short	.Lset24
.Ltmp84:
	.byte	86
.Ltmp85:
	.long	.Ltmp24
	.long	.Ltmp26
.Lset25 = .Ltmp87-.Ltmp86
	.short	.Lset25
.Ltmp86:
	.byte	88
.Ltmp87:
	.long	0
	.long	0
.Ldebug_loc35:
	.long	.Ltmp18
	.long	.Ltmp19
.Lset26 = .Ltmp89-.Ltmp88
	.short	.Lset26
.Ltmp88:
	.byte	87
.Ltmp89:
	.long	0
	.long	0
.Ldebug_loc37:
	.long	.Ltmp20
	.long	.Ltmp21
.Lset27 = .Ltmp91-.Ltmp90
	.short	.Lset27
.Ltmp90:
	.byte	87
.Ltmp91:
	.long	0
	.long	0
.Ldebug_loc39:
	.long	.Ltmp32
	.long	.Ltmp33
.Lset28 = .Ltmp93-.Ltmp92
	.short	.Lset28
.Ltmp92:
	.byte	86
.Ltmp93:
	.long	0
	.long	0
.Ldebug_loc41:
	.section	.debug_aranges,"",%progbits
	.section	.debug_ranges,"",%progbits
	.section	.debug_macinfo,"",%progbits
	.section	.debug_str,"MS",%progbits,1
.Linfo_string0:
	.asciz	 "clang version 3.4 (https://github.com/llvm-mirror/clang.git 6df7a2db6d74d41c4150b2c46b9a9e4e2db6c1dc) (https://github.com/llvm-mirror/llvm.git fa487e83a83c260d6a50f3df00a0eb012553a912)"
.Linfo_string1:
	.asciz	 "kernel/main.c"
.Linfo_string2:
	.asciz	 "/home/andy/UNI/DiplArbeit/andix/src/trusted/kernelspace/kernel"
.Linfo_string3:
	.asciz	 "kputchar"
.Linfo_string4:
	.asciz	 "char"
.Linfo_string5:
	.asciz	 "dummy_irq_handler"
.Linfo_string6:
	.asciz	 "entry"
.Linfo_string7:
	.asciz	 "SECURE"
.Linfo_string8:
	.asciz	 "NONSECURE"
.Linfo_string9:
	.asciz	 "irq"
.Linfo_string10:
	.asciz	 "int"
.Linfo_string11:
	.asciz	 "atagparam"
.Linfo_string12:
	.asciz	 "unsigned int"
.Linfo_string13:
	.asciz	 "uint32_t"
.Linfo_string14:
	.asciz	 "systemID"
.Linfo_string15:
	.asciz	 "sysid"
.Linfo_string16:
	.asciz	 "atag_base"
.Linfo_string17:
	.asciz	 "vectors"
.Linfo_string18:
	.asciz	 "cp15"
.Linfo_string19:
	.asciz	 "atags"
.Linfo_string20:
	.asciz	 "atag"
.Linfo_string21:
	.asciz	 "_svc_vect"
.Linfo_string22:
	.asciz	 "_mon_vect"
.Linfo_string23:
	.asciz	 "main_task"
.Linfo_string24:
	.asciz	 "task_t"
.Linfo_string25:
	.asciz	 "csu"
.Linfo_string26:
	.asciz	 "csu_base"
.Linfo_string27:
	.asciz	 "uintptr_t"
.Linfo_string28:
	.asciz	 "v_load_addr"
.Linfo_string29:
	.asciz	 "clk_request"
.Linfo_string30:
	.asciz	 "value"
.Linfo_string31:
	.asciz	 "clk_request_t"

