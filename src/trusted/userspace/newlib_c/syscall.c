/*
 * syscall.c
 *
 *  Created on: Jan 2, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <sys/stat.h>
#include <swi.h>
#include <string.h>

int _isatty(int file) {
	if(file == 0 || file == 1 || file == 2) {
		return (1);
	}
	return (0);
}

int _close(int file) {
	return (__swi_1(SWI_CLOSE, file));
}

int _lseek(int file, int ptr, int dir) {
	return (__swi_3(SWI_LSEEK, file, ptr, dir));
}

int _open(const char *name, int flags, int mode) {
	return (__swi_3(SWI_OPEN, (unsigned int) name, (unsigned int) flags,
			(unsigned int) mode));
}

int _read(int file, char *ptr, int len) {
	return (__swi_3(SWI_READ, file, (unsigned int) ptr, len));
}

int _fstat(int file, struct stat *st) {
	int ret  = __swi_1(SWI_FSTAT, file);
	if(ret == -1) {
		return (-1);
	}
	memset(st, 0, sizeof(struct stat));
	st->st_size = ret;
	return (0);
}

caddr_t _sbrk(int incr) {
	char * prev_heap_end;

	prev_heap_end = (char*)__swi_1(SWI_HEND, 0);
	if (((void*)prev_heap_end + incr) >
		((void*) &prev_heap_end)) {
		return ((caddr_t) -1);
	}

	__swi_1(SWI_SBRK, incr);

	return ((caddr_t) prev_heap_end);
}

int _write(int file, char *ptr, int len) {
	return (__swi_3(SWI_WRITE, file, (unsigned int) ptr, len));
}
