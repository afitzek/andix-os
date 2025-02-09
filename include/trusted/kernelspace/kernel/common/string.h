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
 * string.h
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef STRING_H_
#define STRING_H_

#include <common/typedefs.h>

/**
 * Map va_start to __builtin
 */
#define va_start(argptr, last_fixed_arg) __builtin_va_start(argptr, last_fixed_arg)

/**
 * Map va_arg to __builtin
 */
#define va_arg(argptr, type) __builtin_va_arg(argptr, type)

/**
 * Map va_copy to __builtin
 */
#define va_copy(dest, src) __builtin_va_copy(dest, src)

/**
 * Map va_end to __builtin
 */
#define va_end(argptr) __builtin_va_end(argptr)

/**
 * define va_list
 */
typedef __builtin_va_list va_list;

/**
 * Is character a space
 *
 * @param c The character to check
 * @return true if c is a space
 */
int isspace(int c);

/**
 * Get length of string
 *
 * @param str The string to check
 * @return the string length
 */
size_t strlen(const char* str);

/**
 * Locate first occurrence of character in string
 *
 * Returns a pointer to the first occurrence of character in the C string str.
 *
 * The terminating null-character is considered part of the C string.
 * Therefore, it can also be located in order to retrieve a pointer to the end of a string.
 *
 * @param str the string
 * @param c the character
 * @return A pointer to the first occurrence of character in str. If the character is not found, the function returns a null pointer.
 */
char * strchr(const char *str, int c);

/**
 * Copy characters from string
 *
 * Copies the first num characters of source to destination.
 * If the end of the source C string (which is signaled by a null-character) is found before num characters have been copied,
 * destination is padded with zeros until a total of num characters have been written to it.
 *
 * No null-character is implicitly appended at the end of destination if source is longer than num (thus, in this case,
 * destination may not be a null terminated C string).
 *
 * destination and source shall not overlap (see memmove for a safer alternative when overlapping).
 *
 * @param destination Pointer to the destination array where the content is to be copied.
 * @param source C string to be copied.
 * @param num Maximum number of characters to be copied from source.
 * @return destination is returned.
 */
char * strncpy(char * destination, const char * source, size_t num);

/**
 * Compare characters of two strings
 *
 * Compares up to num characters of the C string str1 to those of the C string str2.
 * This function starts comparing the first character of each string. If they are equal to each other,
 * it continues with the following pairs until the characters differ, until a terminating null-character
 * is reached, or until num characters match in both strings, whichever happens first.
 *
 * @param str1 C string to be compared.
 * @param str2 C string to be compared.
 * @param num Maximum number of characters to compare.
 * @return Returns an integral value indicating the relationship between the strings:
 *  zero value indicates that the characters compared in both strings form the same string.
 *  A value greater than zero indicates that the first character that does not match has a greater value in str1 than in str2;
 *  And a value less than zero indicates the opposite.
 */
int strncmp(const char * str1, const char * str2, size_t num);

/**
 * Copy block of memory
 *
 * Copies the values of num bytes from the location pointed by source directly to the memory block pointed by destination.
 * The underlying type of the objects pointed by both the source and destination pointers are irrelevant for this function; The result is a binary copy of the data.
 * The function does not check for any terminating null character in source - it always copies exactly num bytes.
 * To avoid overflows, the size of the arrays pointed by both the destination and source parameters, shall be at least num bytes, and should not overlap (for overlapping memory blocks, memmove is a safer approach).
 *
 * @param dest Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
 * @param src Pointer to the source of data to be copied, type-casted to a pointer of type const void*.
 * @param count Number of bytes to copy.
 */
void* memcpy(void* dest, const void* src, size_t count);

/**
 * Check if character is uppercase letter
 *
 * Checks if parameter c is an uppercase alphabetic letter.
 * Notice that what is considered a letter may depend on the locale being used;
 * In the default "C" locale, an uppercase letter is any of: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z.
 *
 * @param c The character to check
 * @return
 */
int isupper(int c);

/**
 * Convert uppercase letter to lowercase
 *
 * Converts c to its lowercase equivalent if c is an uppercase letter and has a lowercase equivalent.
 * If no such conversion is possible, the value returned is c unchanged.
 *
 * Notice that what is considered a letter may depend on the locale being used;
 * In the default "C" locale, an uppercase letter is any of: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z,
 * which translate respectively to: a b c d e f g h i j k l m n o p q r s t u v w x y z.
 *
 * @param c The character
 * @return
 */
int tolower(int c);

/**
 * Check if character is alphabetic
 *
 * Checks whether c is an alphabetic letter.
 *
 * @param c The character
 * @return
 */
int isalpha(int c);

/**
 * Check if character is a hex digit
 *
 * @param c The character
 */
int isxdigit(int c);

/**
 * Check if character is a digit
 *
 * @param c The character
 */
int isdigit(int c);

/**
 * Compare strings case insensitive!
 *
 * @param str1 The first string
 * @param str2 The second string
 * @param num The number of chars to compare
 * @return 0 if match
 */
int strncasecmp(const char * str1, const char * str2, size_t num);

/**
 * Compare memory regions
 * @param m1 The memory buffer 1
 * @param m2 The memory buffer 2
 * @param n number of bytes to compare
 * @return 0 if match
 */
int memcmp(const void* m1, const void* m2, size_t n);

char *strncat(char *s1, const char *s2, size_t n);

char * strstr(const char *searchee, const char *lookfor);

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#endif /* STRING_H_ */
