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

