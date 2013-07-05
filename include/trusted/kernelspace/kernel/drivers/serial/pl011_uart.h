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
 * pl011_uart.h
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#ifndef PL011_UART_H_
#define PL011_UART_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

#define PL011_UART_DRIVER_NAME "PL011_UART"

#define PL011_UARTDR		(0x000)
#define PL011_UARTFR		(0x018)

#define PL011_TX_FULL		(0x20)
#define PL011_RX_EMPTY		(0x10)

typedef struct {
	uintptr_t	uart_base;
} pl011_uart_device_data;

static const platform_driver_t pl011_uart_driver;

void pl011_uart_putchar(char c);
int pl011_uart_probe(platform_device_t *dev);
void pl011_uart_release(platform_device_t *dev);
uint32_t pl011_uart_write(platform_device_t *dev, void* data, uint32_t size);
uint32_t pl011_uart_read(platform_device_t *dev, void* data, uint32_t nsize);
uint32_t pl011_uart_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize);

#endif /* PL011_UART_H_ */
