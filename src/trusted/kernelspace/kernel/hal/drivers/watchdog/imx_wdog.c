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
 * imx_wdog.c
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#include <drivers/watchdog/imx_wdog.h>
#include <devices/watchdog/watchdog.h>
#include <platform/vector_debug.h>

void imx_wdog_reset(imx_wdog_t* wdog) {
	__raw_writew(0x5555, &(wdog->wsr));
	__raw_writew(0xAAAA, &(wdog->wsr));
}

void imx_wdog_irq_handler(int irq) {

	platform_device_t* wdog_dev = hal_find_device(WATCHDOG_DEVICE, 0);

	if(wdog_dev != NULL) {
		if(wdog_dev->device_data != NULL) {
			imx_wdog_reset((imx_wdog_t*)wdog_dev->device_data);
		}
	}

	hal_info("WDOG Interrupt!");
}

int imx_wdog_probe(platform_device_t *dev) {
	int32_t i = 0;
	uint16_t val = 0;
	hal_map_io_mem(dev);

	imx_wdog_t* wdog = (imx_wdog_t*) dev->vbase;
	dev->device_data = (uintptr_t) wdog;

	if(wdog->wrsr == 0x10) {
		hal_info("Last reset caused by Power On Reset");
	} else if(wdog->wrsr == 0x1) {
		hal_info("Last reset caused by Software Reset");
	} else if(wdog->wrsr == 0x2) {
		hal_info("Last reset caused by WDOG Timeout");
	} else {
		hal_info("Last reset caused by UNKOWN (%x)", wdog->wrsr);
	}

	return HAL_SUCCESS;
}

void imx_wdog_init(imx_wdog_t* wdog) {
	uint16_t val = 0;
	wdog->wmcr = 0;

	wdog->wicr = 0x8003;

	val = wdog->wcr;
	val = 0x033C;

	wdog->wcr = val;

	val = val | 0x04;

	wdog->wcr = val;

	imx_wdog_reset(wdog);

	irq_register_handler(58, &imx_wdog_irq_handler);
}

uint32_t imx_wdog_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {

	imx_wdog_t* wdog = (imx_wdog_t*) dev->device_data;
	if (wdog == NULL) {
		return (HAL_E_INVALID_DEV);
	}

	switch (request) {
		case WDOG_INIT:
			imx_wdog_init(wdog);
			break;
		case WDOG_PING:
			imx_wdog_reset(wdog);
			break;
		default:
			return HAL_E_IOCTL_REQ_NOT_AVAIL;
	}
	return HAL_SUCCESS;
}

void imx_wdog_release(platform_device_t *dev) {
	irq_free(58);
}

static const platform_driver_t imx_wdog_driver = { .probe = imx_wdog_probe,
		.release = imx_wdog_release, .write = NULL,
		.read = NULL, .ioctl = imx_wdog_ioctl, .driver = { .name =
				IMX53_WDOG_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_wdog_driver)

