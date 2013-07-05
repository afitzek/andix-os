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
 * swi.h
 *
 *  Created on: Jan 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef SWI_H_
#define SWI_H_

#define SWI_WRITE	0x01
#define SWI_READ	0x02
#define SWI_SBRK	0x03
#define SWI_HEND	0x04
#define SWI_OPEN	0x05
#define SWI_CLOSE	0x06
#define SWI_LSEEK	0x07
#define SWI_FSTAT   0x08

#define SWI_SECURE_REQUEST 0x81
#define SWI_SECURE_SET_UUID 0x82
#define SWI_GET_RAND 0x83

#define SWI_SYSD_INSTALL   0xA1

#define forceinline __inline__ __attribute__((always_inline))
#define ensure_forceinline __attribute__((always_inline))

forceinline static unsigned int __swi_1(unsigned int call, unsigned int arg0)
{
  register unsigned int __call __asm__("ip") = call;
  register unsigned int __arg0 __asm__("r0") = arg0;
  __asm__ __volatile__ ("swi #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

forceinline static unsigned int __swi_2(unsigned int call, unsigned int arg0, unsigned int arg1)
{
  register unsigned int __call __asm__("ip") = call;
  register unsigned int __arg0 __asm__("r0") = arg0;
  register unsigned int __arg1 __asm__("r1") = arg1;
  __asm__ __volatile__ ("swi #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

forceinline static unsigned int __swi_3(unsigned int call, unsigned int arg0, unsigned int arg1,
		unsigned int arg2)
{
  register unsigned int __call __asm__("ip") = call;
  register unsigned int __arg0 __asm__("r0") = arg0;
  register unsigned int __arg1 __asm__("r1") = arg1;
  register unsigned int __arg2 __asm__("r2") = arg2;
  __asm__ __volatile__ ("swi #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [arg2]"r"(__arg2),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

forceinline static unsigned int __swi_4(unsigned int call, unsigned int arg0, unsigned int arg1,
		unsigned int arg2, unsigned int arg3)
{
  register unsigned int __call __asm__("ip") = call;
  register unsigned int __arg0 __asm__("r0") = arg0;
  register unsigned int __arg1 __asm__("r1") = arg1;
  register unsigned int __arg2 __asm__("r2") = arg2;
  register unsigned int __arg3 __asm__("r3") = arg3;
  __asm__ __volatile__ ("swi #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [arg2]"r"(__arg2),
                          [arg3]"r"(__arg3),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}



#endif /* SWI_H_ */
