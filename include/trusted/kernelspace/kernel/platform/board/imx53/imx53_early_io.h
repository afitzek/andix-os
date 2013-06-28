/*
 * imx53_early_io.h
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#ifndef IMX53_EARLY_IO_H_
#define IMX53_EARLY_IO_H_

#include <common.h>

#define UART_1_APITZ_OFFSET 0xBC000
#define PHYSICAL_AIPSTZ_1_START 0x53F00000
#define UART_BASE PHYSICAL_AIPSTZ_1_START + UART_1_APITZ_OFFSET

void imx53_early_init();
void imx53_early_putchar(uint8_t c);
uint8_t imx53_early_getchar();

#endif /* IMX53_EARLY_IO_H_ */
