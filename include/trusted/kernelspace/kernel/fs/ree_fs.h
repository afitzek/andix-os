/*
 * ree_fs.h
 *
 *  Created on: Feb 1, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef REE_FS_H_
#define REE_FS_H_

#include <kprintf.h>
#include <communication_types.h>
#include <task/service.h>
#include <common.h>
#include <fs/fs.h>

int32_t ree_remove(uint8_t *storename, uint8_t *filename) ;
int32_t ree_open(uint8_t *storename, uint8_t *filename, int32_t flags,
		int32_t mode);
int32_t ree_close(int32_t fd);
int32_t ree_read(int32_t fd, void* buf, uint32_t count);
int32_t ree_write(int32_t fd, void* buf, uint32_t count);
int32_t ree_fstat(int32_t fd, fs_stat* buf);
uint32_t ree_lseek(int32_t fd, uint32_t offset, int32_t whence);

#endif /* REE_FS_H_ */
