/*
 * imx_gpt.h
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef IMX_GPT_H_
#define IMX_GPT_H_

#include <common/typedefs.h>
#include <devices/devices.h>
#include <drivers/drivers.h>
#include <kprintf.h>
#include <common.h>
#include <hal.h>

#define IMX53_GPT_DRIVER_NAME "IMX53_GPT"

struct imx_gpt_regs {
	uint32_t	control;
	uint32_t	prescaler;
	uint32_t	status;
	uint32_t	interrupt;
	uint32_t	out_cmp_r1;
	uint32_t	out_cmp_r2;
	uint32_t	out_cmp_r3;
	uint32_t	in_cmp_r1;
	uint32_t	in_cmp_r2;
	uint32_t	counter;
};

/* General purpose timers bitfields */
#define GPTCR_SWR		(1 << 15)	/* Software reset */
#define GPTCR_FRR		(1 << 9)	/* Freerun / restart */
#define GPTCR_CLKSOURCE_32	(4 << 6)	/* Clock source */
#define GPTCR_TEN		1		/* Timer enable */
#define CLK_32KHZ		32768		/* 32Khz input */

typedef struct imx_gpt_regs imx_gpt_t;

static const platform_driver_t imx_gpt_driver;

#endif
