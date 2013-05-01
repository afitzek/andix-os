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

int tee_fd = 0;
TZ_TEE_SPACE *comm = NULL;

int processComm() {
	int res = -1;
	if (comm != NULL ) {
		if (tee_fd != 0) {
			printf("CALLING TZ TEE comm (0x%x)\n", (unsigned int)comm);
			res = ioctl(tee_fd, ANDIX_TEE_PUSH, comm);
			printf("RETURN FROM TZ TEE\n");
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
	comm = (TZ_TEE_SPACE*)malloc(sizeof(TZ_TEE_SPACE));
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
