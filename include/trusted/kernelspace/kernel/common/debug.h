/*
 * debug.h
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <common/typedefs.h>

/**
 * Get name of function where instruction vaddr is in
 * @param vaddr Address of some instruction in the function
 * @return the function name
 */
char* get_function_name(void* vaddr);

/**
 * Parse a character from hex to integer value
 * @param c The character
 * @return the numeric value
 */
uint8_t get_hex_value(uint8_t c);

/**
 * Parse exactly 8 character hex string to its numerical value!
 * @param ptr To 8 character string
 * @return the numeric value
 */
uint32_t get_hex_word(uint8_t* ptr);

#endif /* DEBUG_H_ */
