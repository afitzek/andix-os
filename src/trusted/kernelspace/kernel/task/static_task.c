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
 * static_task.c
 *
 *  Created on: Jan 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <task/task.h>
#include <loader.h>

task_t* prepare_static_userspace_task(uintptr_t start, uintptr_t end,
		char* name, char* uuid_string) {
	//TASK_UUID uuid;
	task_debug("Preparing %s task from elf@ 0x%x - 0x%x", name, start, end);
	task_t* user_elf = create_user_task((uint8_t*)start, end - start);
	task_set_name(user_elf, name);
	uuid_parse(uuid_string, &user_elf->uuid);
	// 47b57610-925b-11e2-9e96-0800200c9a66
	/*user_elf->uuid.timeLow = 0x47b57610;
	user_elf->uuid.timeMid = 0x925b;
	user_elf->uuid.timeHiAndVersion = 0x11e2;
	user_elf->uuid.clockSeqAndNode[7] = 0x9e;
	user_elf->uuid.clockSeqAndNode[6] = 0x96;
	user_elf->uuid.clockSeqAndNode[5] = 0x08;
	user_elf->uuid.clockSeqAndNode[4] = 0x00;
	user_elf->uuid.clockSeqAndNode[3] = 0x20;
	user_elf->uuid.clockSeqAndNode[2] = 0x0c;
	user_elf->uuid.clockSeqAndNode[1] = 0x9a;
	user_elf->uuid.clockSeqAndNode[0] = 0x66;*/
	add_task(user_elf);
	set_sysd_task(user_elf);
	return (user_elf);
}

void init_userspace_task(task_t* task) {
	__smc_1(SSC_TASK_SWITCH, (uint32_t) task);
}
