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
 * string.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/string.h>

int isspace( int c ) {
	return (c == 0x20 || c == 0x09 || c == 0x0a || c == 0x0b || c == 0x0c || c == 0x0d);
}

size_t strlen(const char* str) {
	const char* start = str;
	while (*str) {
		str++;
	}
	return ((size_t) (str - start));
}

char * strchr(const char *str, int i) {
	const uint8_t *s = (const uint8_t *) str;
	unsigned char c = i;

	while (*s && *s != c)
		s++;
	if (*s == c)
		return ((char *) s);
	return (NULL );
}

char * strncpy(char * destination, const char * source, size_t num) {
	char *dscan;
	const char *sscan;

	dscan = destination;
	sscan = source;
	while (num > 0) {
		--num;
		if ((*dscan++ = *sscan++) == '\0')
			break;
	}
	while (num-- > 0)
		*dscan++ = '\0';

	return (destination);
}

#ifndef __clang__
void* memcpy(void* dest, const void* src, size_t count) {
        uint8_t* dst8 = (uint8_t*)dest;
        uint8_t* src8 = (uint8_t*)src;

        while (count--) {
            *dst8++ = *src8++;
        }
        return (dest);
}
#endif

int strncmp(const char * str1, const char * str2, size_t num){
	if (num == 0)
	    return (0);

	  while (num-- != 0 && *str1 == *str2)
	    {
	      if (num== 0 || *str1 == '\0')
	        break;
	      str1++;
	      str2++;
	    }

	  return ((*(unsigned char *) str1) - (*(unsigned char *) str2));
}

int isupper(int c) {
	return (c >= 0x41 && c <= 0x5A);
}

int isalpha(int c) {
	return ((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A));
}


int isdigit(int c) {
	return ((c >= 0x30 && c <= 0x39));
}

int isxdigit(int c) {
	return ((c >= 0x30 && c <= 0x39) ||
			(c >= 0x41 && c <= 0x46) || (c >= 0x61 && c <= 0x66) );
}

int tolower(int c) {
  return (isupper(c) ? (c) - 'A' + 'a' : c);
}

int strncasecmp(const char * str1, const char * str2, size_t num) {
  const unsigned char *ucs1 = (const unsigned char *) str1;
  const unsigned char *ucs2 = (const unsigned char *) str2;
  int d = 0;
  for ( ; num != 0; num--)
    {
	  const int c1 = tolower(*ucs1++);
	  const int c2 = tolower(*ucs2++);
      if (((d = c1 - c2) != 0) || (c2 == '\0'))
        break;
    }
  return (d);
}

int memcmp(const void* m1, const void* m2, size_t n) {
  unsigned char *s1 = (unsigned char *) m1;
  unsigned char *s2 = (unsigned char *) m2;
  while (n--)
    {
      if (*s1 != *s2)
        {
          return (*s1 - *s2);
        }
      s1++;
      s2++;
    }
  return (0);
}

char *strncat(char *s1, const char *s2, size_t n)
{
  char *s = s1;

  while (*s1)
    s1++;
  while (n-- != 0 && (*s1++ = *s2++))
    {
      if (n == 0)
        *s1 = '\0';
    }

  return (s);
}

char * strstr(const char *searchee, const char *lookfor)
{
  /* Less code size, but quadratic performance in the worst case.  */
  if (*searchee == 0)
    {
      if (*lookfor)
        return ((char *) NULL);
      return ((char *) searchee);
    }

  while (*searchee)
    {
      size_t i;
      i = 0;

      while (1)
        {
          if (lookfor[i] == 0)
            {
              return ((char *) searchee);
            }

          if (lookfor[i] != searchee[i])
            {
              break;
            }
          i++;
        }
      searchee++;
    }

  return ((char *) NULL);
}

