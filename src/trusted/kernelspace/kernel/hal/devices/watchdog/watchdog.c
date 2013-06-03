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
