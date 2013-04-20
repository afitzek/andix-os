/*
 * andix.c
 *
 *  Created on: Jan 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <swi.h>
#include <andix.h>
#include <ta_interface.h>

void __ta_get_secure_request(TA_RPC* rpc) {
	__swi_1(SWI_SECURE_REQUEST, (unsigned int)rpc);
}

void set_uuid(const TEE_UUID* uuid) {
	__swi_1(SWI_SECURE_SET_UUID,(int) uuid);
}
