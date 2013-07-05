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
 * imx53_early_io.c
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#include <platform/board/imx53/imx53_early_io.h>
#include <drivers/serial/imx_uart.h>

uint32_t imx_uart_base = UART_BASE;

void imx53_early_init() {
	imx_uart_base = UART_BASE;

	//__REG(0x53FA8878) = 0;
	//__REG(0x53FA80E8) = 1;
	//__REG(0x53FA80EC) = 1;
	//53FA_80E8 = 1
	//53FA_80EC = 1
	/*
	 __REG(UART_BASE + UCR1) = 0x0;
	 __REG(UART_BASE + UCR2) = 0x0;

	 while (!(__REG(UART_BASE + UCR2) & UCR2_SRST))
	 ;

	 __REG(UART_BASE + UCR3) = 0x0704;
	 __REG(UART_BASE + UCR4) = 0x8000;
	 __REG(UART_BASE + UESC) = 0x002b;
	 __REG(UART_BASE + UTIM) = 0x0;

	 __REG(UART_BASE + UTS) = 0x0;

	 __REG(UART_BASE + UFCR) = 4 << 7;
	 __REG(UART_BASE + UBIR) = 0xf;
	 __REG(UART_BASE + UBMR) = 0xf0;

	 __REG(UART_BASE + UCR2) = UCR2_WS | UCR2_IRTS | UCR2_RXEN | UCR2_TXEN
	 | UCR2_SRST;
	 __REG(UART_BASE + UCR1) = UCR1_UARTEN;
	 */
}

void imx53_early_putchar(uint8_t c) {
	__REG(imx_uart_base + UTXD) = c;

	/* wait for transmitter to be ready */
	while (!(__REG(imx_uart_base + UTS) & UTS_TXEMPTY))
		;
}

uint8_t imx53_early_getchar() {
	return (-1);
}
