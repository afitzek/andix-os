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
 * interrupt_controller.h
 *
 *  Created on: Jun 2, 2013
 *      Author: andy
 */

#ifndef INTERRUPT_CONTROLLER_H_
#define INTERRUPT_CONTROLLER_H_

#include <common/typedefs.h>
#include <platform/vector_debug.h>
#include <hal.h>

/**
 * \addtogroup irq_ctrl
 *	\{
 */

/**
 * CATCH IRQ in controller
 */
#define IRQ_CATCH (0x1)

/**
 * Release IRQ
 */
#define IRQ_RELEASE (0x2)

/**
 * Get next pending IRQ
 */
#define IRQ_GET_PENDING (0x3)

/**
 * Get min irq value
 */
#define IRQ_GET_MIN_IRQ (0x4)

/**
 * Get max irq value
 */
#define IRQ_GET_MAX_IRQ (0x5)

/**
 * Probe IRQ
 */
#define IRQ_PROBE_IRQ (0x6)

/**
 * Clear interrupt
 */
#define IRQ_CLEAR_INT (0x7)

/**
 * Process all pending interrupts
 */
#define IRQ_DO_PENDING (0x8)

/**
 * Generate Software interrupt
 */
#define IRQ_SW_INT (0x9)

/**
 * Dump IRQ controller information
 */
#define IRQ_DUMP_INFO (0x10)

/**
 * IRQ Handler function pointer
 */
typedef void (*irq_handler)(int irq);

/**
 * initialize IRQ system
 */
void irq_init();

/**
 * register a handler for a specific IRQ
 *
 * @param irq the irq number
 * @param handler The handler function
 * @return 0 on success other on failure
 */
int irq_register_handler(int irq, irq_handler handler);

/**
 * free irq handler from irq
 *
 * @param irq the irq number
 */
void irq_free(int irq);

/**
 * handle a given irq just calls the registered handler function
 *
 * @param irq the irq number
 * @return 0 on success other on failure
 */
int irq_handle(int irq);

/**
 * get pending irq
 *
 * @return irq number -1 of none available
 */
int irq_get_pending();

/**
 * Clear irq from beeing set
 *
 * @param irq the irq number
 */
void irq_clear(int irq);

/**
 * process all pending IRQs
 */
int irq_do();

/**
 * assert irq via software interrupt
 *
 * @param irq the irq number
 */
void irq_swint(int irq);

/**
 * dump irq controller information
 */
void irq_dump();

/* \} group */

#endif /* INTERRUPT_CONTROLLER_H_ */
