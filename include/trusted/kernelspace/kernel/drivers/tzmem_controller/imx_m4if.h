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
 * imx_m4if.h
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#ifndef IMX_M4IF_H_
#define IMX_M4IF_H_

#include <devices/devices.h>
#include <drivers/drivers.h>

/**
 * IMX53 M4IF CS0 start addr
 */
#define IMX53_M4IF_CS0_PADDR_START (0x70000000)

/**
 * IMX53 M4IF CS0 border
 */
#define IMX53_M4IF_CS0_PADDR_BORDER   (0x80000000)

/**
 * IMX53 M4IF CS0 end addr
 */
#define IMX53_M4IF_CS0_PADDR_END   (0x90000000)

/**
 * IMX53 M4IF CS1 start addr
 */
#define IMX53_M4IF_CS1_PADDR_START (0xB0000000)

/**
 * IMX53 M4IF CS1 border
 */
#define IMX53_M4IF_CS1_PADDR_BORDER   (0xC0000000)

/**
 * IMX53 M4IF CS1 end addr
 */
#define IMX53_M4IF_CS1_PADDR_END   (0xD0000000)

/**
 * IMX53 M4IF driver name
 */
#define IMX53_M4IF_DRIVER_NAME "IMX53_M4IF"

/**
 * IMX M4IF register map
 */
struct imx_m4if_regs {
	uint32_t	__space_0[59];
	uint32_t	wm0_start_cs0;      /**< watermark start addr cs0 */
	uint32_t	wm0_start_cs1;      /**< watermark start addr cs1 */
	uint32_t	__space_1[6];
	uint32_t	wm0_end_cs0;   		/**< watermark end addr cs0  */
	uint32_t	wm0_end_cs1;   		/**< watermark end addr cs1  */
	uint32_t	wm0_status;   		/**< watermark status  */
	uint32_t	wm0_violation; 		/**< watermark violation  */
};

/**
 * Typedefine for IMX M4IF register map
 */
typedef struct imx_m4if_regs imx_m4if_regs_t;

/**
 * Protected memory area
 */
struct imx_m4if_protected_area {
	uintptr_t physical_start; /**< physical start addr */
	uintptr_t physical_end; /**< physical end addr */
};

/**
 * Typedefine for Protected memory area
 */
typedef struct imx_m4if_protected_area imx_m4if_protected_area_t;

/**
 * Device data for m4if instances
 */
struct imx_m4if_device_data {
	imx_m4if_regs_t* regs;	/**< io mapped device registers */
	imx_m4if_protected_area_t* cs0; /**< CS0 protected memory */
	imx_m4if_protected_area_t* cs1; /**< CS1 protected memory */
};

/**
 * Typedefine for Device data for m4if instances
 */
typedef struct imx_m4if_device_data imx_m4if_device_data_t;

/**
 * Find out which chipselect to use
 *
 * @param physical_start physical start addr
 * @param size the memory size
 * @return 0 CS0, 1 CS1 else invalid!
 */
int32_t imx_m4if_get_cs(void* physical_start, uint32_t size);

/**
 * m4if probe handler
 */
int imx_m4if_probe(platform_device_t *dev);

/**
 * IOCTL handler for imx53 m4if
 */
uint32_t imx_m4if_ioctl(platform_device_t *dev,
		uint32_t request, uintptr_t param, uint32_t psize);

/**
 * Protect memory
 *
 * @param m4if The m4if device data
 * @param physical_start physical start addr
 * @param size the memory size
 * @return TZ_MEM_*
 */
uint32_t imx_m4if_protect_mem(imx_m4if_device_data_t* m4if,
		uintptr_t physical_start, uint32_t size);

/**
 * Unprotect memory
 *
 * @param m4if The m4if device data
 * @param physical_start physical start addr
 * @param size the memory size
 * @return TZ_MEM_*
 */
uint32_t imx_m4if_unprotect_mem(imx_m4if_device_data_t* m4if,
		uintptr_t physical_start, uint32_t size);

/**
 * Dump m4if information
 *
 * @param m4if The m4if device data
 */
void imx_m4if_dump(imx_m4if_device_data_t* m4if);

/**
 * The m4if driver instance
 */
static const platform_driver_t imx_m4if_driver;


#endif /* IMX_M4IF_H_ */
