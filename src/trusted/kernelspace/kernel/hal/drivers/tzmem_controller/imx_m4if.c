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
 * imx_m4if.c
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#include <devices/tzmem_controller/tzmem_controller.h>
#include <drivers/tzmem_controller/imx_m4if.h>
#include <hal.h>
#include <common.h>
#include <mm/mm.h>

/**
 * m4if probe handler
 */
int imx_m4if_probe(platform_device_t *dev) {
	int32_t i = 0;
	hal_map_io_mem(dev);

	if (dev->vbase == NULL ) {
		return (HAL_E_OUT_OF_MEMORY);
	}

	imx_m4if_device_data_t* dev_data = (imx_m4if_device_data_t*) kmalloc(
			sizeof(imx_m4if_device_data_t));

	if (dev_data == NULL ) {
		return (HAL_E_OUT_OF_MEMORY);
	}

	dev_data->cs0 = NULL;
	dev_data->cs1 = NULL;
	dev_data->regs = (imx_m4if_regs_t*) dev->vbase;

	dev->device_data = (uintptr_t) dev_data;

	return (HAL_SUCCESS);
}

int32_t imx_m4if_get_cs(void* physical_start, uint32_t size) {
	void* physical_end = physical_start + size;

	hal_info("CS for %x %x", physical_start, physical_end);

	if (physical_start >= IMX53_M4IF_CS1_PADDR_BORDER
			&& physical_end < IMX53_M4IF_CS1_PADDR_END) {
		return (1);
	} else if (physical_start >= IMX53_M4IF_CS1_PADDR_START
			&& physical_end < IMX53_M4IF_CS1_PADDR_BORDER) {
		return (1);
	} else if (physical_start >= IMX53_M4IF_CS0_PADDR_BORDER
			&& physical_end < IMX53_M4IF_CS0_PADDR_END) {
		return (0);
	} else if (physical_start >= IMX53_M4IF_CS0_PADDR_START
			&& physical_end < IMX53_M4IF_CS0_PADDR_BORDER) {
		return (0);
	}

	return (-1);
}

/**
 * IOCTL handler for imx53 m4if
 */
uint32_t imx_m4if_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	imx_m4if_device_data_t* dev_data = NULL;
	tz_mem_ioparam_t* ioparam = NULL;

	if (dev == NULL ) {
		hal_error("device dev is NULL");
		return (HAL_E_INVALID_DEV);
	}

	dev_data = (imx_m4if_device_data_t*) dev->device_data;

	if (dev_data == NULL ) {
		hal_error("device dev_data is NULL");
		return (HAL_E_INVALID_DEV);
	}

	ioparam = (tz_mem_ioparam_t*) param;

	if (ioparam == NULL ) {
		return (HAL_E_IOCTL_PARA_INVALID);
	}

	switch (request) {
	case TZ_MEM_IOCTL_LOCK_MEMORY:
		return (imx_m4if_protect_mem(dev_data, ioparam->physical_addr,
				ioparam->size));
	case TZ_MEM_IOCTL_UNLOCK_MEMORY:
		return (imx_m4if_unprotect_mem(dev_data, ioparam->physical_addr,
				ioparam->size));
	case TZ_MEM_IOCTL_DUMP:
		imx_m4if_dump(dev_data);
		break;
	default:
		return (HAL_E_IOCTL_REQ_NOT_AVAIL);
	}

	return HAL_SUCCESS;
}

/**
 * Protect memory
 *
 * @param physical_start physical start addr
 * @param size the memory size
 * @return HAL_*
 */
