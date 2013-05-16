/*
 * sample_trustlet.h
 *
 *  Created on: May 13, 2013
 *      Author: andy
 */

#ifndef SAMPLE_TRUSTLET_H_
#define SAMPLE_TRUSTLET_H_

#define TZ_NEW_KEY	(0x1)
#define TZ_ENCRYPT	(0x11)
#define TZ_DECRYPT	(0x21)

typedef struct {
	unsigned char iv[16];
	unsigned char data[1];
} crypt_pack_t;

#endif /* SAMPLE_TRUSTLET_H_ */
