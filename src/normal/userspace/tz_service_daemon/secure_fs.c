/*
 * secure_fs.c
 *
 *  Created on: Mar 17, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <communication_types.h>
#include <andixtz.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

char* get_fs_string(REE_FS_NAME* fs_name, char* string, int maxlen) {
	int i = 0;
	char* str = string;
	char tstr[10];
	int size_left = maxlen;
	memset(str, 0, maxlen);

	strncat(str, TZ_ROOT, size_left);

	size_left -= strlen(TZ_ROOT);

	for (i = 0; i < TZ_FS_HASH_SIZE; i++) {
		memset(tstr, '\0', 10);
		snprintf(tstr, 10, "%02X", fs_name->dirname[i]);
		strncat(str, tstr, size_left);
		size_left -= strlen(tstr);
	}

	strncat(str, TZ_SEPERATOR, size_left);

	size_left -= strlen(TZ_SEPERATOR);

	for (i = 0; i < TZ_FS_HASH_SIZE; i++) {
		memset(tstr, '\0', 10);
		snprintf(tstr, 10, "%02X", fs_name->filename[i]);
		strncat(str, tstr, size_left);
		size_left -= strlen(tstr);
	}

	strncat(str, TZ_FEND, size_left);

	return (str);
}

char* get_fs_pathstring(REE_FS_NAME* fs_name, char* string, int maxlen) {
	int i = 0;
	char* str = string;
	char tstr[10];
	int size_left = maxlen;
	memset(str, 0, maxlen);

	strncat(str, TZ_ROOT, size_left);

	size_left -= strlen(TZ_ROOT);

	for (i = 0; i < TZ_FS_HASH_SIZE; i++) {
		memset(tstr, '\0', 10);
		snprintf(tstr, 10, "%02X", fs_name->dirname[i]);
		strncat(str, tstr, size_left);
		size_left -= strlen(tstr);
	}

	return (str);
}

void tz_fs_remove(TZ_CTLR_SPACE* space) {
	char filename[150];
	get_fs_string(&(space->params.ree_remove.pathname), filename,
			sizeof(filename));

	printf("tz_fs_remove -> removing file: %s\n", filename);

	space->ret = remove(filename);
	printf("tz_fs_remove -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_remove");
	}
}

void tz_fs_open(TZ_CTLR_SPACE* space) {
	char filename[150];
	char pathname[150];
	get_fs_string(&(space->params.ree_open.pathname), filename,
			sizeof(filename));

	get_fs_pathstring(&(space->params.ree_open.pathname), pathname,
			sizeof(pathname));

	printf("tz_fs_open -> opening file: %s\n", filename);

	space->ret = open(filename, O_RDWR | O_CREAT, S_IRWXU);

	if ((int) space->ret < 0) {
		int lerror = errno;
		printf("tz_fs_open -> failed\n");
		if (lerror == ENOENT) {
			printf("tz_fs_open -> trying to create directory\n");
			if (mkdir(pathname, S_IRWXU) == 0) {
				printf("tz_fs_open -> directory created!\n");
				space->ret = open(filename, O_RDWR | O_CREAT, S_IRWXU);

				if ((int) space->ret < 0) {
					printf("ERRNO: %d\n", errno);
					perror("tz_fs_open");
				}

			} else {
				printf("ERRNO: %d\n", errno);
				perror("tz_fs_open mkdir");
			}
		} else {
			printf("ERRNO: %d\n", errno);
			perror("tz_fs_open");
		}
	}

	printf("tz_fs_open -> result %d\n", space->ret);
}

void tz_fs_close(TZ_CTLR_SPACE* space) {
	printf("tz_fs_close -> closing file: %d\n", space->params.ree_close.fd);

	space->ret = close(space->params.ree_close.fd);

	printf("tz_fs_close -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_close");
	}
}

void tz_fs_read(TZ_CTLR_SPACE* space) {
	ssize_t readbytes = 0;
	ssize_t totalread = 0;
	size_t left = space->params.ree_read.count;
	uint8_t* buffer = (uint8_t*) space->params.ree_read.buffer;

	printf("tz_fs_read -> reading file: %d\n", space->params.ree_read.fd);

	while (totalread < space->params.ree_read.count) {
		readbytes = read(space->params.ree_read.fd, buffer, left);

		printf("read: %d\n", readbytes);

		if (readbytes < 0) {
			space->ret = readbytes;
			perror("tz_fs_read");
			break;
		} else if (readbytes == 0) {
			totalread += readbytes;
			if (totalread < space->params.ree_read.count) {
				space->ret = 0;
				return;
			}
			break;
		} else {
			totalread += readbytes;
			left -= readbytes;
			buffer += readbytes;
		}
	}

	space->ret = totalread;

	printf("tz_fs_read -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_read");
	}
}

void tz_fs_write(TZ_CTLR_SPACE* space) {
	ssize_t readbytes = 0;
	ssize_t totalread = 0;
	size_t left = space->params.ree_write.count;
	uint8_t* buffer = (uint8_t*) space->params.ree_write.buffer;

	printf("tz_fs_write -> writing file: %d\n", space->params.ree_write.fd);

	while (totalread < space->params.ree_write.count) {
		readbytes = write(space->params.ree_write.fd, buffer, left);
		printf("tz_fs_write -> written %d bytes / %d\n", readbytes,
				space->params.ree_write.count);
		if (readbytes < 0) {
			space->ret = readbytes;
			perror("tz_fs_write");
			break;
		} else if (readbytes == 0) {
			totalread += readbytes;
			if (totalread < space->params.ree_write.count) {
				space->ret = 0;
				return;
			}
			break;
		} else {
			totalread += readbytes;
			left -= readbytes;
			buffer += readbytes;
		}
	}

	space->ret = totalread;

	printf("tz_fs_write -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_write");
	}
}

void tz_fs_fstat(TZ_CTLR_SPACE* space) {
	struct stat sstat;
	printf("tz_fs_fstat -> stat file: %d\n", space->params.ree_fstat.fd);

	space->ret = fstat(space->params.ree_fstat.fd, &sstat);
	space->params.ree_fstat.stat.st_size = sstat.st_size;
	printf("tz_fs_fstat -> size %d\n", space->params.ree_fstat.stat.st_size);
	printf("tz_fs_fstat -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_fstat");
	}
}

void tz_fs_lseek(TZ_CTLR_SPACE* space) {
	int whence = SEEK_CUR;
	printf("tz_fs_lseek -> stat file: %d\n", space->params.ree_lseek.fd);

	space->ret = 0;

	if (space->params.ree_lseek.whence == REE_SEEK_CUR) {
		whence = SEEK_CUR;
	} else if (space->params.ree_lseek.whence == REE_SEEK_SET) {
		whence = SEEK_SET;
	} else {
		space->ret = -1;
	}

	if (space->ret == 0) {
		space->ret = lseek(space->params.ree_lseek.fd,
				space->params.ree_lseek.offset, whence);
	}

	printf("tz_fs_lseek -> result %d\n", space->ret);

	if ((int) space->ret < 0) {
		perror("tz_fs_lseek");
	}
}
