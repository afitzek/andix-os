/*
 * blockdev.h
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef BLOCKDEV_H_
#define BLOCKDEV_H_

void create_tz_file(TZ_CTLR_SPACE* space);
void open_tz_file(TZ_CTLR_SPACE* space);
void read_tz_file_block(TZ_CTLR_SPACE* space);
void close_tz_file(TZ_CTLR_SPACE* space);
void write_tz_file_block(TZ_CTLR_SPACE* space);

#endif /* BLOCKDEV_H_ */
