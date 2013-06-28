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
