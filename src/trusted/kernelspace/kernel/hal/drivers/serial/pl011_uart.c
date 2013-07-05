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

static const platform_driver_t pl011_uart_driver = { .probe = pl011_uart_probe,
		.release = pl011_uart_release, .write = pl011_uart_write, .read =
				pl011_uart_read, .ioctl = pl011_uart_ioctl, .driver = { .name =
				PL011_UART_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(pl011_uart_driver)

