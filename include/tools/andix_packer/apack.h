/*
 * apack.h
 *
 *  Created on: Mar 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef APACK_H_
#define APACK_H_

typedef unsigned int _pack_uint32_t;
typedef unsigned short _pack_uint16_t;
typedef unsigned char _pack_uint8_t;

typedef struct {
	_pack_uint32_t timeLow;
	_pack_uint16_t timeMid;
	_pack_uint16_t timeHiAndVersion;
	_pack_uint8_t clockSeqAndNode[8];
} PACK_UUID;

// TODO: add crypto to protect binary
typedef struct {
	char			name[64];
	PACK_UUID		uuid;
	_pack_uint32_t	datalen; // length of binary data after here
} andixpack;

#endif /* APACK_H_ */
