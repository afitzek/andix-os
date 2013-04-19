/*
 * payload.S
 *
 *  Created on: Nov 19, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */
.section ".dtb", "awx"

payload_dtb:
        .incbin "payload/imx53.dtb"


