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
 * k_comm.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <andixtz.h>
#include <k_comm.h>
#include <andix_tz_mod.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <tee_client_api.h>

int tee_fd = 0;
TZ_TEE_SPACE *comm = NULL;

int processParameters(TEEC_Operation* source, TEECOM_Operation* target) {
	int pidx = 0;
	uint32_t paramType = 0;
	uint32_t pTypeSource = source->paramTypes;
	target->paramTypes = source->paramTypes;
	target->started = source->started;
	//printf("Processing parameters\n");
	for (pidx = 0; pidx < 4; pidx++) {
		//printf("Getting Parameter type\n");
		paramType = TEE_PARAM_TYPE_GET(pTypeSource, pidx);
		//printf("Getting Parameter type is %d\n", paramType);
		switch (paramType) {
		case TEEC_NONE:
			//printf("Processing parameters %d None\n", pidx);
			target->params[pidx].value.a = 0;
			target->params[pidx].value.b = 0;
			//printf("Processing parameters %d done\n", pidx);
			break;
		case TEEC_VALUE_INPUT:
		case TEEC_VALUE_INOUT:
		case TEEC_VALUE_OUTPUT:
			target->params[pidx].value.a =
					source->params[pidx].value.a;
			target->params[pidx].value.b =
					source->params[pidx].value.b;
			//printf("Processing parameters %d done\n", pidx);
			break;
		case TEEC_MEMREF_TEMP_INPUT:
		case TEEC_MEMREF_TEMP_OUTPUT:
		case TEEC_MEMREF_TEMP_INOUT:
			target->params[pidx].tmpref.buffer =
					source->params[pidx].tmpref.buffer;
			target->params[pidx].tmpref.size =
					source->params[pidx].tmpref.size;
			break;
		case TEEC_MEMREF_PARTIAL_INOUT:
		case TEEC_MEMREF_PARTIAL_INPUT:
		case TEEC_MEMREF_PARTIAL_OUTPUT:
		case TEEC_MEMREF_WHOLE:
			//printf("Processing parameters %d 0x%x 0x%x 0x%x\n",
			//		pidx, target, source, source->params[pidx].memref.parent);
			target->params[pidx].memref.memid =
					source->params[pidx].memref.parent->memid;
			target->params[pidx].memref.size =
					source->params[pidx].memref.size;
			target->params[pidx].memref.offset =
					source->params[pidx].memref.offset;
			//printf("Processing parameters %d done\n", pidx);
			break;
		default:
			return (TEEC_ERROR_BAD_PARAMETERS);
		}
	}
	//printf("Processing parameters done\n");
	return (TEEC_SUCCESS);
}

int processComm() {
	int res = -1;
	if (comm != NULL ) {
		if (tee_fd != 0) {
			//printf("CALLING TZ TEE comm (0x%x)\n", (unsigned int) comm);
			res = ioctl(tee_fd, ANDIX_TEE_PUSH, comm);
			//printf("RETURN FROM TZ TEE\n");
		}
	}
	return (res);
}

TZ_TEE_SPACE* getCommSpace() {
	if (comm == NULL ) {
		if (tee_fd == 0) {
			openTEEDevice();
		}
		allocCommMem();
		atexit(doexit);
	}
	return comm;
}

void closeTEEDevice() {
	if (tee_fd > 0) {
		printf("closing TZ tee device\n");
		close(tee_fd);
		tee_fd = 0;
	}
}

void freeCommMem() {
	if (comm != NULL ) {
		printf("freeing communication memory\n");
		free(comm);
		comm = NULL;
	}
}

void openTEEDevice() {
	tee_fd = open(DEVICE_NAME, O_RDWR);

	if (tee_fd < 0) {
		printf("FAILED TO open device: %s\n", (char*) strerror(errno));
		doexit();
	}
}

void allocCommMem() {
	freeCommMem();
	comm = (TZ_TEE_SPACE*) malloc(sizeof(TZ_TEE_SPACE));
}

pthread_mutex_t commMutex = PTHREAD_MUTEX_INITIALIZER;

void lockComm() {
	pthread_mutex_lock(&commMutex);
}

void unlockComm() {
	pthread_mutex_unlock(&commMutex);
}

void doexit() {
	printf("existing\n");
	cleanup();
}

void cleanup() {
	printf("cleanup\n");
	freeCommMem();
	closeTEEDevice();
}
