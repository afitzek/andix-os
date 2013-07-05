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
 * kprintf.c
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <kprintf.h>
#include <devices/devices.h>
#include <task/task.h>
#include <hal.h>

extern uint32_t svc_stack;

platform_device_t* serial_dev = NULL;

early_io_init early_init;
early_io_putc early_putc;
early_io_getc early_getc;

void getSerial() {
	if (serial_dev == NULL ) {
		serial_dev = hal_find_device(SERIAL_DEVICE, 0);
	}
}

char* gettaskname() {
	if (get_current_task() != NULL ) {
		return (get_current_task()->name);
	}
	return ("NO-TASK");
}

void kprintHex(uint8_t* buffer, uint32_t size) {
	uint32_t i = 0;
	for (i = 0; i < size; i++) {
		if (i != 0 && (i % 16 == 0)) {
			kprintf("\n");
		}
		kprintf("%X:", buffer[i]);
	}
	kprintf("\n");
}

int getchar() {
	getSerial();
	uint8_t character = -1;
	if (serial_dev != NULL ) {
		hal_read(serial_dev, &character, 1);
		return ((int) character);
	} else {
		if (early_getc != NULL ) {
			return (early_getc());
		}
	}
	return (-1);
}

void getinput(char *buffer, uint32_t size) {
	uint32_t idx = 0;
	int currentChar = 0;
	while (idx < size) {
		currentChar = getchar();
		if (currentChar != 0xA && currentChar != 0xD) {
			buffer[idx] = (char) currentChar;
			idx++;
		} else {
			break;
		}
	}
}

void mputchar(char c) {

	//getSerial();
	//if (serial_dev != NULL ) {
	//	hal_write(serial_dev, &c, 1);
	//	return;
	//}
	if (kputchar != NULL ) {
		kputchar(c);
	} else {
		if (early_putc != NULL ) {
			early_putc(c);
		} else {
			return;
		}
	}
	/* If \n, also do \r */
	if (c == '\n')
		mputchar('\r');
}

void mputchar_ex(char c) {
	mputchar(c);
	/*
	 if (kputchar != NULL ) {
	 kputchar(c);
	 return;
	 }

	 __REG(UART_BASE + UTXD) = 0x41;

	 while (!(__REG(UART_BASE + UTS) & UTS_TXEMPTY))
	 ;

	 if (c == '\n')
	 mputchar_ex('\r');*/
}

void init_serial(uint32_t sysid) {
	serial_dev = NULL;
	kputchar = NULL;
	early_init = NULL;
	early_putc = NULL;
	early_getc = NULL;

	hal_platform_t* plat = hal_find_platform(sysid);
	if (plat != NULL ) {
		early_init = plat->__early_io_init;
		early_putc = plat->__early_io_putc;
		early_getc = plat->__early_io_getc;
	}

	if (early_init != NULL ) {
		early_init();
	}
}

void kdumpMem32(void* mem, uint32_t size) {
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t* buffer = (uint32_t*) mem;
	for (i = 0; i < size / 4; i++) {

		kprintf_ex("0x%x: ", buffer + i);
		kprintf_ex("0x%x |", buffer[i]);

		for (j = 0; j < 4; j++) {
			uint8_t* cbuffer = (uint8_t*) (buffer + i);
			if (cbuffer[j] >= 0x20 && cbuffer[j] <= 0x7E) {
				kprintf_ex("%c", cbuffer[j]);
			} else {
				kprintf_ex(".", cbuffer[j]);
			}
		}
		kprintf_ex("\n");
	}
}

