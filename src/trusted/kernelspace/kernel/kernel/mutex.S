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
