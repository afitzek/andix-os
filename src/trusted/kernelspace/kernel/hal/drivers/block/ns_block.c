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
 * ns_block.c
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <drivers/block/ns_block.h>
#include <communication_types.h>
#include <mm/mm.h>
#include <task/service.h>

int andix_ns_block_probe(platform_device_t *dev) {

	ns_block_dev* dev_data = (ns_block_dev*) kmalloc(sizeof(ns_block_dev));

	hal_info("Probe blk device!");

	if (dev_data == NULL ) {
		hal_error("Out of memory!");
		return (HAL_E_OUT_OF_MEMORY);
	}

	dev_data->nsfd = 0;

	dev->device_data = (uintptr_t)dev_data;

	return (HAL_SUCCESS);
}

void andix_ns_block_release(platform_device_t *dev) {
	TZ_CTLR_SPACE ctrl;
	ns_block_dev* bdev = (ns_block_dev*)(dev->device_data);

	if (bdev != NULL ) {
		hal_info("Releasing blk device! 0x%x", bdev->nsfd);
		ctrl.op = TZ_CTRL_OP_BCLOSE;
		ctrl.params.bopen.nsfd = bdev->nsfd;
		ctrl.params.bopen.flags = 0;
		service_do_request(&ctrl);
	}
	if (dev->device_data) {
		kfree(dev->device_data);
	}
}

uint32_t andix_ns_block_create(platform_device_t *dev, ns_block_setup* setup) {
	TZ_CTLR_SPACE ctrl;

	ns_block_dev* bdev = (ns_block_dev*)(dev->device_data);

	if (bdev == NULL ) {
		return (HAL_E_INVALID_DEV);
	}

	hal_info("Creating blk device!");
	ctrl.op = TZ_CTRL_OP_BCREATE;
	ctrl.params.bopen.flags = 0;
	memcpy(ctrl.params.bopen.filename, setup->device_name, 32);

	service_do_request(&ctrl);

	if (ctrl.ret != 0) {
		hal_error("Failed to create blk device!");
		return (HAL_E_INVALID_DEV);
	}

	return (0);
}

uint32_t andix_ns_block_setup(platform_device_t *dev, ns_block_setup* setup) {
	TZ_CTLR_SPACE ctrl;

	ns_block_dev* bdev = (ns_block_dev*)(dev->device_data);

	if (bdev == NULL ) {
		return (HAL_E_INVALID_DEV);
	}
	hal_info("Opening blk device!");
	ctrl.op = TZ_CTRL_OP_BOPEN;
	ctrl.params.bopen.flags = 0;
	memcpy(ctrl.params.bopen.filename, setup->device_name, 32);

	service_do_request(&ctrl);

	if (ctrl.ret != 0) {
		hal_error("Failed to open blk device!");
		return (HAL_E_INVALID_DEV);
	}

	bdev->nsfd = ctrl.params.bopen.nsfd;
	hal_info("Setting nsfd: 0x%x 0x%x", bdev->nsfd, ctrl.params.bopen.nsfd);

	return (0);
}

uint32_t andix_ns_block_write(platform_device_t *dev, ns_block_blk* blk) {
	TZ_CTLR_SPACE ctrl;

	hal_info("Write blk device!");

	ns_block_dev* bdev = (ns_block_dev*)(dev->device_data);

	if (bdev == NULL ) {
		return (HAL_E_INVALID_DEV);
	}

	if ((blk->offset % BLOCK_SIZE) != 0){
		return (1);
	}

	ctrl.op = TZ_CTRL_OP_BWRITE;
	ctrl.params.bwrite.blockpos = blk->offset;
	ctrl.params.bwrite.nsfd = bdev->nsfd;
	memcpy(ctrl.params.bwrite.buffer, blk->blk_data, 1024);

	service_do_request(&ctrl);

	//TODO: error handling!

	return (ctrl.ret);
}

uint32_t andix_ns_block_read(platform_device_t *dev, ns_block_blk* blk) {
	TZ_CTLR_SPACE ctrl;

	hal_info("Read blk device!");

	ns_block_dev* bdev = (ns_block_dev*)(dev->device_data);

	if (bdev == NULL ) {
		return (HAL_E_INVALID_DEV);
	}

	if ((blk->offset % BLOCK_SIZE)!= 0){
		return (1);
	}

	ctrl.op = TZ_CTRL_OP_BREAD;
	ctrl.params.bwrite.blockpos = blk->offset;
	ctrl.params.bwrite.nsfd = bdev->nsfd;

	service_do_request(&ctrl);

	if (ctrl.ret == 0) {
		memcpy(blk->blk_data, ctrl.params.bwrite.buffer, 1024);
	}

	//TODO: error handling!
	return (ctrl.ret);
}

uint32_t andix_ns_block_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	if (dev == NULL ) {
		return (HAL_E_INVALID_DEV);
	}
	ns_block_dev* dinfo = NULL;

	dinfo = (ns_block_dev*) dev->device_data;
	if (dinfo == NULL ) {
		return (HAL_E_INVALID_DEV);
	}

	switch (request) {
	case NS_BLK_CREATE:
		if (psize != sizeof(ns_block_setup)) {
			hal_error("Parameter size invalid!");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (param == NULL ) {
			hal_error("Parameter is null");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (dinfo->nsfd != 0) {
			hal_error("NS File descriptor is initialised");
			return (HAL_E_FUNC_NOT_AVAIL);
		}
		return (andix_ns_block_create(dev, (ns_block_setup*) param));
	case NS_BLK_SETUP:
		if (psize != sizeof(ns_block_setup)) {
			hal_error("Parameter size invalid!");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (param == NULL ) {
			hal_error("Parameter is null");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (dinfo->nsfd != 0) {
			hal_error("NS File descriptor is initialised");
			return (HAL_E_FUNC_NOT_AVAIL);
		}
		return (andix_ns_block_setup(dev, (ns_block_setup*) param));
	case NS_BLK_WRITEBLK:
		if (psize != sizeof(ns_block_blk)) {
			hal_error("Parameter size invalid!");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (param == NULL ) {
			hal_error("Parameter is null");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (dinfo->nsfd == 0) {
			hal_error("NS File descriptor is not initialised");
			return (HAL_E_FUNC_NOT_AVAIL);
		}
		return (andix_ns_block_write(dev, (ns_block_blk*) param));
	case NS_BLK_READBLK:
		if (psize != sizeof(ns_block_blk)) {
			hal_error("Parameter size invalid!");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (param == NULL ) {
			hal_error("Parameter is null");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		if (dinfo->nsfd == 0) {
			hal_error("NS File descriptor is not initialised");
			return (HAL_E_FUNC_NOT_AVAIL);
		}
		return (andix_ns_block_read(dev, (ns_block_blk*) param));
	default:
		hal_error("IOCTL CALL NOT AVAILABLE!");
		return (HAL_E_IOCTL_REQ_NOT_AVAIL);
	}
}

static const platform_driver_t andix_ns_block_driver = { .probe =
		andix_ns_block_probe, .release = andix_ns_block_release, .write = NULL,
		.read = NULL, .ioctl = andix_ns_block_ioctl, .driver = { .name =
				ANDIX_NS_BLOCK_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(andix_ns_block_driver)
