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
