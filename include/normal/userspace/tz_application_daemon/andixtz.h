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

#define ANDIXTZ_IOC_MAGIC  'T'
#define ANDIX_IOCTZ 		_IOW(ANDIXTZ_IOC_MAGIC,  1, int)
#define ANDIX_IOGMOFF		_IOW(ANDIXTZ_IOC_MAGIC,  2, int)
#define ANDIX_IOCTEEZ 		_IOW(ANDIXTZ_IOC_MAGIC,  3, int)

#define DEVICE_NAME			"/dev/andixtee0"
#define SOCKET_NAME			"/tmp/tee.socket"
#define BACKLOG				10

void (*service_handler)(TZ_CTLR_SPACE* ctrl);

#endif /* ANDIXTZ_H_ */
