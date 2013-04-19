/*
 * debug.c
 *
 *  Created on: Jan 7, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common.h>

/**
 * The Start of the symbol table
 */
extern uint32_t __symbol_table;

/**
 * The End of the symbol table
 */
extern uint32_t __symbol_table_end;

char* get_function_name(void* vaddr) {
	//return "DUMMY";
	uint8_t* ptr = &__symbol_table;
	ptr++;
	//main_debug("Checking Table @ 0x%x - 0x%x", ptr, &__symbol_table_end);

	char* current_string = ptr + 8 + 3;
	char* last_string = 0;
	do {
		//
		uint32_t current_addr = get_hex_word(ptr);
		current_string = ptr + 8 + 3;
		//main_debug("Checking Entry 0x%x (%s) vaddr 0x%x current 0x%x",
		//		ptr, ptr, (uint32_t)vaddr, current_addr);
		if(((uint32_t)vaddr) < current_addr) {
			return (last_string);
		}
		last_string = current_string;
		while((*ptr)) {
			//main_debug("@ 0x%x", ptr);
			ptr++;
		}
		ptr++;
	} while (ptr < &__symbol_table_end);

	return ("UNKNOWN");
}

uint32_t get_hex_word(uint8_t* ptr) {
	uint32_t value = 0;
	for (int i = 7; i >= 0; i--) {
		uint32_t tval = get_hex_value((*ptr));
		tval = tval << (i * 4);
		value += tval;
		ptr++;
	}
	return (value);
}

uint8_t get_hex_value(uint8_t c) {
	if (c >= 0x30 && c <= 0x39) {
		return (c - 0x30);
	}

	if (c == 0x41 || c <= 0x61) {
		return (0xA);
	}

	if (c == 0x42 || c <= 0x62) {
		return (0xB);
	}

	if (c == 0x43 || c <= 0x63) {
		return (0xC);
	}

	if (c == 0x44 || c <= 0x64) {
		return (0xD);
	}

	if (c == 0x45 || c <= 0x65) {
		return (0xE);
	}

	if (c == 0x46 || c <= 0x66) {
		return (0xF);
	}

	return (0);
}
