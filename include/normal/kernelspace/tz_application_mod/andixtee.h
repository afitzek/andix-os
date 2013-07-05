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
 * module.h
 *
 *  Created on: Nov 30, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ANDIXTZ_H_
#define ANDIXTZ_H_

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/mutex.h>
#include <communication_types.h>
#include <tz_application_mod/types.h>
#include <tz_application_mod/smc.h>
#include <tz_application_mod/utils.h>
#include <tz_application_mod/ctrl_mem.h>
#include <tz_application_mod/tee_mem.h>

#ifndef KERN_ERR
#define KERN_EMERG  "<0>"   /* system is unusable               */
#define KERN_ALERT  "<1>"   /* action must be taken immediately */
#define KERN_CRIT   "<2>"   /* critical conditions              */
#define KERN_ERR    "<3>"   /* error conditions                 */
#define KERN_WARNING"<4>"       /* warning conditions               */
#define KERN_NOTICE "<5>"   /* normal but significant condition */
#define KERN_INFO   "<6>"   /* informational                    */
#define KERN_DEBUG  "<7>"   /* debug-level messages             */
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE (0x1000)
#endif

#define CTRL_IDLE				(0)
#define CTRL_PENDING_FROM_S		(1)
#define CTRL_PENDING_IN_NS		(2)
#define CTRL_RESPONSE_FROM_NS	(3)

#define DRIVER_AUTHOR	"Andreas Fitzek <andreas.fitzek@iaik.tugraz.at>"
#define DRIVER_DESC		"Andix TZ TEE Driver"
#define DEVICE_NAME		"andixtee"
#define BUF_LEN			80

#define SUCCESS			0

#define SMC_REGISTER_CMEM	0x01
#define SMC_UNREGISTER_CMEM	0x02
#define SMC_PROCESS_CMEM	0x03

#define SMC_REGISTER_TMEM	0x11
#define SMC_UNREGISTER_TMEM	0x12
#define SMC_PROCESS_TMEM	0x13


// =============================================================
// Global Variables
// =============================================================
extern uint32_t tz_device;
extern uint8_t initialized;
extern struct file_operations tz_fops;
extern struct cdev cdev;

// =============================================================

// =============================================================
// Main Module Methods
// =============================================================
int _init_tz_driver( void );
void _exit_tz_driver( void );
// =============================================================

int needed_log_pages(size_t memsize);

// =============================================================
// TZ File operations
// =============================================================
int tz_driver_open(struct inode *, struct file *);
int tz_driver_release(struct inode *, struct file *);
long tz_driver_ioctl(struct file *, unsigned int, unsigned long);
//int tz_driver_mmap(struct file *, struct vm_area_struct *);
// =============================================================

int tz_process_ctrl_mem(TZ_TEE_SPACE* teespace, TZ_CTLR_SPACE* ctrlspace,
		TZ_PACKAGE *package);
int tz_process_tee_mem(TZ_TEE_SPACE* userspace);

// =============================================================
// TrustZone Service
// =============================================================
//int register_tee_mem_in_tz(TZ_TEE_SPACE *com);
//int unregister_tee_mem_from_tz(void);
// =============================================================


#endif /* ANDIXTZ_H_ */
