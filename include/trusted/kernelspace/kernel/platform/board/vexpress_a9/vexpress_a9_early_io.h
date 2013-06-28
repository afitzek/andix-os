/*
 * vexpress_a9_early_io.h
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#ifndef VEXPRESS_A9_EARLY_IO_H_
#define VEXPRESS_A9_EARLY_IO_H_

#include <drivers/serial/pl011_uart.h>

#define VE_A9_UART_0_BASE	(0x10009000)

void ve_a9_early_init();
void ve_a9_early_putchar(uint8_t c);
uint8_t ve_a9_early_getchar();

#endif /* VEXPRESS_A9_EARLY_IO_H_ */
