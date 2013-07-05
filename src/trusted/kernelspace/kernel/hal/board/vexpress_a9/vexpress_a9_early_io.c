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
 * vexpress_a9_early_io.c
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#include <platform/board/vexpress_a9/vexpress_a9_early_io.h>

static uint32_t ve_a9_early_ufr;
static uint32_t ve_a9_early_udr;

void ve_a9_early_init() {
	ve_a9_early_ufr = VE_A9_UART_0_BASE + PL011_UARTFR;
	ve_a9_early_udr = VE_A9_UART_0_BASE + PL011_UARTDR;
}

void ve_a9_early_putchar(uint8_t c) {
	while ((__raw_readl((void*) ve_a9_early_ufr) & PL011_TX_FULL)!= 0){
		NOP;
	}
	__raw_writel(c, (void*) ve_a9_early_udr);
}

uint8_t ve_a9_early_getchar() {
	while ((__raw_readl((void*) ve_a9_early_ufr) & PL011_RX_EMPTY)!= 0){
		NOP;
	}

	uint8_t c = (__raw_readl((void*) ve_a9_early_udr) & 0xFF);
	return ((char) c);
}

