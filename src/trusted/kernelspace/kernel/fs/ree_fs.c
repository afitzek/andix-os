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
 * ree_fs.c
 *
 *  Created on: Mar 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <fs/ree_fs.h>

int32_t ree_remove(uint8_t *storename, uint8_t *filename) {
	TZ_CTLR_SPACE ctrl;

	ctrl.op = TZ_CTRL_REE_FS_REMOVE;
	ctrl.ret = 0;
	memcpy(ctrl.params.ree_remove.pathname.dirname, storename, TZ_FS_HASH_SIZE);
	memcpy(ctrl.params.ree_remove.pathname.filename, filename, TZ_FS_HASH_SIZE);

	service_do_request(&ctrl);

	if (ctrl.ret != 0) {
		ree_fs_error("Failed to remove file!");
		return (ctrl.ret);
	}

	return (0);
}

int32_t ree_open(uint8_t *storename, uint8_t *filename, int32_t flags,
		int32_t mode) {
	TZ_CTLR_SPACE ctrl;
	int fd;
	ctrl.op = TZ_CTRL_REE_FS_OPEN;
	ctrl.ret = 0;
	memcpy(ctrl.params.ree_open.pathname.dirname, storename, TZ_FS_HASH_SIZE);
	memcpy(ctrl.params.ree_open.pathname.filename, filename, TZ_FS_HASH_SIZE);
	ctrl.params.ree_open.flags = flags;
	ctrl.params.ree_open.flags = mode;

	service_do_request(&ctrl);

	fd = ctrl.ret;

	ree_fs_info("Resulting fd: %d", fd);

	if (fd <= 0) {
		ree_fs_error("Failed to open file!");
	}

	return (fd);
}

int32_t ree_close(int32_t fd) {
	TZ_CTLR_SPACE ctrl;

	ctrl.op = TZ_CTRL_REE_FS_CLOSE;
	ctrl.ret = 0;
	ctrl.params.ree_close.fd = fd;

	service_do_request(&ctrl);

	if (ctrl.ret != 0) {
		ree_fs_error("Failed to close file!");
	}

	return (ctrl.ret);
}

int32_t ree_read(int32_t fd, void* buf, uint32_t count) {
	TZ_CTLR_SPACE ctrl;
	uint8_t* currentBuffer = buf;
	uint32_t currentRead = 0;
	uint32_t left = count;
	uint32_t totalRead = 0;
	int32_t ret;

	while (left > 0) {

		currentRead = (left > TZ_BLOCK_SIZE) ? TZ_BLOCK_SIZE : left;

		ctrl.op = TZ_CTRL_REE_FS_READ;
		ctrl.ret = 0;
		ctrl.params.ree_read.fd = fd;
		ctrl.params.ree_read.count = currentRead;

		service_do_request(&ctrl);

		ret = ctrl.ret;

		if (ret < 0) {
			ree_fs_error("Failed to read file!");
			return (ctrl.ret);
		}

		if (ret == 0) {
			ree_fs_error("Failed to read file! EOF!");
			return (totalRead);
		}

		if (ret == currentRead) {
			memcpy(currentBuffer, ctrl.params.ree_read.buffer, currentRead);
			currentBuffer = currentBuffer + currentRead;
			left = left - currentRead;
			totalRead = totalRead + currentRead;
		} else {
			ree_fs_error("Read return invalid amount of data");
			return (-1);
		}
	}

	return (totalRead);
}

int32_t ree_write(int32_t fd, void* buf, uint32_t count) {
	TZ_CTLR_SPACE ctrl;
	uint8_t* currentBuffer = buf;
	uint32_t currentRead = 0;
	uint32_t left = count;
	uint32_t totalRead = 0;
	int32_t ret;

	while (left > 0) {

		currentRead = (left > TZ_BLOCK_SIZE) ? TZ_BLOCK_SIZE : left;

		ctrl.op = TZ_CTRL_REE_FS_WRITE;
		ctrl.ret = 0;
		ctrl.params.ree_write.fd = fd;
		ctrl.params.ree_write.count = currentRead;
		memcpy(ctrl.params.ree_write.buffer, currentBuffer, currentRead);
		currentBuffer = currentBuffer + currentRead;

		service_do_request(&ctrl);

		ret = ctrl.ret;

		ree_fs_info("Written %d bytes", ret);

		if (ret <= 0) {
			ree_fs_error("Failed to write to file!");
			break;
		}

		if (ret != currentRead) {
			ree_fs_error("Write return invalid amount of data");
			break;
		}
		left = left - currentRead;
		totalRead = totalRead + currentRead;
	}

	return (totalRead);
}

int32_t ree_fstat(int32_t fd, fs_stat* buf) {
	TZ_CTLR_SPACE ctrl;
	ctrl.op = TZ_CTRL_REE_FS_FSTAT;
	ctrl.ret = 0;
	ctrl.params.ree_fstat.fd = fd;

	service_do_request(&ctrl);

	ree_fs_info("plain stat size: %d", ctrl.params.ree_fstat.stat.st_size);

	if (ctrl.ret == 0) {
		buf->st_size = ctrl.params.ree_fstat.stat.st_size;
		//memcpy(buf, &(ctrl.params.ree_fstat.stat), sizeof(REE_FS_STAT));
	}

	return (ctrl.ret);
}

uint32_t ree_lseek(int32_t fd, uint32_t offset, int32_t whence) {
	TZ_CTLR_SPACE ctrl;
	ctrl.op = TZ_CTRL_REE_FS_LSEEK;
	ctrl.ret = 0;
	ctrl.params.ree_lseek.fd = fd;
	ctrl.params.ree_lseek.offset = offset;
	ctrl.params.ree_lseek.whence = whence;

	service_do_request(&ctrl);

	if((uint32_t)-1 == ctrl.ret) {
		return (-1);
	} else {
		return (0);
	}
}
