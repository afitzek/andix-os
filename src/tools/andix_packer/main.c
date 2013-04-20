/*
 * main.c
 *
 *  Created on: Mar 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <stdio.h>
#include <apack.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void usage() {
	printf("Usage:\n");
	printf("pack <binaryfile> <uuid_string> <name> <outfile>\n");
}

int parse_uuid(char* uuid, PACK_UUID* tee) {
	char bytes[20];
	char byte[3];
	int i = 0;

	memset(bytes, 0, 20);

	if (sscanf(uuid, "%08x-%04x-%04x-%04x-%s", &tee->timeLow, &tee->timeMid,
			&tee->timeHiAndVersion, &tee->clockSeqAndNode[6], bytes) != 5) {
		return (-1);
	}

	printf("left: %s\n", bytes);
	_pack_uint8_t d7 = tee->clockSeqAndNode[7];
	_pack_uint8_t d6 = tee->clockSeqAndNode[6];

	for (i = 6; i > 0; i--) {
		memset(byte, 0, 3);
		memcpy(byte, &bytes[(i - 1) * 2], 2);
		printf("scanning: %s\n", byte);
		if (sscanf(byte, "%02x", &tee->clockSeqAndNode[6 - i]) != 1) {
			return (-1);
		}
	}

	tee->clockSeqAndNode[7] = d7;
	tee->clockSeqAndNode[6] = d6;

	return (0);
}

void print_uuid(PACK_UUID* tee) {
	printf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", tee->timeLow,
			tee->timeMid, tee->timeHiAndVersion, tee->clockSeqAndNode[7],
			tee->clockSeqAndNode[6], tee->clockSeqAndNode[5],
			tee->clockSeqAndNode[4], tee->clockSeqAndNode[3],
			tee->clockSeqAndNode[2], tee->clockSeqAndNode[1],
			tee->clockSeqAndNode[0]);
}

int main(int argc, char** argv) {
	PACK_UUID tee;
	andixpack pack;
	int fd = 0;
	struct stat fst;
	_pack_uint8_t* buffer = NULL;

	if (argc != 5) {
		usage();
		return (0);
	}

	if (parse_uuid(argv[2], &tee) != 0) {
		printf("UUID invalid!\n");
		return (0);
	}

	print_uuid(&tee);

	if (strlen(argv[3]) > 64) {
		printf("Name to long!\n");
		return (0);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd <= 0) {
		perror("Failed to open file!");
		return (0);
	}

	if (fstat(fd, &fst) != 0) {
		perror("Failed to stat file!");
		close(fd);
		return (0);
	}

	buffer = (_pack_uint8_t*) malloc(fst.st_size);

	if (buffer == NULL ) {
		perror("Out of memory!");
		close(fd);
		return (0);
	}

	int totalread = 0;
	int curread = 0;

	while (totalread < fst.st_size) {
		curread = read(fd, buffer + totalread, fst.st_size - totalread);

		if (curread <= 0) {
			perror("Failed to read file");
			free(buffer);
			close(fd);
			return (0);
		} else {
			totalread += curread;
		}
	}

	close(fd);

	memset(pack.name, 0, 64);
	strncpy(pack.name, argv[3], 64);
	memcpy(&pack.uuid, &tee, sizeof(PACK_UUID));
	pack.datalen = fst.st_size;

	fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

	if (fd <= 0) {
		perror("Failed to open output file");
		free(buffer);
		return (0);
	}

	totalread = 0;
	curread = 0;

	while (totalread < sizeof(andixpack)) {
		curread = write(fd, ((_pack_uint8_t*)&pack) + totalread,
				sizeof(andixpack) - totalread);

		if (curread <= 0) {
			perror("Failed to write file");
			free(buffer);
			close(fd);
			return (0);
		} else {
			totalread += curread;
		}
	}

	totalread = 0;
	curread = 0;

	while (totalread < fst.st_size) {
		curread = write(fd, buffer + totalread, fst.st_size - totalread);

		if (curread <= 0) {
			perror("Failed to write file");
			free(buffer);
			close(fd);
			return (0);
		} else {
			totalread += curread;
		}
	}

	close(fd);
	free(buffer);

	return (0);
}
