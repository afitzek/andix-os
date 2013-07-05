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
 * hal_doc.h
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef HAL_DOC_H_
#define HAL_DOC_H_

/**
 * \defgroup hal HAL - Hardware Abstraction Layer
 *
 * Abstraction layer for hardware access.
 *
 * It divides into devices module which defines high level API for common
 * device classes and the driver module which implements such a device
 * class for a special hardware instance.
 */



/**
 * \addtogroup hal
 *	\{
 */

/**
 * \defgroup dev Devices
 *
 * Generic device API
 */

/**
 * \defgroup driver Drivers
 *
 * Specific drivers
 */

/* \} group */

/**
 * \addtogroup dev
 *	\{
 */

/**
 * \defgroup csu CSU - Central Security Unit
 *
 * Central Security Unit is used to manage permission for device access
 */

/**
 * \defgroup irq_ctrl IRQ_CTRL - Interrupt controller
 *
 * Allows the registration of interrupt handlers. The registered interrupts
 * are hidden from the normal world.
 */

/**
 * \defgroup hal_random RANDOM - Hardware abstraction for random generation
 *
 * Abstracts random number generation, can be implemented with real hardware
 * or pseudo random numbers
 */

/**
 * \defgroup wdog Watchdog
 *
 * Watchdog resets the platform if not pinged periodic
 */

/**
 * \defgroup tzmem_ctrl TrustZone Memory Controller
 *
 * TrustZone Memory Controller can lockdown memory regions from nonsecure
 * access.
 */

/* \} group */


/**
 * \addtogroup driver
 *	\{
 */

/**
 * \defgroup imx53_csu IMX53 CSU - Central Security Unit
 *
 * Driver for Central Security Unit is used to manage permission for device access
 */

/**
 * \defgroup imx53_tzic IMX53 TZIC - TrustZone Aware Interrupt Controller
 *
 * Driver for TrustZone aware interrupt controller
 */

/* \} group */


#endif
