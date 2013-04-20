/*
 * andixtz.h
 *
 *  Created on: Jan 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ANDIXTZ_H_
#define ANDIXTZ_H_

#include <communication_types.h>

#define ANDIXTZ_IOC_MAGIC  'Y'
#define ANDIX_IOCTZ 		_IOW(ANDIXTZ_IOC_MAGIC,  1, int)
#define ANDIX_IOGMOFF		_IOW(ANDIXTZ_IOC_MAGIC,  2, int)

#define DEVICE_NAME			"/dev/andixtz0"

#define TZ_ROOT				"/tz/"
#define TZ_FEND				".tz"
#define TZ_SEPERATOR		"/"

void (*service_handler)(TZ_CTLR_SPACE* ctrl);

#endif /* ANDIXTZ_H_ */
