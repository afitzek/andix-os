/*
 * andix_packer.h
 *
 *  Created on: Mar 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef ANDIX_PACKER_H_
#define ANDIX_PACKER_H_

#include <tee_internal_api.h>

typedef struct {
	char			name[64];
	TEE_UUID		uuid;
	uint32_t		datalen; // length of binary data after here
} andixpack;

#endif /* ANDIX_PACKER_H_ */
