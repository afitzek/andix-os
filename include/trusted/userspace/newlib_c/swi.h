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

static inline unsigned int __swi_1(unsigned int call, unsigned int arg0)
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

static inline unsigned int __swi_2(unsigned int call, unsigned int arg0, unsigned int arg1)
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

static inline unsigned int __swi_3(unsigned int call, unsigned int arg0, unsigned int arg1,
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

static inline unsigned int __swi_4(unsigned int call, unsigned int arg0, unsigned int arg1,
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