void dump_bt(backtrace_t* bt, uint32_t idx) {
	//uint32_t* lv = bt->linkvalue;
	char* func_name = get_function_name((void*) bt->codeptr);
	if (func_name) {
		main_error("%d   | %s | 0x%x | 0x%x | 0x%x | 0x%x", idx, func_name,
				bt->codeptr, bt->linkvalue, bt->stackptr, bt->frameptr);
	} else {
		main_error("%d   | ----------------- | 0x%x | 0x%x | 0x%x | 0x%x", idx,
				bt->codeptr, bt->linkvalue, bt->stackptr, bt->frameptr);
	}
	//main_debug("CODE PTR: 0x%x, LINK: 0x%x, SP: 0x%x, FP: 0x%x", bt->codeptr, bt->linkvalue, bt->stackptr, bt->frameptr);
	//if(is_valid_kernel_addr(lv)) {
	//	main_debug("LV: 0x%x", *(lv));
	//}
	/*main_debug("V1: 0x%x, V2: 0x%x, V3: 0x%x, V4: 0x%x",
	 bt->v[0], bt->v[1], bt->v[2], bt->v[3]);
	 main_debug("V5: 0x%x, V6: 0x%x, V7: 0x%x",
	 bt->v[4], bt->v[5], bt->v[6]);
	 main_debug("A1: 0x%x, A2: 0x%x, A3: 0x%x, A4: 0x%x",
	 bt->a[0], bt->a[1], bt->a[2], bt->a[3]);*/

}

void dump_stack_trace() {
	uint32_t fpAddr;
	backtrace_t* bt;
	uint32_t startstack;
	__asm__ __volatile__("MOV %0, fp": "=r" (fpAddr)::"memory");

	fpAddr = fpAddr - 0xC;

	bt = (backtrace_t*) (fpAddr);

	if (get_current_task() != NULL ) {
		startstack = ((task_t*) get_current_task())->kernelStartSP;
	} else {
		startstack = (uint32_t) &svc_stack;
	}
	main_error("=======================================");
	main_error("STACK TRACE:");
	main_error("=======================================");
	main_error("IDX | FUNC NAME | CODE PTR | SAVE LINK | STACK PTR | FRAME PTR");
	uint32_t idx = 0;
	while ((void*) bt < (void*) startstack) {
		dump_bt(bt, idx);
		bt = (backtrace_t*) (bt->frameptr - 0xC);
		idx++;
	}
	main_error("=======================================");
}

void dump_stack_trace_stack(uint32_t stack, uint32_t fp) {
	backtrace_t* bt;
	uint32_t startstack;

	fp = fp - 0xC;

	bt = (backtrace_t*) (fp);

	if (get_current_task() != NULL ) {
		startstack = ((task_t*) get_current_task())->kernelStartSP;
	} else {
		startstack = (uint32_t) &svc_stack;
	}
	main_error("=======================================");
	main_error("STACK TRACE:");
	main_error("=======================================");
	main_error("IDX | FUNC NAME | CODE PTR | SAVE LINK | STACK PTR | FRAME PTR");
	uint32_t idx = 0;
	while ((void*) bt < (void*) startstack) {
		dump_bt(bt, idx);
		bt = (backtrace_t*) (bt->frameptr - 0xC);
		idx++;
	}
	main_error("=======================================");
}

void kpanic() {
	main_error("#########################################################");
	main_error("#########################################################");
	main_error("                    KERNEL PANIC");
	main_error("#########################################################");
	main_error("#########################################################");
	dump_stack_trace();
	main_error("#########################################################");
	main_error("#########################################################");
	main_error("                      STOPPING");
	main_error("#########################################################");
	DEBUG_STOP;
}

void kdumpMem(void* mem, uint32_t size) {
	uint32_t i = 0;
	uint32_t j = 0;
	uint8_t* buffer = (uint8_t*) mem;
	for (i = 0; i < size; i = i + 16) {

		kprintf_ex("0x%x ", buffer + i);

		for (j = 0; j < 16; j++) {
			if (i + j < size) {
				kprintf_ex("0x%x ", buffer[i + j]);
			}
		}

		kprintf_ex("| ");

		for (j = 0; j < 16; j++) {
			if (i + j < size) {
				if (buffer[i + j] >= 0x20 && buffer[i + j] <= 0x7E) {
					kprintf_ex("%c", buffer[i + j]);
				} else {
					kprintf_ex(".");
				}
			}
		}

		kprintf_ex("\n");
	}
}

