/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/*
 * fs.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef FS_H_
#define FS_H_

#include <communication_types.h>

#define CRYPTO_FS_INIT_MAGIC (0x8d7d3618)

#define CFS_BLK_SIZE			(940)

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC         04010000
#define O_FSYNC          O_SYNC
#define O_ASYNC          020000

typedef REE_FS_STAT fs_stat;

/**
 * File handle
 */
typedef struct {
	int32_t		fd;
	uint32_t	filepos;
	uint8_t		key_data[32];
	uint32_t	init;
} cfs_t;

/**
 * First file block
 */
typedef struct {
	uint8_t		key_data[32];
	uint8_t		filename[TZ_FS_HASH_SIZE];
	uint8_t		storename[TZ_FS_HASH_SIZE];
} cfs_info_blk_t;

typedef struct {
	uint32_t	count;
	uint8_t		blk[CFS_BLK_SIZE];
} cfs_data_blk_t;

typedef union {
	cfs_data_blk_t blk;
	cfs_info_blk_t info;
} cfs_data_t;

/**
 * Crypto block
 */
typedef struct {
	uint8_t		iv[16];
	uint8_t		hmac[32];
	uint8_t		hash[32];
	cfs_data_t  data;
} cfs_blk_t;

int32_t fs_remove(uint8_t *storename, uint32_t slen,
		uint8_t *filename, uint32_t flen);
int32_t fs_open(uint8_t *storename, uint32_t slen,
		uint8_t *filename, uint32_t flen, int32_t flags,
		int32_t mode, cfs_t* handle);
int32_t fs_close(cfs_t* handle);
int32_t fs_read(cfs_t* handle, void* buf, uint32_t count);
int32_t fs_write(cfs_t* handle, void* buf, uint32_t count);
int32_t fs_fstat(cfs_t* handle, fs_stat* buf);
uint32_t fs_lseek(cfs_t* handle, uint32_t offset, int32_t whence);
void fs_set_secret(uint8_t* secret, uint32_t len);

#endif /* FS_H_ */
