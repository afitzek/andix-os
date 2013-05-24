/*
 * stdlib.h
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#ifndef STDLIB_H_
#define STDLIB_H_

#define ULONG_MAX (0xffffffff)
#define USHORT_MAX (0xffff)
#define UBYTE_MAX (0xff)

unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned short strtous(const char *nptr, char **endptr, int base);
unsigned char strtouc(const char *nptr, char **endptr, int base);

#endif /* STDLIB_H_ */
