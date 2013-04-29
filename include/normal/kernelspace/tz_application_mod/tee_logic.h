/*
 * tee_logic.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_LOGIC_H_
#define TEE_LOGIC_H_

#include <tz_application_mod/tee_context.h>
#include <tz_application_mod/tee_session.h>
#include <tz_application_mod/tee_memory.h>

int tee_init(void);
int tee_process(TZ_TEE_SPACE* com_mem);

#endif /* TEE_LOGIC_H_ */
