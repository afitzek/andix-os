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
 * mutex.h
 *
 *  Created on: Nov 28, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <common/typedefs.h>

/**
 * The spinlock data structure
 */
struct	__spinlock {
	uint32_t		_value;
	tid_t			owner_id;
};

/**
 * The spinlock type
 */
typedef struct __spinlock spinlock_t;

/**
 * Lock the spinlock
 * @param lock The spinlock
 */
void spinlock_lock(spinlock_t * lock);

/**
 * Unlock the spinlock
 * @param lock The spinlock
 */
void spinlock_unlock(spinlock_t * lock);

// =====================================================
// Internal stuff
// =====================================================

/**
 * Defines the locked value
 * @param lock
 */
#define LOCKED	1

/**
 * Atomic locking function (use spinlock_lock etc.)
 * @param lock The lock
 */
void atomic_lock(uint32_t* lock);

/**
 * Atomic increment function (don't use directly)
 * @param lock The lock
 * @return the value
 */
uint32_t atomic_inc(uint32_t* lock);

/**
 * Atomic decrement function (don't use directly)
 * @param lock The lock
 * @return the value
 */
uint32_t atomic_dec(uint32_t* lock);

#endif /* MUTEX_H_ */
