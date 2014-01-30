/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/*
 * tee.h
 *
 *  Created on: Apr 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TASK_TEE_H_
#define TASK_TEE_H_

/**
 * \addtogroup teetask
 *	\{
 */

/**
 * Get a pointer to the system TEE service thread.
 * @return
 */
struct thread_t *tee_get_tee_thread(void);
/**
 * TEE release memory handler
 */
uint32_t tee_release_mem(TZ_TEE_REGISTER_MEM* operation);

/**
 * TEE register memory handler
 */
uint32_t tee_register_mem(TZ_TEE_REGISTER_MEM* operation);

/**
 * TEE initialize context handler
 */
uint32_t tee_init_context(TZ_TEE_INIT_CTX* operation) ;

/**
 * TEE finalize context handler
 */
uint32_t tee_finalize_context(TZ_TEE_FIN_CTX* operation);

/**
 * TEE open session handler
 */
uint32_t tee_open_session(TZ_TEE_OPEN_SESSION* operation, TZ_TEE_SPACE* tee);

/**
 * TEE close session handler
 */
uint32_t tee_close_session(TZ_TEE_CLOSE_SESSION* operation, TZ_TEE_SPACE* tee);

/**
 * TEE invoke handler
 */
uint32_t tee_invoke(TZ_TEE_INVOKE_CMD* operation, TZ_TEE_SPACE* tee);

/**
 * Entry of tee task
 */
void tee_task_entry();

/* \} group */

#endif /* TASK_TEE_H_ */
