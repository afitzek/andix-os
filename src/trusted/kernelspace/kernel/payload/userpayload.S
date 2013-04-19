/*
 * payload.S
 *
 *  Created on: Nov 19, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */
.section ".usrpayload", "awx"

payload_user:
        .incbin "payload/tz_sys_daemon.bin"


