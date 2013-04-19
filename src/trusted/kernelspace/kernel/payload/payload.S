/*
 * payload.S
 *
 *  Created on: Nov 19, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */
.section ".payload", "awx"

payload_os:
        .incbin "payload/norm_kernel.bin"


