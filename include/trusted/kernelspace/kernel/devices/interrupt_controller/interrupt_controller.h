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

#define IRQ_CATCH (0x1)
#define IRQ_RELEASE (0x2)
#define IRQ_GET_PENDING (0x3)
#define IRQ_GET_MIN_IRQ (0x4)
#define IRQ_GET_MAX_IRQ (0x5)
#define IRQ_PROBE_IRQ (0x6)
#define IRQ_CLEAR_INT (0x7)
#define IRQ_DO_PENDING (0x8)
#define IRQ_SW_INT (0x9)
#define IRQ_DUMP_INFO (0x10)

typedef void (*irq_handler)(int irq);

void irq_init();
int irq_register_handler(int irq, irq_handler handler);
void irq_free(int irq);
int irq_handle(int irq);
int irq_get_pending();
void irq_clear(int irq);
int irq_do();
void irq_swint(int irq);
void irq_dump();

#endif /* INTERRUPT_CONTROLLER_H_ */
