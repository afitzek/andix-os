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
 * imx53_loco.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <platform/board.h>
#include <platform/board/imx53_loco.h>
#include <hal.h>

static const device_info_t imx53_loco_dev_map[] = {
		[0] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x53FBC000,
				.size 	= 0x4000,
				.driver = IMX53_UART_DRIVER_NAME,
				.id		= 0,
				.flags	= 0
		},
		[1] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x53FC0000,
				.size 	= 0x4000,
				.driver = IMX53_UART_DRIVER_NAME,
				.id		= 1,
				.flags	= FLAG_DO_INIT
		},
		[2] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x5000C000,
				.size 	= 0x4000,
				.driver = IMX53_UART_DRIVER_NAME,
				.id		= 2,
				.flags	= FLAG_DO_INIT
		},
		[3] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x53FF0000,
				.size 	= 0x4000,
				.driver = IMX53_UART_DRIVER_NAME,
				.id		= 3,
				.flags	= FLAG_DO_INIT
		},
		[4] = {
				.name 	= SERIAL_DEVICE,
				.base 	= (uintptr_t)0x63F90000,
				.size 	= 0x4000,
				.driver = IMX53_UART_DRIVER_NAME,
				.id		= 4,
				.flags	= FLAG_DO_INIT
		},
		[5] = {
				.name 	= TIMER_DEVICE,
				.base 	= (uintptr_t)0x53FA0000,
				.size 	= 0x4000,
				.driver = IMX53_GPT_DRIVER_NAME,
				.id		= 0,
				.flags	= 0
		},
		[6] = {
				.name 	= RANDOM_DEVICE,
				.base 	= (uintptr_t)0x00000000,
				.size 	= 0x0000,
				.driver = SRNG_DRIVER_NAME,
				.id		= 0,
				.flags	= 0
		},
		[7] = {
				.name 	= CSU_DEVICE,
				.base 	= (uintptr_t)0x63F9C000,
				.size 	= 0x4000,
				.driver = IMX53_CSU_DRIVER_NAME,
				.id		= 0,
				.flags	= FLAG_DO_INIT
		},
		[8] = {
				.name 	= INTERRUPT_CTRL_DEVICE,
				.base 	= (uintptr_t)0x0FFFC000,
				.size 	= 0x4000,
				.driver = IMX53_TZIC_DRIVER_NAME,
				.id		= 0,
				.flags	= FLAG_DO_INIT
		},
		[9] = {
				.name 	= WATCHDOG_DEVICE,
				.base 	= (uintptr_t)0x53F98000,
				.size 	= 0x4000,
				.driver = IMX53_WDOG_DRIVER_NAME,
				.id		= 0,
				.flags	= FLAG_DO_INIT
		},
		[10] = {
				.name 	= TZMEM_CTRL_DEVICE,
				.base 	= (uintptr_t)0x63FD8000,
				.size 	= 0x4000,
				.driver = IMX53_M4IF_DRIVER_NAME,
				.id		= 0,
				.flags	= FLAG_DO_INIT
		}
};

static const platform_device_t imx53_loco_platform_device_map[] = {
		[0] = {
				.info = &(imx53_loco_dev_map[0]),
				.id = 0,
				.vbase = NULL,
				.size = 0,
				.driver = &imx_uart_driver
		}
};

static const hal_platform_t imx53_loco = {
		.sys_id					= 0xcc9,
		.device_count 			= sizeof(imx53_loco_dev_map)/
			 sizeof(device_info_t),
		.dev_map 				= imx53_loco_dev_map,
		.platform_device_count 	= 1,
		.platform_device_map	= imx53_loco_platform_device_map,
		.__early_io_init		= &imx53_early_init,
		.__early_io_putc		= &imx53_early_putchar,
		.__early_io_getc		= &imx53_early_getchar
};

HAL_PLATFORM_REGISTER(imx53_loco)


