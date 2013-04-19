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
