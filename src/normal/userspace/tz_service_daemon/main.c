/*
 * main.c
 *
 *  Created on: Jan 16, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <stdio.h>
#include <errno.h>
#include <andixtz.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

int openTZ() {
	return (open(DEVICE_NAME, O_RDWR));
}

void dump_mem(uint8_t* ptr, uint32_t len) {
	uint32_t idx = 0;
	uint32_t jdx = 0;
	uint32_t t = 0;
	uint32_t left = 0;
	for (idx = 0; idx < len; idx++) {
		if (idx != 0 && (idx % 0xF) == 0) {
			printf("| ");
			for (jdx = 0xF; jdx > 0; jdx--) {
				if (ptr[idx - jdx] >= 0x20 && ptr[idx - jdx] < 0x7E) {
					printf("%c", (char) ptr[idx - jdx]);
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
		printf("%02X ", ptr[idx]);
	}

	left = 0xF - (idx & 0xF);

	for (t = 0; t < left; t++) {
		printf("   ");
	}

	printf("| ");
	for (jdx = 0xF; jdx > 0; jdx--) {
		if ((idx - jdx) < len) {
			if (ptr[idx - jdx] >= 0x20 && ptr[idx - jdx] < 0x7E) {
				printf("%c", (char) ptr[idx - jdx]);
			} else {
				printf(".");
			}
		}
	}
	printf("\n");
}

void dispatch_ctrl(TZ_CTLR_SPACE* space) {
	switch (space->op) {
	case TZ_CTRL_REE_FS_REMOVE:
		printf("TZ_CTRL_REE_REMOVE\n");
		tz_fs_remove(space);
		break;
	case TZ_CTRL_REE_FS_OPEN:
		printf("TZ_CTRL_REE_OPEN\n");
		tz_fs_open(space);
		break;
	case TZ_CTRL_REE_FS_CLOSE:
		printf("TZ_CTRL_REE_CLOSE\n");
		tz_fs_close(space);
		break;
	case TZ_CTRL_REE_FS_READ:
		printf("TZ_CTRL_REE_READ\n");
		tz_fs_read(space);
		break;
	case TZ_CTRL_REE_FS_WRITE:
		printf("TZ_CTRL_REE_WRITE\n");
		tz_fs_write(space);
		break;
	case TZ_CTRL_REE_FS_LSEEK:
		printf("TZ_CTRL_REE_LSEEK\n");
		tz_fs_lseek(space);
		break;
	case TZ_CTRL_REE_FS_FSTAT:
		printf("TZ_CTRL_REE_FSTAT\n");
		tz_fs_fstat(space);
		break;
	}
}

void dispatcher_loop(int fd, TZ_CTLR_SPACE *comm) {

	comm->op = TZ_CTRL_OP_FORK;
	int8_t running = 1;
	ioctl(fd, ANDIX_IOCTZ);
	comm->op = TZ_CTRL_OP_IDLE;
	while (running) {
		ioctl(fd, ANDIX_IOCTZ);
		printf("Got OP 0x%x\n", comm->op);
		if (comm->op == TZ_CTRL_OP_DEXIT) {
			running = 0;
			continue;
		} else {
			dispatch_ctrl(comm);
		}
	}
}

int main(int argc, char** argv) {
	int fd_tz;
	int err;
	TZ_CTLR_SPACE *comm;

	printf("################## TZ SERVICE DAEMON ##################\n");

	printf("opening TZ device:\n");

	fd_tz = openTZ();

	if (fd_tz < 0) {
		printf("FAILED TO open device: %s\n", (char*)strerror(errno));
		goto cleanup;
	}

	printf("device is open ...\n");
	fflush(stdout);
	fflush(stderr);

	comm = (TZ_CTLR_SPACE *) mmap(NULL, sizeof(TZ_CTLR_SPACE) + 0x1000,
			PROT_READ | PROT_WRITE, MAP_SHARED, fd_tz, 0);
	uint32_t orig = (uint32_t)comm;
	if (comm == NULL || comm == MAP_FAILED ) {
		printf("FAILED TO map communication memory: %s\n", (char*)strerror(errno));
		goto cleanup;
	}

	printf("Communication memory is mapped @ 0x%08x\n", (uint32_t)comm);

	uint32_t poff = ioctl(fd_tz, ANDIX_IOGMOFF);

	printf("Communication memory page offset 0x%08x\n", poff);

	comm = (TZ_CTLR_SPACE*) (((uint32_t) comm) | poff);

	printf("Communication memory is located @ 0x%08x\n", (uint32_t)comm);

	dispatcher_loop(fd_tz, comm);

	printf("################## TZ SERVICE DAEMON ##################\n");

	cleanup:

	if (comm != NULL && comm != MAP_FAILED ) {
		munmap(comm, sizeof(TZ_CTLR_SPACE));
	}

	if (fd_tz > 0) {
		printf("closing TZ device\n");
		close(fd_tz);
	}

	return (0);
}
