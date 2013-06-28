/*
 * payload.S
 *
 *  Created on: Nov 19, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */
.section ".payload_and_rd", "awx"

payload__and_os_rd:
        .incbin "payload/android_ramdisk.bin"


