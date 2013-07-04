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
