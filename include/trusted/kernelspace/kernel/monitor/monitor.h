/*
 * monitor.h
 *
 *  Created on: Nov 5, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <kprintf.h>
#include <common.h>
#include <platform/platform.h>
#include <vectors.h>
#include <communication_types.h>

#define SSC_TASK_SWITCH		0
#define SSC_TASK_SCHEDULE	1
#define SSC_NONS_SERVICE	2

typedef struct {
	uint32_t scr;
	uint32_t r[13];
	uint32_t pc;
	uint32_t cpsr;
} mon_context_t;

typedef struct {
    uint32_t svc_spsr;
    uint32_t svc_sp;
    uint32_t svc_lr;

    uint32_t abt_spsr;
    uint32_t abt_sp;
    uint32_t abt_lr;

    uint32_t und_spsr;
    uint32_t und_sp;
    uint32_t und_lr;

    uint32_t irq_spsr;
    uint32_t irq_sp;
    uint32_t irq_lr;

    uint32_t fiq_spsr;
    uint32_t fiq_r8;
    uint32_t fiq_r9;
    uint32_t fiq_r10;
    uint32_t fiq_r11;
    uint32_t fiq_r12;
    uint32_t fiq_sp;
    uint32_t fiq_lr;

    uint32_t sys_sp;
    uint32_t sys_lr;
} mon_sys_context_t;

static inline uint32_t __smc_1(uint32_t call, uint32_t arg0)
{
  register uint32_t __call __asm__("ip") = call;
  register uint32_t __arg0 __asm__("r0") = arg0;
  __asm__ __volatile__ ("smc #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

static inline uint32_t __smc_2(uint32_t call, uint32_t arg0, uint32_t arg1)
{
  register uint32_t __call __asm__("ip") = call;
  register uint32_t __arg0 __asm__("r0") = arg0;
  register uint32_t __arg1 __asm__("r1") = arg1;
  __asm__ __volatile__ ("smc #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

static inline uint32_t __smc_3(uint32_t call, uint32_t arg0, uint32_t arg1,
                               uint32_t arg2)
{
  register uint32_t __call __asm__("ip") = call;
  register uint32_t __arg0 __asm__("r0") = arg0;
  register uint32_t __arg1 __asm__("r1") = arg1;
  register uint32_t __arg2 __asm__("r2") = arg2;
  __asm__ __volatile__ ("smc #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [arg2]"r"(__arg2),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

static inline uint32_t __smc_4(uint32_t call, uint32_t arg0, uint32_t arg1,
                               uint32_t arg2, uint32_t arg3)
{
  register uint32_t __call __asm__("ip") = call;
  register uint32_t __arg0 __asm__("r0") = arg0;
  register uint32_t __arg1 __asm__("r1") = arg1;
  register uint32_t __arg2 __asm__("r2") = arg2;
  register uint32_t __arg3 __asm__("r3") = arg3;
  __asm__ __volatile__ ("smc #0\n"
                        : [result]"=&r"(__arg0)
                        : [arg0]"0"(__arg0),
                          [arg1]"r"(__arg1),
                          [arg2]"r"(__arg2),
                          [arg3]"r"(__arg3),
                          [call]"r"(__call)
                        : "memory");
  return (__arg0);
}

extern TZ_CTLR_SPACE* com_mem;
extern TZ_TEE_SPACE* tee_mem;

TZ_CTLR_SPACE* mon_get_control_space();
TZ_TEE_SPACE* mon_get_tee_space();
void mon_ctrl_request();
void setup_mon_stacks();
void monitor_save_sys_regs(mon_sys_context_t *sysregs);
void monitor_restore_sys_regs(mon_sys_context_t *sysregs);
uint32_t monitor_com_mem_dispatcher();

#endif /* MONITOR_H_ */
