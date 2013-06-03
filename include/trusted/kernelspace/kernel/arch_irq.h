/*
 * arch_irq.h
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#ifndef ARCH_IRQ_H_
#define ARCH_IRQ_H_

void enable_irq();

void disable_irq();

void enable_fiq();

void disable_fiq();

uint32_t get_cpsr();

#endif /* ARCH_IRQ_H_ */
