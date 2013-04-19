/*
 * tee.c
 *
 *  Created on: Apr 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee/tee_constants.h>
#include <tee/tee_context.h>
#include <tee/tee_memregion.h>
#include <tee/tee_session.h>

void tee_initialize() {
	tee_context_initialize();
	tee_session_initialize();
	tee_memory_initialize();
}
