/*
 * ctime.h
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef CTIME_H_
#define CTIME_H_

#include <common/typedefs.h>

/**
 * Clock type
 */
typedef uint32_t		clock_t;

/**
 * Initialize timing
 */
void timing_init();

/**
 * Set time clock frequency
 */
void timing_get_clock_freq();

/**
 * Get current clocks ticks
 * @return
 */
clock_t clock();

/**
 * Transform seconds to clock ticks
 * @param sec The seconds
 * @return the clock ticks
 */
clock_t sec_to_clocks(uint32_t sec);

/**
 * Transform milliseconds to clock ticks
 * @param usec The milliseconds
 * @return the clock ticks
 */
clock_t usec_to_clocks(uint32_t usec);

/**
 * Sleep for seconds
 * @param sec number of seconds
 */
void sleep(uint32_t sec);

/**
 * Sleep for milliseconds
 * @param usec numer of milliseconds
 */
void usleep(uint32_t usec);

#endif /* CTIME_H_ */
