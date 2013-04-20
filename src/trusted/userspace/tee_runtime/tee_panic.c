/*
 * tee_panic.c
 *
 *  Created on: Feb 27, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_types.h>

/**
 * \defgroup panic TEE Panic Functions
 *
 */

void TEE_Panic(TEE_Result panicCode) {
	printf("!!! TEE PANIC 0x%x\n", panicCode);
	// TODO: KILL USER APP
	for(;;) {
	}
}
