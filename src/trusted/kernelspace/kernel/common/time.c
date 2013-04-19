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
