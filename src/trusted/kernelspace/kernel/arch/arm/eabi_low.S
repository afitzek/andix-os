
	.global	__aeabi_memset
	.global	__aeabi_memset4
	.global	__aeabi_memset8
	.global	memset
	.text
    .arm
	.type	__aeabi_memset, %function
__aeabi_memset:
__aeabi_memset4:
__aeabi_memset8:
memset:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	str	r2, [fp, #-24]
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-8]
	b	.L2
.L3:
	ldr	r3, [fp, #-24]
	and	r2, r3, #255
	ldr	r3, [fp, #-8]
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L2:
	ldr	r3, [fp, #-20]
	cmp	r3, #0
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
	ldr	r2, [fp, #-20]
	sub	r2, r2, #1
	str	r2, [fp, #-20]
	cmp	r3, #0
	bne	.L3
	ldr	r3, [fp, #-16]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	__aeabi_memset, .-__aeabi_memset


	.align	2
	.global	__aeabi_memcpy
	.global	__aeabi_memcpy4
	.global	__aeabi_memcpy8
	.global	memcpy
	.type	__aeabi_memcpy, %function
__aeabi_memcpy:
__aeabi_memcpy4:
__aeabi_memcpy8:
memcpy:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	str	r2, [fp, #-24]
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-20]
	str	r3, [fp, #-8]
	b	.L6
.L7:
	ldr	r3, [fp, #-8]
	ldrb	r2, [r3, #0]	@ zero_extendqisi2
	ldr	r3, [fp, #-12]
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L6:
	ldr	r3, [fp, #-24]
	cmp	r3, #0
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
	ldr	r2, [fp, #-24]
	sub	r2, r2, #1
	str	r2, [fp, #-24]
	cmp	r3, #0
	bne	.L7
	ldr	r3, [fp, #-16]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	__aeabi_memcpy, .-__aeabi_memcpy


	.align	2
	.global	__aeabi_memmove
	.global	__aeabi_memmove4
	.global	__aeabi_memmove8
	.global memmove
	.type	__aeabi_memmove, %function
__aeabi_memmove:
__aeabi_memmove4:
__aeabi_memmove8:
memmove:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	str	r2, [fp, #-24]
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-20]
	str	r3, [fp, #-8]
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-12]
	cmp	r2, r3
	bcs	.L17
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-24]
	add	r2, r2, r3
	ldr	r3, [fp, #-12]
	cmp	r2, r3
	bls	.L17
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	str	r3, [fp, #-8]
	ldr	r2, [fp, #-12]
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	str	r3, [fp, #-12]
	b	.L11
.L12:
	ldr	r3, [fp, #-12]
	sub	r3, r3, #1
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-8]
	sub	r3, r3, #1
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldrb	r2, [r3, #0]	@ zero_extendqisi2
	ldr	r3, [fp, #-12]
	strb	r2, [r3, #0]
.L11:
	ldr	r3, [fp, #-24]
	cmp	r3, #0
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
	ldr	r2, [fp, #-24]
	sub	r2, r2, #1
	str	r2, [fp, #-24]
	cmp	r3, #0
	bne	.L12
	b	.L13
.L15:
	ldr	r3, [fp, #-8]
	ldrb	r2, [r3, #0]	@ zero_extendqisi2
	ldr	r3, [fp, #-12]
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	b	.L14
.L17:
	mov	r0, r0	@ nop
.L14:
	ldr	r3, [fp, #-24]
	cmp	r3, #0
	moveq	r3, #0
	movne	r3, #1
	and	r3, r3, #255
	ldr	r2, [fp, #-24]
	sub	r2, r2, #1
	str	r2, [fp, #-24]
	cmp	r3, #0
	bne	.L15
.L13:
	ldr	r3, [fp, #-16]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	__aeabi_memmove, .-__aeabi_memmove
