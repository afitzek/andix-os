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

#define DEVICE_NAME			"/dev/andixtee0"

#define TZ_ROOT				"/tz/"
#define TZ_FEND				".tz"
#define TZ_SEPERATOR		"/"

void (*service_handler)(TZ_CTLR_SPACE* ctrl);

#endif /* ANDIXTZ_H_ */
