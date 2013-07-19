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

forceinline static void gotoNSecure() {
	uint32_t fp;
	__asm__ __volatile__(
			"MRC p15, 0, %0, c1, c1, 0\n"
			"ORR %0, %0, #0x1\n"
			"MCR p15, 0, %0, c1, c1, 0\n":"=r" (fp)::"memory");

}

forceinline static void gotoSecure() {
	uint32_t fp;
	__asm__ __volatile__(
				"MRC p15, 0, %0, c1, c1, 0\n"
				"BIC %0, %0, #0x1\n"
				"MCR p15, 0, %0, c1, c1, 0\n":"=r" (fp):"r" (fp):"memory");
}

forceinline static void restoreABT(uint32_t lr, uint32_t cpsr) {
	__asm__ __volatile__(
			"CPS #0x17\n" // GOTO ABT
			"MOV lr, %0\n"
			:"=r" (lr): "r" (lr):"memory");
	__asm__ __volatile__(
			"MSR SPSR, %0\n"
			"CPS #0x16\n"
			:"=r" (cpsr): "r" (cpsr):"memory");
}

#endif /* VECTORS_H_ */
