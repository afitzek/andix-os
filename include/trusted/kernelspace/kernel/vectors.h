/*
 * vectors.h
 *
 *  Created on: Dec 1, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef VECTORS_H_
#define VECTORS_H_

#include <platform/platform.h>
#include <common/typedefs.h>
#include <kprintf.h>


#define STACK_SIZE	0x4000

void setup_mode_stacks();

static void inline setFIQSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x11 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

static void inline setIRQSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x12 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}
static void inline setSVCSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x13 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}
static uint32_t inline getSVCSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x13 \n"
			"MOV %0, sp \n"
			"CPS #0x13":
			"=r" (sp)::"memory", "cc");
	return (sp);
}
static void inline setABTSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x17 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

static uint32_t inline getABTSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x17 \n"
			"MOV %0, sp \n"
			"CPS #0x13": "=r" (sp)::"memory", "cc");
	return (sp);
}

static void inline setUNDSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x1B \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

static void inline setSYSSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x1F \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

static uint32_t inline getSYSSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x1F \n"
			"MOV %0, sp \n"
			"CPS #0x13": "=r" (sp)::"memory", "cc");
	return (sp);
}

static void inline setMONSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x16 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

static void inline gotoSVCMode() {
	asm volatile("CPSID aif, #0x13");
}

static void inline gotoUSRMode() {
	asm volatile("CPSID aif, #0x10");
}

static void inline gotoABTMode() {
	asm volatile("CPSID aif, #0x17");
}

static void inline gotoMONMode() {
	asm volatile("CPSID aif, #0x16");
}

static uint32_t inline getSVCFPIntoAbt() {
	uint32_t fp;
	__asm__ __volatile__(
			"CPSID aif, #0x13 \n"
			"MOV %0, fp\n"
			"CPS #0x17":"=r" (fp)::"memory");
	return (fp);
}

#endif /* VECTORS_H_ */
