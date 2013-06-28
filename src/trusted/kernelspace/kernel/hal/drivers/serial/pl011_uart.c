/*
 * pl011_uart.c
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#include <drivers/serial/pl011_uart.h>

static uint32_t ufr;
static uint32_t udr;

int pl011_uart_probe(platform_device_t *dev) {
	hal_map_io_mem(dev);

	if (dev->id == 0) {
		ufr = (uint32_t) ((uint8_t*) dev->vbase + PL011_UARTFR);
		udr = (uint32_t) ((uint8_t*) dev->vbase + PL011_UARTDR);
		kputchar = pl011_uart_putchar;
		hal_debug("MAPPED!!!");
	}
	return HAL_SUCCESS;
}

void pl011_uart_release(platform_device_t *dev) {

}

void pl011_uart_putchar(char c) {
	while ((__raw_readw((void*) ufr) & PL011_TX_FULL)!= 0){
		NOP;
	}
	__raw_writew(c, (void*) udr);
}

char pl011_uart_getchar() {
	while ((__raw_readw((void*) ufr) & PL011_RX_EMPTY)!= 0){
		NOP;
	}

	uint8_t c = (__raw_readw((void*) udr) & 0xFF);
	return ((char) c);
}

uint32_t pl011_uart_write(platform_device_t *dev, void* data, uint32_t size) {
	uint32_t idx = 0;
	char* buffer = (char*) data;
	for (idx = 0; idx < size; idx++) {
		pl011_uart_putchar(buffer[idx]);
	}
	return HAL_SUCCESS;
}

uint32_t pl011_uart_read(platform_device_t *dev, void* data, uint32_t nsize) {
	uint32_t idx = 0;
	char* buffer = (char*) data;
	for (idx = 0; idx < nsize; idx++) {
		buffer[idx] = pl011_uart_getchar();
	}
	return HAL_SUCCESS;
}
uint32_t pl011_uart_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	return (HAL_E_IOCTL_REQ_NOT_AVAIL);
}

