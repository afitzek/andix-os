/*
 * entropy.c
 *
 *  Created on: May 24, 2013
 *      Author: andy
 */

#include <stdio.h>
#include <unistd.h>
#include <polarssl/entropy.h>
#include <fcntl.h>

int platform_pseudo_entropy( void *data,
                           unsigned char *output, size_t len, size_t *olen ) {
	size_t ret;
	int fd = open("/dev/urandom", O_RDONLY);

	(*olen) = 0;

	if(fd <= 0)
		return (POLARSSL_ERR_ENTROPY_SOURCE_FAILED);

	ret  = read(fd, output, len);

	if(ret != len) {
		close(fd);
		return (POLARSSL_ERR_ENTROPY_SOURCE_FAILED);
	}

	close(fd);

	(*olen) = len;

	return (0);
}
