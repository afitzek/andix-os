/*
 * tee_bits.h
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_BITS_H_
#define TEE_BITS_H_

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define TEE_IS_REF_ATTR(attr) ((CHECK_BIT(attr, 29)) == 0)
#define TEE_IS_VAL_ATTR(attr) ((CHECK_BIT(attr, 29)) != 0)

#endif /* TEE_BITS_H_ */
