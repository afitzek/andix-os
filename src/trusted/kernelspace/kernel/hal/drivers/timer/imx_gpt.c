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
 * imx_gpt.c
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <drivers/timer/imx_gpt.h>

int imx_gpt_probe(platform_device_t *dev) {
	int32_t i = 0;
	hal_map_io_mem(dev);

	//dump_mmu_full();

	imx_gpt_t* gpt = (imx_gpt_t*) dev->vbase;
	dev->device_data = (uintptr_t) gpt;

	/* setup GP Timer 1 */
	__raw_writel(GPTCR_SWR, &gpt->control);

	__raw_writel(0x3F, &gpt->interrupt);

	/* We have no udelay by now */
	while(__raw_readl(&gpt->control) & GPTCR_SWR);

	__raw_writel(0, &gpt->prescaler); /* 32Khz */

	/* Freerun Mode, PERCLK1 input */
	i = __raw_readl(&gpt->control);
	__raw_writel(i | GPTCR_FRR | GPTCR_CLKSOURCE_32 | GPTCR_TEN, &gpt->control);

	return HAL_SUCCESS;
}

void imx_gpt_release(platform_device_t *dev) {

}

uint32_t imx_gpt_write(platform_device_t *dev, void* data, uint32_t size) {
	return HAL_E_FUNC_NOT_AVAIL;
}

uint32_t imx_gpt_read(platform_device_t *dev, void* data, uint32_t nsize) {
	return HAL_E_FUNC_NOT_AVAIL;
}

uint32_t imx_gpt_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {

	clk_request_t* requ = (clk_request_t*) param;

	if (requ == NULL || psize != sizeof(clk_request_t)) {
		return HAL_E_IOCTL_PARA_INVALID;
	}

	imx_gpt_t* gpt = (imx_gpt_t*) dev->device_data;
	if (gpt == NULL) {
		gpt = (imx_gpt_t*) dev->vbase;
	}

	if (gpt == NULL) {
		hal_debug("GPT Device has no virtual address!!");
		return HAL_E_INVALID_DEV;
	}

	switch (request) {
		case IOCTL_CLOCK_GET_CTR:
			requ->value = __raw_readl(&gpt->counter);
			break;
		case IOCTL_CLOCK_GET_FREQ_KHZ:
			requ->value = CLK_32KHZ;
			break;
		case IOCTL_CLOCK_ALARM_AT:
			__raw_writel(requ->value, &gpt->out_cmp_r1);
			break;
		default:
			return HAL_E_IOCTL_REQ_NOT_AVAIL;
	}
	return HAL_SUCCESS;
}

static const platform_driver_t imx_gpt_driver = { .probe = imx_gpt_probe,
		.release = imx_gpt_release, .write = imx_gpt_write,
		.read = imx_gpt_read, .ioctl = imx_gpt_ioctl, .driver = { .name =
				IMX53_GPT_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_gpt_driver)
