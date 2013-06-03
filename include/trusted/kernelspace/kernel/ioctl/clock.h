/*
 * clock.h
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef CLOCK_H_
#define CLOCK_H_

/**
 * Clock IOCTL request ...
 */

#define IOCTL_CLOCK_GET_CTR			(0x00000001)
#define IOCTL_CLOCK_RESET_CTR		(0x00000002)
#define IOCTL_CLOCK_GET_FREQ_KHZ	(0x00000003)
#define IOCTL_CLOCK_ALARM_AT		(0x00000004)

struct clk_request {
	uint32_t value;
};

typedef struct clk_request clk_request_t;

#endif /* CLOCK_H_ */
