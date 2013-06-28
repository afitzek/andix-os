/*
 * payload.S
 *
 *  Created on: Nov 19, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */
.section ".payload_and", "awx"

payload_and_os:
        .incbin "payload/android_kernel.bin"


