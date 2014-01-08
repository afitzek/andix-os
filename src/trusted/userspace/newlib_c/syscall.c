/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
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
#include <sys/time.h>

int _isatty(int file) {
	if (file == 0 || file == 1 || file == 2) {
		return (1);
	}
	return (0);
}

void _exit(int ret) {
	for(;;) ;	// TODO
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
	int ret = __swi_1(SWI_FSTAT, file);
	if (ret == -1) {
		return (-1);
	}
	memset(st, 0, sizeof(struct stat));
	st->st_size = ret;
	return (0);
}

caddr_t _sbrk(int incr) {
	char * prev_heap_end;

	prev_heap_end = (char*) __swi_1(SWI_HEND, 0);
	if (((void*) prev_heap_end + incr) > ((void*) &prev_heap_end)) {
		return ((caddr_t) -1);
	}

	__swi_1(SWI_SBRK, incr);

	return ((caddr_t) prev_heap_end);
}

int _write(int file, char *ptr, int len) {
	return (__swi_3(SWI_WRITE, file, (unsigned int) ptr, len));
}

int _kill(int pid, int sig) {
	return -1;
}

int _gettimeofday(struct timeval *tv, struct timezone *tz) {
	return -1;
}

int _getpid(void) {
	return 1;
}
