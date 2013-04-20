/*
 * tee_panics.h
 *
 *  Created on: Feb 27, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_PANICS_H_
#define TEE_PANICS_H_

#include <tee_internal_api.h>

/**
 * \addtogroup panic
 *	\{
 */

/**
 * Panic in Trusted Application Instance kill application
 *
 * This function will not return!
 *
 * @param panicCode The reason for the panic
 */
void TEE_Panic(TEE_Result panicCode);

/* \} group */


#endif /* TEE_PANICS_H_ */
