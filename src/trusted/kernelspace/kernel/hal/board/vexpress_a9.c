/*
 * vexpress_a9.c
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */

#include <platform/board.h>
#include <platform/board/vexpress_a9.h>
#include <hal.h>

static const device_info_t vexpress_a9_dev_map[] = {
		[0] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x10009000,
				.size 	= 0x1000,
				.driver = PL011_UART_DRIVER_NAME,
				.id		= 0,
				.flags	= 0
		},
		[1] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x1000a000,
				.size 	= 0x1000,
				.driver = PL011_UART_DRIVER_NAME,
				.id		= 1,
				.flags	= FLAG_DO_INIT
		},
		[2] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x1000b000,
				.size 	= 0x1000,
				.driver = PL011_UART_DRIVER_NAME,
				.id		= 2,
				.flags	= FLAG_DO_INIT
		},
		[3] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x1000c000,
				.size 	= 0x1000,
				.driver = PL011_UART_DRIVER_NAME,
				.id		= 3,
				.flags	= FLAG_DO_INIT
		},
		[4] = {
				.name 	= RANDOM_DEVICE,
				.base 	= (uintptr_t)0x00000000,
				.size 	= 0x0000,
				.driver = SRNG_DRIVER_NAME,
				.id		= 0,
				.flags	= 0
		}
};

static const platform_device_t vexpress_a9_platform_device_map[] = {
		[0] = {
				.info = &(vexpress_a9_dev_map[0]),
				.id = 0,
				.vbase = NULL,
				.size = 0,
				.driver = &pl011_uart_driver
		}
};

static const hal_platform_t vexpress_a9 = {
		.sys_id					= 0x8e0,
		.device_count 			= sizeof(vexpress_a9_dev_map)/
			 sizeof(device_info_t),
		.dev_map 				= vexpress_a9_dev_map,
		.platform_device_count 	= 1,
		.platform_device_map	= vexpress_a9_platform_device_map,
		.__early_io_init		= &ve_a9_early_init,
		.__early_io_putc		= &ve_a9_early_putchar,
		.__early_io_getc		= &ve_a9_early_getchar
};

HAL_PLATFORM_REGISTER(vexpress_a9)

