/*
 * watchdog.h
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/**
 * \addtogroup wdog
 *	\{
 */

#define WDOG_INIT (0x1)
#define WDOG_PING (0x2)

int wdog_init();
void wdog_ping();

/* \} group */

#endif /* WATCHDOG_H_ */
