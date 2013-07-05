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
 * watchdog.c
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#include <devices/watchdog/watchdog.h>
#include <hal.h>
#include <common.h>

platform_device_t* wdog;

int wdog_init() {
	uint32_t ioctl_res = 0;
	wdog = NULL;
	wdog = hal_find_device(WATCHDOG_DEVICE, 0);

	if (wdog == NULL ) {
		hal_error("Failed to find platform watchdog...");
		return (-1);
	}

	ioctl_res = hal_ioctl(wdog, WDOG_INIT, NULL, 0);

	if (ioctl_res != HAL_SUCCESS) {
		hal_error("Failed to initialize watchdog! %s (0x%x)",
				hal_get_error_string(ioctl_res), ioctl_res);
		return (-1);
	}

	return (0);
}

void wdog_ping() {
	if (wdog != NULL ) {
		if(hal_ioctl(wdog, WDOG_PING, NULL, 0) != HAL_SUCCESS) {
			hal_error("Watchdog ping failed!");
		}
	}
}
