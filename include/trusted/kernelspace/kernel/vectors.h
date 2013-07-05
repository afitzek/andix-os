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

forceinline static void setFIQSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x11 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static void setIRQSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x12 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static void setSVCSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x13 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static uint32_t getSVCSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x13 \n"
			"MOV %0, sp \n"
			"CPS #0x13":
			"=r" (sp)::"memory", "cc");
	return (sp);
}

forceinline static void setABTSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x17 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static uint32_t getABTSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x17 \n"
			"MOV %0, sp \n"
			"CPS #0x13": "=r" (sp)::"memory", "cc");
	return (sp);
}

forceinline static void setUNDSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x1B \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static void setSYSSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x1F \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static uint32_t getSYSSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x1F \n"
			"MOV %0, sp \n"
			"CPS #0x13": "=r" (sp)::"memory", "cc");
	return (sp);
}

forceinline static void setMONSP(uintptr_t sp) {
	asm volatile(
			"CPSID aif, #0x16 \n"
			"MOV sp, %0 \n"
			"CPS #0x13":: "r" (sp):"memory", "cc");
}

forceinline static uint32_t getMONSP() {
	uint32_t sp;
	asm volatile(
			"CPSID aif, #0x16 \n"
			"MOV %0, sp \n"
			"CPS #0x13":
			"=r" (sp)::"memory", "cc");
	return (sp);
}

forceinline static void gotoSVCMode() {
	asm volatile("CPSID aif, #0x13");
}

forceinline static void gotoUSRMode() {
	asm volatile("CPSID aif, #0x10");
}

forceinline static void gotoABTMode() {
	asm volatile("CPSID aif, #0x17");
}

forceinline static void gotoMONMode() {
	asm volatile("CPSID aif, #0x16");
}

forceinline static uint32_t getSVCFPIntoAbt() {
	uint32_t fp;
	__asm__ __volatile__(
			"CPSID aif, #0x13 \n"
			"MOV %0, fp\n"
			"CPS #0x17":"=r" (fp)::"memory");
	return (fp);
}

forceinline static uint32_t getSYSFPIntoAbt() {
	uint32_t fp;
	__asm__ __volatile__(
			"CPSID aif, #0x1F \n"
			"MOV %0, fp\n"
			"CPS #0x17":"=r" (fp)::"memory");
	return (fp);
}

forceinline static uint32_t getMONFPIntoAbt() {
	uint32_t fp;
	__asm__ __volatile__(
			"CPSID aif, #0x16 \n"
			"MOV %0, fp\n"
			"CPS #0x17":"=r" (fp)::"memory");
	return (fp);
}

#endif /* VECTORS_H_ */