void kprintf_ex(const char *s, ...) {
	va_list va;

	va_start(va, s);

	while (*s) {
		switch (*(s++)) {
		case '%':
			switch (*(s++)) {
			case '%':
				mputchar_ex('%');
				break;
			case 's': {
				const char *ns = va_arg(va, const char *);
				while (*ns)
					mputchar_ex(*(ns++));
				break;
			}
			case 'c':
				mputchar_ex(va_arg(va, int));
				break;
			case 'd':
			case 'i': {
				int num = va_arg(va, int);
				if (!num)
					mputchar_ex('0');
				else {
					if (num < 0) {
						mputchar_ex('-');
						num *= -1;
					}

					char buf[11];
					int i = 11;
					while (num) {
						buf[--i] = num % 10 + '0';
						num /= 10;
					}
					while (i < 11)
						mputchar_ex(buf[i++]);
				}
				break;
			}
			case 'x': {
				unsigned num = va_arg(va, unsigned);
				if (!num)
					mputchar_ex('0');
				else {
					int i;
					for (i = 0; !(num >> 28); i++)
						num <<= 4;
					for (; i < 8; i++) {
						mputchar_ex(
								(num >> 28)
										+ ((num >> 28) < 10 ? '0' : 'a' - 10));
						num <<= 4;
					}
				}
				break;
			}
			case 'X': {
				unsigned num = va_arg(va, unsigned);
				if (!num)
					mputchar_ex('0');
				else {
					int i;
					for (i = 0; !(num >> 28); i++)
						num <<= 4;
					for (; i < 8; i++) {
						mputchar_ex(
								(num >> 28)
										+ ((num >> 28) < 10 ? '0' : 'A' - 10));
						num <<= 4;
					}
				}
				break;
			}
			case 'p': {
				unsigned num = va_arg(va, unsigned);
				mputchar_ex('0');
				mputchar_ex('x');
				for (int i = 0; i < 8; i++) {
					mputchar_ex(
							(num >> 28) + ((num >> 28) < 10 ? '0' : 'a' - 10));
					num <<= 4;
				}
				break;
			}
			default:
				mputchar_ex('%');
				mputchar_ex(s[-1]);
				break;
			}
			break;
		default:
			mputchar_ex(s[-1]);
			break;
		}
	}

	va_end(va);
}

void kprintf(const char *s, ...) {
	va_list va;

	va_start(va, s);

	while (*s) {
		switch (*(s++)) {
		case '%':
			switch (*(s++)) {
			case '%':
				mputchar('%');
				break;
			case 's': {
				const char *ns = va_arg(va, const char *);
				while (*ns)
					mputchar(*(ns++));
				break;
			}
			case 'c':
				mputchar(va_arg(va, int));
				break;
			case 'd':
			case 'i': {
				int num = va_arg(va, int);
				if (!num)
					mputchar('0');
				else {
					if (num < 0) {
						mputchar('-');
						num *= -1;
					}

					char buf[11];
					int i = 11;
					while (num) {
						buf[--i] = num % 10 + '0';
						num /= 10;
					}
					while (i < 11)
						mputchar(buf[i++]);
				}
				break;
			}
			case 'x': {
				unsigned num = va_arg(va, unsigned);
				if (!num)
					mputchar('0');
				else {
					int i;
					for (i = 0; !(num >> 28); i++)
						num <<= 4;
					for (; i < 8; i++) {
						mputchar(
								(num >> 28)
										+ ((num >> 28) < 10 ? '0' : 'a' - 10));
						num <<= 4;
					}
				}
				break;
			}
			case 'X': {
				unsigned num = va_arg(va, unsigned);
				if (!num)
					mputchar('0');
				else {
					int i;
					for (i = 0; !(num >> 28); i++)
						num <<= 4;
					for (; i < 8; i++) {
						mputchar(
								(num >> 28)
										+ ((num >> 28) < 10 ? '0' : 'A' - 10));
						num <<= 4;
					}
				}
				break;
			}
			case 'p': {
				unsigned num = va_arg(va, unsigned);
				mputchar('0');
				mputchar('x');
				for (int i = 0; i < 8; i++) {
					mputchar((num >> 28) + ((num >> 28) < 10 ? '0' : 'a' - 10));
					num <<= 4;
				}
				break;
			}
			default:
				mputchar('%');
				mputchar(s[-1]);
				break;
			}
			break;
		default:
			mputchar(s[-1]);
			break;
		}
	}

	va_end(va);
}

