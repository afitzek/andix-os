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
 * csu.c
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#include <devices/csu/csu.h>
#include <kprintf.h>

platform_device_t* csu_dev;

/**
 * initialize csu device
 */
void csu_init() {
	csu_dev = NULL;
	csu_dev = hal_find_device(CSU_DEVICE, 0);
}

/**
 * set access permission for device id
 *
 * @param devID The device id
 * @param ap The access permission (see CSU_AP_*)
 *
 * @return 0 on success other on error
 */
int32_t csu_set_device_permission(int32_t devID, int32_t ap) {
	csu_ioctl_param_t param;
	uint32_t hal_res = 0;

	if (csu_dev == NULL ) {
		hal_error("CSU not available for this platform!");
		return (-1);
	}

	if (ap < __CSU_AP_MIN || ap > __CSU_AP_MAX) {
		hal_error("Invalid Access Permission for CSU!");
		return (-1);
	}

	param.access_permission = ap;
	param.device_id = devID;

	hal_res = hal_ioctl(csu_dev, CSU_IOCTL_SET_AP_DEVICE, (uintptr_t)&param,
			sizeof(csu_ioctl_param_t));

	if(hal_res != HAL_SUCCESS) {
		hal_error("Failed to set access permission for device: %s (%d)",
				hal_get_error_string(hal_res), hal_res);
		return (-1);
	}

	return (0);
}
