/*
 * k_comm.c
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#include <andixtz.h>
#include <k_comm.h>
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
	if (comm != NULL ) {
		if (tee_fd != 0) {
			printf("CALLING TZ TEE\n");
			ioctl(tee_fd, ANDIX_IOCTEEZ);
			printf("RETURN FROM TZ TEE\n");
			return 0;
		}
	}
	return -1;
}

TZ_TEE_SPACE * getCommSpace() {
	if (comm == NULL ) {
		if (tee_fd == 0) {
			openTEEDevice();
		}
		mapCommMem();
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

void unmapCommMem() {
	if (comm != NULL ) {
		printf("unmapping communication memory\n");
		munmap(comm, sizeof(TZ_TEE_SPACE));
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

void mapCommMem() {
	uint32_t poff;
	uint32_t orig;

	printf("mapping comm memory!\n");

	comm = (TZ_TEE_SPACE *) mmap(NULL, sizeof(TZ_TEE_SPACE) + 0x1000,
			PROT_READ | PROT_WRITE, MAP_SHARED, tee_fd, 0);
	orig = (uint32_t)comm;
	if (comm == NULL || comm == MAP_FAILED ) {
		printf("FAILED TO map tee memory: %s\n", (char*) strerror(errno));
		doexit();
	}

	printf("TEE memory is mapped @ 0x%08x\n", (uint32_t)comm);

	poff = ioctl(tee_fd, ANDIX_IOGMOFF);

	printf("TEE memory page offset 0x%08x\n", poff);

	comm = (TZ_TEE_SPACE*) (((uint32_t) comm) | poff);

	printf("TEE memory is located @ 0x%08x\n", (uint32_t)comm);
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
	unmapCommMem();
	closeTEEDevice();
}
