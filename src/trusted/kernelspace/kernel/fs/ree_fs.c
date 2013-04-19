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

	ctrl.op = TZ_CTRL_REE_FS_OPEN;
	ctrl.ret = 0;
	memcpy(ctrl.params.ree_open.pathname.dirname, storename, TZ_FS_HASH_SIZE);
	memcpy(ctrl.params.ree_open.pathname.filename, filename, TZ_FS_HASH_SIZE);
	ctrl.params.ree_open.flags = flags;
	ctrl.params.ree_open.flags = mode;

	service_do_request(&ctrl);

	if (ctrl.ret <= 0) {
		ree_fs_error("Failed to open file!");
	}

	return (ctrl.ret);
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

	if (ctrl.ret == 0) {
		memcpy(buf, &(ctrl.params.ree_fstat.stat), sizeof(REE_FS_STAT));
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
