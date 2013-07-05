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
 * imx_csu.c
 *
 *  Created on: Jun 2, 2013
 *      Author: andy
 */

#include <drivers/csu/imx_csu.h>

int imx_csu_probe(platform_device_t *dev) {
	hal_map_io_mem(dev);

	dev->device_data = dev->vbase;

	uintptr_t csu = dev->device_data;

	// We need to unlock everything. Do more reverse engineering
	// to find precise register counts

	while (csu < (uint32_t*) ((uint32_t) dev->vbase + 0x80)) {
		(*csu) = 0x00FF00FF;
		csu++;
	}

	return HAL_SUCCESS;
}

uint32_t imx_csu_ioctl(platform_device_t *dev,
		uint32_t request, uintptr_t param, uint32_t psize) {
	// we don't know which device is which so we support nothing...
	return HAL_E_IOCTL_REQ_NOT_AVAIL;
}

static const platform_driver_t imx_csu_driver = { .probe = imx_csu_probe,
		.release = NULL, .write = NULL,
		.read = NULL, .ioctl = NULL, .driver = { .name =
				IMX53_CSU_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_csu_driver)
