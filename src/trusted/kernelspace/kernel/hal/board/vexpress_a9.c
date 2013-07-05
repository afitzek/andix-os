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

