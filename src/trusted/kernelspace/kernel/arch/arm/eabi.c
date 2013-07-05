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
/*typedef unsigned int size_t;

 void* memset(void *dest, size_t n, int c) {
 char* s = dest;
 while (n--)
 *s++ = (char) c;
 return dest;
 }

 void* memcpy(void *dest, const void *src, size_t n) {
 char* d = dest;
 char* s = src;
 while (n--)
 *d++ = (char) *s++;
 return dest;
 }

 void* memmove(void *dest_void, const void *src_void, size_t n) {
 char *dst = dest_void;
 const char *src = src_void;

 if (src < dst && dst < src + n) {
 src += n;
 dst += n;
 while (n--) {
 *--dst = *--src;
 }
 } else {
 while (n--) {
 *dst++ = *src++;
 }
 }

 return dest_void;
 }*/
/*
#include <common.h>

void __aeabi_memcpy8(void *dest, const void *src, size_t n) {
	memcpy(dest, src, n);
}

void __aeabi_memcpy4(void *dest, const void *src, size_t n) {
	memcpy(dest, src, n);
}

void __aeabi_memcpy(void *dest, const void *src, size_t n) {
	memcpy(dest, src, n);
}

void __aeabi_memmove8(void *dest, const void *src, size_t n) {
	memmove(dest, src, n);
}

void __aeabi_memmove4(void *dest, const void *src, size_t n) {
	memmove(dest, src, n);
}

void __aeabi_memmove(void *dest, const void *src, size_t n) {
	memmove(dest, src, n);
}

void __aeabi_memset8(void *dest, size_t n, int c) {
	memset(dest, c, n);
}

void __aeabi_memset4(void *dest, size_t n, int c) {
	memset(dest, c, n);
}

void __aeabi_memset(void *dest, size_t n, int c) {
	memset(dest, c, n);
}*/
/*
void __aeabi_memclr8(void *dest, size_t n) {
	//__aeabi_memset8(dest, n, 0);
}

void __aeabi_memclr4(void *dest, size_t n) {
	//__aeabi_memset4(dest, n, 0);
}

void __aeabi_memclr(void *dest, size_t n) {
	//__aeabi_memset(dest, n, 0);
}*/
