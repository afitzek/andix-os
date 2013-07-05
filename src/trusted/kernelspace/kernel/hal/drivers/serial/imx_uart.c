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
 * imx_uart.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <drivers/serial/imx_uart.h>

static uint32_t utx;
static uint32_t urx;
static uint32_t uts;

int imx_uart_probe(platform_device_t *dev) {
	hal_map_io_mem(dev);

	if (dev->id == 0) {
		utx = (uint32_t) ((uint8_t*) dev->vbase + UTXD);
		urx = (uint32_t) ((uint8_t*) dev->vbase + URXD);
		uts = (uint32_t) ((uint8_t*) dev->vbase + UTS);
		kputchar = imx_uart_putchar;
		hal_debug("MAPPED!!!");
	}

	if (CHECK_BIT(dev->info->flags, FLAG_DO_INIT_POS)) {
		//init_imx_uart(dev);
	}

	return HAL_SUCCESS;
}

void imx_uart_release(platform_device_t *dev) {

}

uint32_t imx_uart_write(platform_device_t *dev, void* data, uint32_t size) {
	//uint32_t reg_utx = ((uint8_t*)dev->vbase + UTXD);
	//uint32_t reg_uts = ((uint8_t*)dev->vbase + UTS);
	uint32_t idx = 0;
	char* buffer = (char*) data;
	for (idx = 0; idx < size; idx++) {
		imx_uart_putchar(buffer[idx]);
	}
	return HAL_SUCCESS;
}

char imx_uart_getchar() {
	/* wait for transmitter to be ready */
	while ((__raw_readl((void*) uts) & UTS_RXEMPTY)) {
		NOP;
	}

	//__REG(utx) = c;
	uint8_t c = __raw_readb((void*) urx);

	return ((char) c);
}

uint32_t imx_uart_read(platform_device_t *dev, void* data, uint32_t nsize) {
	uint32_t idx = 0;
	char* buffer = (char*) data;
	for (idx = 0; idx < nsize; idx++) {
		buffer[idx] = imx_uart_getchar();
	}
	return HAL_SUCCESS;
}

uint32_t imx_uart_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	return (HAL_E_IOCTL_REQ_NOT_AVAIL);;
}

void imx_uart_putchar(char c) {
	//__REG(utx) = c;
	__raw_writeb(c, (void*) utx);

	/* wait for transmitter to be ready */
	while (!(__raw_readl((void*) uts) & UTS_TXEMPTY)) {
		NOP;
	}
}

static const platform_driver_t imx_uart_driver = { .probe = imx_uart_probe,
		.release = imx_uart_release, .write = imx_uart_write, .read =
				imx_uart_read, .ioctl = imx_uart_ioctl, .driver = { .name =
				IMX53_UART_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_uart_driver)

void init_imx_uart(platform_device_t *dev) {
	uint32_t uart_base = (uint32_t) dev->vbase;

	__REG(uart_base + UCR1) = 0x0;
	__REG(uart_base + UCR2) = 0x0;

	while (!(__REG(uart_base + UCR2) & UCR2_SRST))
		;

	__REG(uart_base + UCR3) = 0x0704;
	__REG(uart_base + UCR4) = 0x8000;
	__REG(uart_base + UESC) = 0x002b;
	__REG(uart_base + UTIM) = 0x0;

	__REG(uart_base + UTS) = 0x0;

	__REG(uart_base + UFCR) = 4 << 7;
	__REG(uart_base + UBIR) = 0xf;
	__REG(uart_base + UBMR) = 0xf0;

	__REG(uart_base + UCR2) = UCR2_WS | UCR2_IRTS | UCR2_RXEN | UCR2_TXEN
			| UCR2_SRST;
	__REG(uart_base + UCR1) = UCR1_UARTEN;
}
