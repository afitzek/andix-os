/*
 * entropy.h
 *
 *  Created on: May 24, 2013
 *      Author: andy
 */

#ifndef ENTROPY_H_
#define ENTROPY_H_

int platform_pseudo_entropy( void *data,
                           unsigned char *output, size_t len, size_t *olen );

#endif /* ENTROPY_H_ */
