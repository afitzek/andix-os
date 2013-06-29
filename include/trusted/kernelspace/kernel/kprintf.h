/*
 * kprintf.h
 *
 *  Created on: Aug 24, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef KPRINTF_H_
#define KPRINTF_H_

#include <common/typedefs.h>
//#include <drivers/drivers.h>

// Remove this define if __func__ not available!
#define _FUNCTION_KEYWORD_

void printA();

typedef struct {
	uint32_t	frameptr;
	uint32_t	stackptr;
	uint32_t	linkvalue;
	uint32_t	codeptr;
} backtrace_t;

#define SEPERATOR "##########"
#define LINE_SEPERATOR "########################################"

void (*kputchar)(char c);

void dump_stack_trace();

void kprintf(const char *s, ...);

void init_serial(uint32_t sysid);

void kprintf_ex(const char *s, ...);

void kpanic();

void getSerial();

int getchar();

void mputchar(char c);

void kprintHex(uint8_t* buffer, uint32_t size);

void getinput(char *buffer, uint32_t size);

char* gettaskname();

void dump_stack_trace_stack(uint32_t stack, uint32_t fp);

#define non_print_handler(subsys,...) \
	do{}while(0)

#ifdef _FUNCTION_KEYWORD_

#define userspace_out(...) \
	kprintf("[USERSPACE][%s]:", gettaskname()); \
	kprintf(__VA_ARGS__);

#define subsys_error(subsys,...) \
	kprintf("[ERR][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_error_ex(subsys,...) \
	kprintf_ex("[ERR][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#else
#define subsys_error(subsys,...) \
	kprintf("[ERR] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_error_ex(subsys,...) \
	kprintf_ex("[ERR] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#endif

#ifdef DEBUG
#ifdef _FUNCTION_KEYWORD_
#define subsys_debug(subsys,...) \
	kprintf("[DBG][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_debug_ex(subsys,...) \
	kprintf_ex("[DBG][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#else
#define subsys_debug(subsys,...) \
	kprintf("[DBG] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_debug_ex(subsys,...) \
	kprintf_ex("[DBG] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#endif
#else
#define subsys_debug(subsys,...) \
	non_print_handler(subsys, __VA_ARGS__)

#define subsys_debug_ex(subsys,...) \
	non_print_handler(subsys, __VA_ARGS__)
#endif

#ifdef INFO
#ifdef _FUNCTION_KEYWORD_
#define subsys_info(subsys,...) \
	kprintf("[INF][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_info_ex(subsys,...) \
	kprintf_ex("[INF][%s]->%s [%s(%d)] [%s]:", subsys, __func__, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#else
#define subsys_info(subsys,...) \
	kprintf("[INF] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf(__VA_ARGS__); \
	kprintf("\n")

#define subsys_info_ex(subsys,...) \
	kprintf_ex("[INF] [%s] %s(%d) [%s]:", subsys, __FILE__, __LINE__, gettaskname()); \
	kprintf_ex(__VA_ARGS__); \
	kprintf_ex("\n")
#endif
#else
#define subsys_info(subsys,...) \
	non_print_handler(subsys, __VA_ARGS__)

#define subsys_info_ex(subsys,...) \
	non_print_handler(subsys, __VA_ARGS__)
#endif

// Main

#define MAIN_TAG " main"

#define main_error(...) subsys_error(MAIN_TAG, __VA_ARGS__)
#define main_info(...) subsys_info(MAIN_TAG, __VA_ARGS__)
#define main_debug(...) subsys_debug(MAIN_TAG, __VA_ARGS__)

// PMM

#define PMM_TAG " pmm "

#define pmm_error(...) subsys_error(PMM_TAG, __VA_ARGS__)
#define pmm_info(...) subsys_info(PMM_TAG, __VA_ARGS__)
#define pmm_debug(...) subsys_debug(PMM_TAG, __VA_ARGS__)

// MMM

#define MMM_TAG " mmm "

#define mmm_error(...) subsys_error(MMM_TAG, __VA_ARGS__)
#define mmm_info(...) subsys_info(MMM_TAG, __VA_ARGS__)
#define mmm_debug(...) subsys_debug(MMM_TAG, __VA_ARGS__)

// HAL

#define HAL_TAG " hal "

#define hal_error(...) subsys_error(HAL_TAG, __VA_ARGS__)
#define hal_info(...) subsys_info(HAL_TAG, __VA_ARGS__)
#define hal_debug(...) subsys_debug(HAL_TAG, __VA_ARGS__)

// LOADER

#define LOADER_TAG " load"

#define loader_error(...) subsys_error(LOADER_TAG, __VA_ARGS__)
#define loader_info(...) subsys_info(LOADER_TAG, __VA_ARGS__)
#define loader_debug(...) subsys_debug(LOADER_TAG, __VA_ARGS__)

// LIST

#define LIST_TAG " list"

#define list_error(...) subsys_error(LIST_TAG, __VA_ARGS__)
#define list_info(...) subsys_info(LIST_TAG, __VA_ARGS__)
#define list_debug(...) subsys_debug(LIST_TAG, __VA_ARGS__)

// MON

#define MON_TAG " mon "

#define mon_error(...) subsys_error(MON_TAG, __VA_ARGS__)
#define mon_info(...) subsys_info(MON_TAG, __VA_ARGS__)
#define mon_debug(...) subsys_debug(MON_TAG, __VA_ARGS__)
//#define mon_debug(...) non_print_handler(MON_TAG, __VA_ARGS__)

// CMM

#define CMM_TAG " cmm "

#define cmm_error(...) subsys_error(CMM_TAG, __VA_ARGS__)
#define cmm_info(...) subsys_info(CMM_TAG, __VA_ARGS__)
#define cmm_debug(...) subsys_debug(CMM_TAG, __VA_ARGS__)

// ATAG

#define ATAGS_TAG "atags"

#define atag_error(...) subsys_error(ATAGS_TAG, __VA_ARGS__)
#define atag_info(...) subsys_info(ATAGS_TAG, __VA_ARGS__)
#define atag_debug(...) subsys_debug(ATAGS_TAG, __VA_ARGS__)

// VMM

#define VMM_TAG " vmm "

#define vmm_error(...) subsys_error(VMM_TAG, __VA_ARGS__)
#define vmm_info(...) subsys_info(VMM_TAG, __VA_ARGS__)
#define vmm_debug(...) subsys_debug(VMM_TAG, __VA_ARGS__)
//#define vmm_debug(...) non_print_handler(VMM_TAG, __VA_ARGS__)

// SMM

#define SMM_TAG " smm "

#define smm_error(...) subsys_error(SMM_TAG, __VA_ARGS__)
#define smm_info(...) subsys_info(SMM_TAG, __VA_ARGS__)
#define smm_debug(...) subsys_debug(SMM_TAG, __VA_ARGS__)

// UMM

#define UMM_TAG " umm "

#define umm_error(...) subsys_error(UMM_TAG, __VA_ARGS__)
#define umm_info(...) subsys_info(UMM_TAG, __VA_ARGS__)
#define umm_debug(...) subsys_debug(UMM_TAG, __VA_ARGS__)

// DAB

#define DAB_TAG " dab "

#define dab_error(...) subsys_error_ex(DAB_TAG, __VA_ARGS__)
#define dab_info(...) subsys_info_ex(DAB_TAG, __VA_ARGS__)
#define dab_debug(...) subsys_debug_ex(DAB_TAG, __VA_ARGS__)

// EX

#define EX_TAG "  ex "

#define ex_error(...) subsys_error_ex(EX_TAG, __VA_ARGS__)
#define ex_info(...) subsys_info_ex(EX_TAG, __VA_ARGS__)
#define ex_debug(...) subsys_debug_ex(EX_TAG, __VA_ARGS__)

// TASK

#define TASK_TAG " task"

#define task_error(...) subsys_error(TASK_TAG, __VA_ARGS__)
#define task_info(...) subsys_info(TASK_TAG, __VA_ARGS__)
#define task_debug(...) subsys_debug(TASK_TAG, __VA_ARGS__)

// VECTOR

#define VECTOR_TAG " vec "

#define vector_error(...) subsys_error(VECTOR_TAG, __VA_ARGS__)
#define vector_info(...) subsys_info(VECTOR_TAG, __VA_ARGS__)
#define vector_debug(...) subsys_debug(VECTOR_TAG, __VA_ARGS__)

// SCHEDULER

#define SCHEDULER_TAG "sched"

#define scheduler_error(...) subsys_error(SCHEDULER_TAG, __VA_ARGS__)
#define scheduler_info(...) subsys_info(SCHEDULER_TAG, __VA_ARGS__)
#define scheduler_debug(...) subsys_debug(SCHEDULER_TAG, __VA_ARGS__)

// SWI

#define SWI_TAG " swi "

#define swi_error(...) subsys_error(SWI_TAG, __VA_ARGS__)
#define swi_info(...) subsys_info(SWI_TAG, __VA_ARGS__)
//#define swi_debug(...) subsys_debug(SWI_TAG, __VA_ARGS__)
#define swi_debug(...) non_print_handler(SWI_TAG, __VA_ARGS__)

// USER

#define USER_TAG " usr "

#define user_error(...) subsys_error(USER_TAG, __VA_ARGS__)
#define user_info(...) subsys_info(USER_TAG, __VA_ARGS__)
#define user_debug(...) subsys_debug(USER_TAG, __VA_ARGS__)

// CRYPTO

#define CRYPTO_TAG "crypt"

#define crypto_error(...) subsys_error(CRYPTO_TAG, __VA_ARGS__)
#define crypto_info(...) subsys_info(CRYPTO_TAG, __VA_ARGS__)
#define crypto_debug(...) subsys_debug(CRYPTO_TAG, __VA_ARGS__)

// CRYPTO

#define CRYPTO_FS_TAG " cfs "

#define crypto_fs_error(...) subsys_error(CRYPTO_FS_TAG, __VA_ARGS__)
#define crypto_fs_info(...) subsys_info(CRYPTO_FS_TAG, __VA_ARGS__)
#define crypto_fs_debug(...) subsys_debug(CRYPTO_FS_TAG, __VA_ARGS__)

// SERVICE

#define SERVICE_TAG " srv "

#define service_error(...) subsys_error(SERVICE_TAG, __VA_ARGS__)
#define service_info(...) subsys_info(SERVICE_TAG, __VA_ARGS__)
#define service_debug(...) subsys_debug(SERVICE_TAG, __VA_ARGS__)

// PROCESSOR

#define PROCESSOR_TAG " srv "

#define processor_error(...) subsys_error(PROCESSOR_TAG, __VA_ARGS__)
#define processor_info(...) subsys_info(PROCESSOR_TAG, __VA_ARGS__)
#define processor_debug(...) subsys_debug(PROCESSOR_TAG, __VA_ARGS__)

// PROCESSOR

#define PFS_TAG " pfs "

#define plain_fs_error(...) subsys_error(PFS_TAG, __VA_ARGS__)
#define plain_fs_info(...) subsys_info(PFS_TAG, __VA_ARGS__)
#define plain_fs_debug(...) subsys_debug(PFS_TAG, __VA_ARGS__)

// REE FS

#define REE_TAG "reefs"

#define ree_fs_error(...) subsys_error(REE_TAG, __VA_ARGS__)
#define ree_fs_info(...) subsys_info(REE_TAG, __VA_ARGS__)
#define ree_fs_debug(...) subsys_debug(REE_TAG, __VA_ARGS__)

// FS

#define FS_TAG "  fs "

#define fs_error(...) subsys_error(FS_TAG, __VA_ARGS__)
#define fs_info(...) subsys_info(FS_TAG, __VA_ARGS__)
#define fs_debug(...) subsys_debug(FS_TAG, __VA_ARGS__)

// TEE

#define TEE_TAG " tee "

#define tee_error(...) subsys_error(TEE_TAG, __VA_ARGS__)
#define tee_info(...) subsys_info(TEE_TAG, __VA_ARGS__)
//#define tee_debug(...) subsys_debug(TEE_TAG, __VA_ARGS__)
#define tee_debug(...) non_print_handler(TEE_TAG, __VA_ARGS__)

// IRQ

#define IRQ_TAG " irq "

#define irq_error(...) subsys_error(IRQ_TAG, __VA_ARGS__)
#define irq_info(...) subsys_info(IRQ_TAG, __VA_ARGS__)
#define irq_debug(...) subsys_debug(IRQ_TAG, __VA_ARGS__)


#endif /* KPRINTF_H_ */
