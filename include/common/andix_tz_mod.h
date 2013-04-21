/*
 * andix_tz_mod.h
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#ifndef ANDIX_TZ_MOD_H_
#define ANDIX_TZ_MOD_H_

#define ANDIXTZ_IOC_MAGIC  'T'
#define ANDIX_CTRL_POLL		_IOW(ANDIXTZ_IOC_MAGIC,  1, int)
#define ANDIX_CTRL_PUSH		_IOW(ANDIXTZ_IOC_MAGIC,  2, int)
#define ANDIX_TEE_PUSH		_IOW(ANDIXTZ_IOC_MAGIC,  3, int)

#define ANDIX_IOC_MAXNR 3


#endif /* ANDIX_TZ_MOD_H_ */
