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


#endif
