/*
 * stackprotect.c
 *
 *  Created on: Sep 2, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

void * __stack_chk_guard = 0x0;

void __stack_chk_guard_setup() {
	unsigned char * p;
	p = (unsigned char *) &__stack_chk_guard;

	/* If you have the ability to generate random numbers in your kernel then use them,
	 otherwise for 32-bit code: */
	*p = 0xDE;
}

void __attribute__((noreturn)) __stack_chk_fail() {
	/* put your panic function or similar in here */
	unsigned char * vid = (unsigned char *) 0xB8000;
	vid[1] = 7;
	for (;;)
		vid[0]++;
}
