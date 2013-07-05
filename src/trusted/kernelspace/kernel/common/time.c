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
 * ctime.c
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/time.h>
#include <hal.h>

platform_device_t* timer = NULL;

uint32_t clk_freq_khz = 0;

void timing_init() {
	timer = hal_find_device(TIMER_DEVICE, 0);
	hal_debug("clock time timer @ 0x%x", timer);
	timing_get_clock_freq();
}

void timing_get_clock_freq() {
	if (timer) {
		clk_request_t req;
		if (hal_ioctl(timer, IOCTL_CLOCK_GET_FREQ_KHZ, (uintptr_t)&req,
				sizeof(clk_request_t)) == HAL_SUCCESS) {
			clk_freq_khz = req.value;
		}
	}
}

clock_t clock() {
	if (timer) {
		clk_request_t req;
		if (hal_ioctl(timer, IOCTL_CLOCK_GET_CTR, (uintptr_t)&req,
				sizeof(clk_request_t)) == HAL_SUCCESS) {
			return (req.value);
		}
	}
	return (0);
}

clock_t sec_to_clocks(uint32_t sec) {
	if (clk_freq_khz) {
		return (sec * clk_freq_khz);
	}
	return (0);
}

clock_t usec_to_clocks(uint32_t usec) {
	if (clk_freq_khz) {
		return ((usec * clk_freq_khz) / 1000000);
	}
	return (0);
}

void sleep(uint32_t sec) {
	clock_t now = clock();
	uint32_t tmp = now + sec_to_clocks(sec);

	while (clock() < tmp)
		NOP
	;
}

void usleep(uint32_t usec) {
	clock_t now = clock();
	uint32_t tmp = now + usec_to_clocks(usec);

	while (clock() < tmp)
		NOP
	;
}
