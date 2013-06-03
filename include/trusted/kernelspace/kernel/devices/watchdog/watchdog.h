/*
 * watchdog.h
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#define WDOG_INIT (0x1)
#define WDOG_PING (0x2)

int wdog_init();
void wdog_ping();

#endif /* WATCHDOG_H_ */
