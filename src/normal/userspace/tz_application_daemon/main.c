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
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

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

void handle(int newsock, fd_set *set) {
	// TODO:
	// recv TEE request from newsock
	// make TZ TEE request
	// send response
	// if exit command remove newsock
}

void dispatcher_loop(int fd, TZ_TEE_SPACE *comm) {
	int sock, maxsock;
	fd_set socks;
	fd_set readsocks;
	struct sockaddr_un local, remote;
	int len;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (sock < 0) {
		perror("Failed to create unix socket");
		return;
	}

	local.sun_family = AF_UNIX; /* local is declared before socket() ^ */
	strcpy(local.sun_path, SOCKET_NAME);
	unlink(local.sun_path);

	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(sock, (struct sockaddr *) &local, len) < 0) {
		perror("Failed to create unix socket");
		goto cleanup;
	}

	if (listen(sock, BACKLOG) < 0) {
		perror("Failed to create unix socket");
		goto cleanup;
	}

	/* Set up the fd_set */
	FD_ZERO(&socks);
	FD_SET(sock, &socks);
	maxsock = sock;

	/* Main loop */
	while (1) {
		int s;
		readsocks = socks;
		if (select(maxsock + 1, &readsocks, NULL, NULL, NULL ) == -1) {
			perror("select");
			goto cleanup;
		}
		for (s = 0; s <= maxsock; s++) {
			if (FD_ISSET(s, &readsocks)) {
				printf("socket %d was ready\n", s);
				if (s == sock) {
					/* New connection */
					int newsock;
					len = sizeof(struct sockaddr_un);
					newsock = accept(s, (struct sockaddr*)&remote, &len);
					if (newsock == -1) {
						perror("accept");
						continue;
					} else {
						FD_SET(newsock, &socks);
						if (newsock > maxsock) {
							maxsock = newsock;
						}
					}
				} else {
					/* Handle read or disconnection */
					handle(s, &socks);
				}
			}
		}
	}
	cleanup:

	if (sock > 0) {
		close(sock);
	}
}

int main(int argc, char** argv) {
	int fd_tz;
	int err;
	TZ_TEE_SPACE *comm;

	printf("################## TZ TEE DAEMON ##################\n");

	printf("opening TZ tee device:\n");

	fd_tz = openTZ();

	if (fd_tz < 0) {
		printf("FAILED TO open device: %s\n", strerror(errno));
		goto cleanup;
	}

	printf("device is open ...\n");
	fflush(stdout);
	fflush(stderr);

	comm = (TZ_TEE_SPACE *) mmap(NULL, sizeof(TZ_TEE_SPACE) + 0x1000,
			PROT_READ | PROT_WRITE, MAP_SHARED, fd_tz, 0);
	uint32_t orig = (uint32_t)comm;
	if (comm == NULL || comm == MAP_FAILED ) {
		printf("FAILED TO map tee memory: %s\n", strerror(errno));
		goto cleanup;
	}

	printf("TEE memory is mapped @ 0x%08x\n", (uint32_t)comm);

	uint32_t poff = ioctl(fd_tz, ANDIX_IOGMOFF);

	printf("TEE memory page offset 0x%08x\n", poff);

	comm = (TZ_TEE_SPACE*) (((uint32_t) comm) | poff);

	printf("TEE memory is located @ 0x%08x\n", (uint32_t)comm);

	comm->op = TZ_TEE_OP_INIT_CTX;
	comm->params.initCtx.context = 0;

	printf("CALLING TZ INIT CTX!\n");

	ioctl(fd_tz, ANDIX_IOCTEEZ);

	printf("RESULT: 0x%x\n", comm->ret);
	printf("CONTEXTID: 0x%x\n", comm->params.initCtx.context);

	uint32_t ctx = comm->params.initCtx.context;

	if (comm->ret == 0) {
		void* test = malloc(0x2000);

		memset(test, 0xED, 0x2000);

		mlock(test, 0x2000);

		printf("REGISTER MEM @ 0x%x\n", (uint32_t)test);

		comm->op = TZ_TEE_OP_REGISTER_MEM;
		comm->params.regMem.context = ctx;
		comm->params.regMem.memid = 0;
		comm->params.regMem.size = 0x2000;
		comm->params.regMem.paddr = (uint32_t)test;
		comm->params.regMem.flags = 0;

		printf("CALLING TZ REG MEM!\n");

		ioctl(fd_tz, ANDIX_IOCTEEZ);

		printf("RESULT: 0x%x\n", comm->ret);
		printf("MEMID: 0x%x\n", comm->params.regMem.memid);

		printf("RELEASE MEM @ 0x%x\n", (uint32_t)test);

		comm->op = TZ_TEE_OP_RELEASE_MEM;
		comm->params.regMem.context = ctx;

		printf("CALLING TZ REL MEM!\n");

		ioctl(fd_tz, ANDIX_IOCTEEZ);

		printf("RESULT: 0x%x\n", comm->ret);
		printf("MEMID: 0x%x\n", comm->params.regMem.memid);

		comm->op = TZ_TEE_OP_FIN_CTX;
		comm->params.finCtx.context = comm->params.initCtx.context;

		printf("CALLING TZ FIN CTX!\n");

		ioctl(fd_tz, ANDIX_IOCTEEZ);

		printf("RESULT: 0x%x\n", comm->ret);
		printf("CONTEXTID: 0x%x\n", comm->params.finCtx.context);
	}

	//dispatcher_loop(fd_tz, comm);

	printf("################## TZ TEE DAEMON ##################\n");

	cleanup:

	if (comm != NULL && comm != MAP_FAILED ) {
		munmap(comm, sizeof(TZ_TEE_SPACE));
	}

	if (fd_tz > 0) {
		printf("closing TZ tee device\n");
		close(fd_tz);
	}

	return (0);
}
