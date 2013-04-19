/*
 * platform.h
 *
 *  Created on: Sep 7, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#define SVC_STACK_SIZE 4096
#define IRQ_STACK_SIZE 512
#define DA_STACK_SIZE  512
#define FIQ_STACK_SIZE 512
#define SYS_STACK_SIZE 1024


// defined CPSR and SPSR bits (selected parts)
#define PSR_T       0x00000020
#define PSR_F       0x00000040
#define PSR_I       0x00000080
#define PSR_A       0x00000100
#define PSR_E       0x00000200

#define MODE_BITS 	0x1F   /* Bit mask for mode bits in CPSR */
#define USR_MODE	0x10   /* User mode */
#define FIQ_MODE 	0x11   /* Fast Interrupt Request mode */
#define IRQ_MODE 	0x12   /* Interrupt Request mode */
#define SVC_MODE 	0x13   /* Supervisor mode */
#define ABT_MODE 	0x17   /* Abort mode */
#define UND_MODE 	0x1B   /* Undefined Instruction mode */
#define SYS_MODE 	0x1F   /* System mode */
#define MON_MODE   	0x16

#define USR_MODE_STR	"USR"   /* User mode */
#define FIQ_MODE_STR 	"FIQ"   /* Fast Interrupt Request mode */
#define IRQ_MODE_STR 	"IRQ"   /* Interrupt Request mode */
#define SVC_MODE_STR 	"SVC"   /* Supervisor mode */
#define ABT_MODE_STR 	"ABT"   /* Abort mode */
#define UND_MODE_STR 	"UND"   /* Undefined Instruction mode */
#define SYS_MODE_STR 	"SYS"   /* System mode */
#define MON_MODE_STR   	"MON"

/* Secure configuration register */
#define SCR_NS  (1 << 0)
#define SCR_IRQ (1 << 1)
#define SCR_FIQ (1 << 2)
#define SCR_EA  (1 << 3)
#define SCR_FW  (1 << 4)
#define SCR_AW  (1 << 5)


#define DFSR_RW (1 << 11)
#define DFSR_SD (1 << 12)
#define DFSR_DOMAIN 0xF0
#define DFSR_STATUS 0xF
#define DFSR_STATUS_1 (1 << 10)

#define DA_STATUS_ALIGN_S		"alignment fault"
#define DA_STATUS_INSTR_S		"instruction cache maintenance fault"
#define DA_STATUS_L1PRE_S		"L1 translation, precise external abort"
#define DA_STATUS_L1PRE_AXI_S	"L1 translation, precise external abort (AXI)"
#define DA_STATUS_L2PRE_S		"L2 translation, precise external abort"
#define DA_STATUS_L2PRE_AXI_S	"L2 translation, precise external abort (AXI)"
#define DA_STATUS_L1PPE_S		"L1 translation precise parity error"
#define DA_STATUS_L2PPE_S		"L2 translation precise parity error"
#define DA_STATUS_TFSEC_S		"translation fault, section"
#define DA_STATUS_TFPAG_S		"translation fault, page"
#define DA_STATUS_AFSEC_S		"access flag fault, section"
#define DA_STATUS_AFPAG_S		"access flag fault, page"
#define DA_STATUS_DFSEC_S		"domain fault, section"
#define DA_STATUS_DFPAG_S		"domain fault, page"
#define DA_STATUS_PFSEC_S		"permission fault, section"
#define DA_STATUS_PFPAG_S		"permission fault, page"
#define DA_STATUS_PEANT_S		"precise external abort, nontranslation"
#define DA_STATUS_PEANT_AXI_S	"precise external abort, nontranslation (AXI)"
#define DA_STATUS_IMPEA_S		"imprecise external abort"
#define DA_STATUS_IMPEA_AXI_S	"imprecise external abort (AXI)"
#define DA_STATUS_IMEPE_S		"imprecise error, parity or ECC"
#define DA_STATUS_DEBUG_S		"debug event"


#define DA_STATUS_ALIGN 		1  //0b000001
#define DA_STATUS_INSTR 		4  //0b000100
#define DA_STATUS_L1PRE 		12 //0b001100
#define DA_STATUS_L1PRE_AXI 	44 //0b101100
#define DA_STATUS_L2PRE 		14 //0b001110
#define DA_STATUS_L2PRE_AXI 	46 //0b101110
#define DA_STATUS_L1PPE 		28 //0b011100
#define DA_STATUS_L2PPE			30 //0b011110
#define DA_STATUS_TFSEC			5  //0b000101
#define DA_STATUS_TFPAG			7  //0b000111
#define DA_STATUS_AFSEC			3  //0b000011
#define DA_STATUS_AFPAG			6  //0b000110
#define DA_STATUS_DFSEC			9  //0b001001
#define DA_STATUS_DFPAG			11 //0b001011
#define DA_STATUS_PFSEC			13 //0b001101
#define DA_STATUS_PFPAG			15 //0b001111
#define DA_STATUS_PEANT			8  //0b001000
#define DA_STATUS_PEANT_AXI		40 //0b101000
#define DA_STATUS_IMPEA			22 //0b010110
#define DA_STATUS_IMPEA_AXI		54 //0b110110
#define DA_STATUS_IMEPE			24 //0b011000
#define DA_STATUS_DEBUG			2  //0b000010



//#define DA_STATUS_L2PPE			0b011110






#endif /* PLATFORM_H_ */
