/*
 * main.c
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <andix_tz_mod.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[]) {
	int tee_fd = open(DEVICE_NAME, O_RDWR);

	if (tee_fd < 0) {
		printf("failed to open TZ tee device\n");
		return 0;
	}

	ioctl(tee_fd, ANDIX_TZ_TEST, 0);

	printf("closing TZ tee device\n");
	close(tee_fd);
	return 0;
}