uint32_t imx_m4if_protect_mem(imx_m4if_device_data_t* m4if,
		uintptr_t physical_start, uint32_t size) {
	int32_t cs = imx_m4if_get_cs((void*) physical_start, size);
	void* physical_end = (void*)physical_start + size;
	uint32_t start_ = (unsigned long) physical_start >> (12);
	uint32_t end_ = (unsigned long) physical_end >> (12);

	hal_info("Got CS %d", cs);

	if (cs == 0) {
		if (m4if->cs0 != NULL ) {
			return (HAL_E_OUT_OF_RESOURCES);
		}
		m4if->cs0 = (imx_m4if_protected_area_t*) kmalloc(
				sizeof(imx_m4if_protected_area_t));
		if (m4if->cs0 == NULL ) {
			return (HAL_E_OUT_OF_MEMORY);
		}

		__raw_writel(0x0, &m4if->regs->wm0_status);
		__raw_writel(0x0, &m4if->regs->wm0_start_cs0);
		__raw_writel(end_, &m4if->regs->wm0_end_cs0);
		__raw_writel(0x80000000 | start_, &m4if->regs->wm0_start_cs0);
		__raw_writel(0x80000000, &m4if->regs->wm0_status);

		m4if->cs0->physical_start = physical_start;
		m4if->cs0->physical_end = physical_end;

		return (HAL_SUCCESS);
	} else if (cs == 1) {
		if (m4if->cs1 != NULL ) {
			return (HAL_E_OUT_OF_RESOURCES);
		}
		m4if->cs1 = (imx_m4if_protected_area_t*) kmalloc(
				sizeof(imx_m4if_protected_area_t));
		if (m4if->cs1 == NULL ) {
			return (HAL_E_OUT_OF_MEMORY);
		}

		__raw_writel(0x0, &m4if->regs->wm0_status);
		__raw_writel(0x0, &m4if->regs->wm0_start_cs1);
		__raw_writel(end_, &m4if->regs->wm0_end_cs1);
		__raw_writel(0x80000000 | start_, &m4if->regs->wm0_start_cs1);
		__raw_writel(0x80000000, &m4if->regs->wm0_status);

		m4if->cs1->physical_start = physical_start;
		m4if->cs1->physical_end = physical_end;

		return (HAL_SUCCESS);
	}

	return (HAL_E_IOCTL_PARA_INVALID);
}

/**
 * Unprotect memory
 *
 * @param physical_start physical start addr
 * @param size the memory size
 * @return HAL_*
 */
uint32_t imx_m4if_unprotect_mem(imx_m4if_device_data_t* m4if,
		uintptr_t physical_start, uint32_t size) {
	void* physical_end = (void*)physical_start + size;

	if (m4if->cs0 != NULL ) {
		if (m4if->cs0->physical_start == physical_start
				&& m4if->cs0->physical_end == physical_end) {
			__raw_writel(0x0, &m4if->regs->wm0_status);
			__raw_writel(0x0, &m4if->regs->wm0_start_cs0);
			__raw_writel(0x80000000, &m4if->regs->wm0_status);
			return (HAL_SUCCESS);
		}
		return (HAL_E_IOCTL_PARA_INVALID);
	}

	if (m4if->cs1 != NULL ) {
		if (m4if->cs1->physical_start == physical_start
				&& m4if->cs1->physical_end == physical_end) {
			__raw_writel(0x0, &m4if->regs->wm0_status);
			__raw_writel(0x0, &m4if->regs->wm0_start_cs1);
			__raw_writel(0x80000000, &m4if->regs->wm0_status);
			return (HAL_SUCCESS);
		}
		return (HAL_E_IOCTL_PARA_INVALID);
	}

	return (HAL_E_IOCTL_PARA_INVALID);
}

void imx_m4if_dump(imx_m4if_device_data_t* m4if) {
	hal_info("M4IF DUMP:");
	hal_info("WM_START_CS0: %x", __raw_readl(&m4if->regs->wm0_start_cs0));
	hal_info("WM_END_CS0  : %x", __raw_readl(&m4if->regs->wm0_end_cs0));
	hal_info("WM_START_CS1: %x", __raw_readl(&m4if->regs->wm0_start_cs1));
	hal_info("WM_END_CS1  : %x", __raw_readl(&m4if->regs->wm0_end_cs1));
	hal_info("WM_STATUS   : %x", __raw_readl(&m4if->regs->wm0_status));
}

static const platform_driver_t imx_m4if_driver = { .probe = imx_m4if_probe,
		.release = NULL, .write = NULL, .read = NULL, .ioctl = imx_m4if_ioctl,
		.driver = { .name = IMX53_M4IF_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_m4if_driver)
