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
 * csu.h
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#ifndef CSU_H_
#define CSU_H_

#include <common/typedefs.h>
#include <platform/vector_debug.h>
#include <hal.h>

/**
 * @addtogroup csu
 *	@{
 */

/**
 * IOCTL call to set access permission for device
 */
#define CSU_IOCTL_SET_AP_DEVICE (0x1)

/**
 * IOCTL parameter structure for csu
 */
typedef struct {
	/**
	 * The device id
	 */
	int32_t device_id;

	/**
	 * The access permission
	 */
	int32_t access_permission;
} csu_ioctl_param_t;

#define __CSU_AP_MIN (0x0)
#define __CSU_AP_MAX (0x7)

/**
 * Non-Secure UserMode RD+RW
 * Non-Secure SpvrMode RD+RW
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_RW_K_RW_S_U_RW_K_RW (0x0)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode RD+RW
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_RW_S_U_RW_K_RW (0x1)

/**
 * Non-Secure UserMode RD
 * Non-Secure SpvrMode RD
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_RD_K_RD_S_U_RW_K_RW (0x2)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode RD
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_RD_S_U_RW_K_RW (0x3)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_RW_K_RW (0x4)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode NONE
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_NO_K_RW (0x5)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode RD
 * Secure SpvrMode RD
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_RD_K_RD (0x6)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode NONE
 * Secure SpvrMode NONE
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_NO_K_NO (0x7)

/**
 * initialize csu device
 */
void csu_init();

/**
 * set access permission for device id
 *
 * @param devID The device id
 * @param ap The access permission (see CSU_AP_*)
 *
 * @return 0 on success other on error
 */
int32_t csu_set_device_permission(int32_t devID, int32_t ap);

/* @} group */

#endif /* CSU_H_ */
