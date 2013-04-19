/*
 * mutex.S
 *
 *  Created on: Nov 3, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

.arm

// lock_mutex
// Declare for use from C as extern void lock_mutex(void * mutex);
.global lock_mutex
lock_mutex:// PROC
    MOV     r1, #1
lt: LDREX   r2, [r0]
    CMP     r2, r1        // Test if mutex is locked or unlocked
	BEQ     lk           // If locked - wait for it to be released, from 2
    STREXNE r2, r1, [r0]  // Not locked, attempt to lock it
    CMPNE   r2, #1        // Check if Store-Exclusive failed
    BEQ     lt           // Failed - retry from 1
    // Lock acquired
    DMB                   // Required before accessing protected resource
    BX      lr

lk:   // Take appropriate action while waiting for mutex to become unlocked
    //WAIT_FOR_UPDATE
    B       lt           // Retry from 1
    //ENDP


// unlock_mutex
// Declare for use from C as extern void unlock_mutex(void * mutex);
.global unlock_mutex
unlock_mutex:// PROC
    MOV     r1, #0
    DMB                   // Required before releasing protected resource
    STR     r1, [r0]      // Unlock mutex
    //SIGNAL_UPDATE
    BX      lr
    //ENDP
