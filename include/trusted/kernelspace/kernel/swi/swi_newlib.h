/*
 * swi_newlib.h
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SWI_NEWLIB_H_
#define SWI_NEWLIB_H_

int32_t swi_fstat(int file);
int32_t swi_lseek(int32_t file, uint32_t ptr, int32_t dir);
int32_t swi_write(uint32_t socket, uint8_t* buffer, uint32_t size);
int32_t swi_read(uint32_t socket, uint8_t* buffer, uint32_t size);
uint32_t swi_heapend();
int32_t swi_close(int file);
int32_t swi_open(char *name, int flags, int mode);
int32_t swi_sbrk(int32_t incr);
uint32_t swi_get_pid();

#endif /* SWI_NEWLIB_H_ */
