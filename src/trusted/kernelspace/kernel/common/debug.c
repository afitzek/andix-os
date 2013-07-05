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
	uint8_t* ptr = (uint8_t*)&__symbol_table;
	ptr++;
	//main_debug("Checking Table @ 0x%x - 0x%x", ptr, &__symbol_table_end);

	char* current_string = (char*)(ptr + 8 + 3);
	char* last_string = 0;
	do {
		//
		uint32_t current_addr = get_hex_word(ptr);
		current_string = (char*)(ptr + 8 + 3);
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
	} while ((uint32_t*)ptr < &__symbol_table_end);

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
