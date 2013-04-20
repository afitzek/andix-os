/*
 * blockdev.c
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <andixtz.h>
#include <blockdev.h>
#include <stdlib.h>
#include <stdio.h>
#include <communication_types.h>
#include <string.h>

long int get_file_size(FILE* file) {
	long int curpos = ftell(file);
	long int size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, curpos, SEEK_SET);
	return (size);
}

void dump_memn(uint8_t* data, uint32_t len) {
	int i = 0;
	int j = 0;

	for (i = 0; i < len; i = i + 16) {
		printf("%04X: ", i);
		for (j = 0; j < 16; j++) {
			int idx = i * 16 + j;
			if (idx < len) {
				printf("%02X ", data[idx]);
			} else {
				printf("   ");
			}
		}

		for (j = 0; j < 16; j++) {
			int idx = i * 16 + j;
			if (idx < len) {
				if (data[idx] >= 0x20 && data[idx] <= 0x7E) {
					printf("%c", (char) data[idx]);
				} else {
					printf(".");
				}
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

char* blkdev_get_filename(TZ_CTLR_SPACE* space, char* fname, uint32_t nlen) {
	char tstr[10];
	int cx = 0;
	int i = 0;

	strncat(fname, TZ_ROOT, nlen);

	for (i = 0; i < sizeof(space->params.bopen.filename); i++) {
		memset(tstr, '\0', 10);
		snprintf(tstr, 10, "%02X", space->params.bopen.filename[i]);
		strncat(fname, tstr, nlen);
	}
	strncat(fname, TZ_FEND, nlen);
	return (fname);
}

void create_tz_file(TZ_CTLR_SPACE* space) {
	char* filename[100];
	FILE* file;
	memset(filename, '\0', 100);
	blkdev_get_filename(space, (char*)filename, 100);
	printf("Creating file: %s\n", (char*)filename);

	file = fopen((char*)filename, "w+");
	if (file == NULL ) {
		space->ret = 1;
	} else {
		fclose(file);
		space->ret = 0;
	}
	space->op = TZ_CTRL_OP_BCREATE_RET;
}

void delete_tz_file(TZ_CTLR_SPACE* space) {
	char* filename[100];
	FILE* file;
	memset(filename, '\0', 100);
	blkdev_get_filename(space, (char*)filename, 100);
	printf("Deleting file: %s\n", (char*)filename);

	remove((char*)filename);
}

void exists_tz_file(TZ_CTLR_SPACE* space) {
	char* filename[100];
	FILE* file;
	memset(filename, '\0', 100);
	blkdev_get_filename(space, (char*)filename, 100);
	printf("Exists file: %s\n", (char*)filename);

	file = fopen((char*)filename, "r");
	if (file == NULL ) {
		space->ret = 0; // not existing
	} else {
		fclose(file);
		space->ret = 1; // exists
	}
	space->op = TZ_CTRL_OP_BFEXISTS_RET;
}

void open_tz_file(TZ_CTLR_SPACE* space) {
	char* filename[100];
	FILE* file;
	memset(filename, '\0', 100);
	blkdev_get_filename(space, (char*)filename, 100);
	printf("Opening file: %s\n", (char*)filename);
	file = fopen((char*)filename, "r+");
	printf("FILE POINTER: 0x%x\n", (uint32_t)file);
	if (file == NULL ) {
		space->params.bopen.nsfd = 0;
		space->ret = 1;
	} else {
		fseek(file, 0, SEEK_SET);
		space->params.bopen.nsfd = (void*) file;
		space->ret = 0;
	}
	space->op = TZ_CTRL_OP_BOPEN_RET;
}

void read_tz_file_block(TZ_CTLR_SPACE* space) {
	space->op = TZ_CTRL_OP_BREAD_RET;
	printf("READ BLK from file: 0x%x\n", (uint32_t)space->params.bwrite.nsfd);
	if (space->params.bwrite.nsfd != NULL ) {

		FILE* file = (FILE*) space->params.bwrite.nsfd;
		long int file_offset = (space->params.bwrite.blockpos * TZ_BLOCK_SIZE);
		printf("Reading BLK: %d (%d)\n", space->params.bwrite.blockpos,
				(int32_t)file_offset);
		long int fsize = get_file_size(file);
		if (ftell(file) != file_offset) {
			// move to file position

			if (fseek(file, file_offset, SEEK_SET) != 0) {
				perror("Failed to fseek\n");

				if (fsize < file_offset) {
					// TODO: This could mean we can inject 0 into tz??
					// FILL with zero blocks
					printf("Readop needs filling ...\n");
					long int diff = file_offset - fsize;
					fseek(file, 0, SEEK_END);
					unsigned char m = 0;
					fwrite(&m, 1, diff, file);
				}

				if (ftell(file) != file_offset) {
					printf("Failed to fill empty blocks to correct size\n");
					goto error;
				}
			}
		}

		fsize = get_file_size(file);

		if (fsize == ftell(file)) {
			// reading block for writing ....
			printf("Readop empty block ...\n");
			memset(space->params.bwrite.buffer, 0, 1024);
			fwrite(space->params.bwrite.buffer, 1024, 1, file);
		} else {
			printf("Reading normal block ...\n");
			if (fread(space->params.bwrite.buffer, 1, 1024, file) != 1024) {
				goto error;
			}
		}

		//printf("Read Block:\n");
		//dump_memn(space->params.bwrite.buffer, TZ_BLOCK_SIZE);

		fflush(file);

		space->ret = 0;
		return;
	}
	error: memset(space->params.bwrite.buffer, 0, 1024);
	space->ret = 1;
}

void write_tz_file_block(TZ_CTLR_SPACE* space) {
	space->op = TZ_CTRL_OP_BWRITE_RET;
	printf("WRITE BLK to file: 0x%x\n", (uint32_t)space->params.bwrite.nsfd);
	if (space->params.bwrite.nsfd != NULL ) {

		FILE* file = (FILE*) space->params.bwrite.nsfd;
		printf("Set File descriptor\n");

		long int file_offset = (space->params.bwrite.blockpos * TZ_BLOCK_SIZE);
		printf("Writing BLK: %d (%d)\n", space->params.bwrite.blockpos,
				(int32_t)file_offset);
		if (ftell(file) != file_offset) {
			// move to file position

			if (fseek(file, file_offset, SEEK_SET) != 0) {
				perror("Failed to fseek\n");

				long int fsize = get_file_size(file);

				if (fsize < file_offset) {
					// TODO: This could mean we can inject 0 into tz??
					// FILL with zero blocks
					printf("Writeop needs filling ...\n");
					long int diff = file_offset - fsize;
					fseek(file, 0, SEEK_END);
					unsigned char m = 0;
					fwrite(&m, 1, diff, file);
				}

				if (ftell(file) != file_offset) {
					printf("Failed to fill empty blocks to correct size\n");
					goto error;
				}
			}
		}

		printf("File position set %d!\n", (int32_t)ftell(file));

		//printf("Writing Block:\n");
		//dump_memn(space->params.bwrite.buffer, TZ_BLOCK_SIZE);

		if (fwrite(space->params.bwrite.buffer, 1, TZ_BLOCK_SIZE,
				file) != TZ_BLOCK_SIZE) {
			printf("Failed to write buffer\n");
			goto error;
		}

		printf("Flush file\n");
		while (fflush(file) != 0) {

		}

		space->ret = 0;
		return;
	}
	error:
	//memset(space->params.bwrite.buffer, 0, 1024);
	space->ret = 1;
}

void count_blks_tz_file(TZ_CTLR_SPACE* space) {
	// TODO: Programm this call to count the blocks ...
	space->op = TZ_CTRL_OP_BCLOSE_RET;
	printf("COUNT BLK to file: 0x%x\n", (uint32_t)space->params.bopen.nsfd);
	if (space->params.bopen.nsfd != NULL ) {
		FILE* file = (FILE*) space->params.bopen.nsfd;
		fclose(file);
	}
	space->ret = 0;
}

void close_tz_file(TZ_CTLR_SPACE* space) {
	space->op = TZ_CTRL_OP_BCLOSE_RET;
	printf("CLOSE BLK to file: 0x%x\n", (uint32_t)space->params.bopen.nsfd);
	if (space->params.bopen.nsfd != NULL ) {
		FILE* file = (FILE*) space->params.bopen.nsfd;
		fclose(file);
	}
	space->ret = 0;
}
