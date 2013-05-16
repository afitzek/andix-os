/*
 * utils.c
 *
 *  Created on: May 14, 2013
 *      Author: andy
 */

#include <tee_client_api.h>
#include <tee_utils.h>
#include <stdlib.h>

unsigned char strtouc(const char *nptr, char **endptr, int base) {
	const unsigned char *s = (const unsigned char *) nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long) 0xFF / (unsigned long) base;
	cutlim = (unsigned long) 0xFF % (unsigned long) base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = 0xFF;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *) s - 1 : nptr);
	return (acc);
}

unsigned short strtous(const char *nptr, char **endptr, int base) {
	const unsigned char *s = (const unsigned char *) nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long) 0xFFFF / (unsigned long) base;
	cutlim = (unsigned long) 0xFFFF % (unsigned long) base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = 0xFFFF;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *) s - 1 : nptr);
	return (acc);
}

int uuid_parse(const char *in, TEEC_UUID* uuid)
{
	char		uuid_string[40];
	int 		i;
	const char	*cp;
	char		buf[3];

	if (strlen(in) != 36)
		return (-1);
	for (i=0, cp = in; i <= 36; i++,cp++) {
		if ((i == 8) || (i == 13) || (i == 18) ||
		    (i == 23)) {
			if (*cp == '-')
				continue;
			else
				return (-1);
		}
		if (i== 36)
			if (*cp == 0)
				continue;
		if (!isxdigit(*cp))
			return (-1);
	}

	memcpy(uuid_string, in, strlen(in));

	uuid_string[8] = 0;
	uuid_string[13] = 0;
	uuid_string[18] = 0;
	uuid_string[23] = 0;

	uuid->timeLow = strtoul(in, NULL, 16);
	uuid->timeMid = strtous(in+9, NULL, 16);
	uuid->timeHiAndVersion = strtous(in+14, NULL, 16);
	//uuid->clockSeqAndNode[0] = strtous(in+19, NULL, 16);
	cp = in+19;
	buf[2] = 0;
	for (i=0; i < 8; i++) {
		buf[0] = *cp++;
		buf[1] = *cp++;
		if((*cp) == '-') {
			cp++;
		}
		uuid->clockSeqAndNode[7-i] = strtouc(buf, NULL, 16);
	}

	return (0);
}
