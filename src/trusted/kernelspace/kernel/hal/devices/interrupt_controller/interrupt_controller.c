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
 * interrupt_controller.c
 *
 *  Created on: Jun 2, 2013
 *      Author: andy
 */

#include <devices/interrupt_controller/interrupt_controller.h>
#include <devices/devices.h>
#include <kprintf.h>
#include <mm/mm.h>

irq_handler* handler_table;
int32_t max_irq;
int32_t min_irq;
platform_device_t* irq_ctrl = NULL;

void irq_init() {
	irq_ctrl = NULL;
	handler_table = NULL;
	int32_t idx = 0;
	irq_ctrl = hal_find_device(INTERRUPT_CTRL_DEVICE, 0);

	if (irq_ctrl != NULL ) {
		if (hal_ioctl(irq_ctrl, IRQ_GET_MAX_IRQ, (uintptr_t) &max_irq,
				sizeof(max_irq)) != HAL_SUCCESS) {
			irq_error(
					"Failed to determine max interrupt. Interrupts not supported!");
			goto reset;
		}

		if (hal_ioctl(irq_ctrl, IRQ_GET_MIN_IRQ, (uintptr_t) &min_irq,
				sizeof(min_irq)) != HAL_SUCCESS) {
			irq_error(
					"Failed to determine min interrupt. Interrupts not supported!");
			goto reset;
		}

		if (max_irq < 0 || min_irq < 0) {
			irq_error("Invalid irq range detected. Interrupts not supported!");
			goto reset;
		}

		irq_info("IRQ %d - %d", min_irq, max_irq);

		handler_table = (irq_handler*) kmalloc(max_irq * sizeof(irq_handler*));

		if (handler_table == NULL ) {
			irq_error(
					"Failed to allocate irq handler table. Interrupts not supported!");
			goto reset;
		}

		for (idx = min_irq; idx < max_irq; idx++) {
			handler_table[idx] = NULL;
		}

		irq_info("IRQ system setup. OK");
		return;
	}
	irq_error("No interrupt controller available. Interrupts not supported!");
	reset: max_irq = 0;
	min_irq = 0;
	irq_ctrl = NULL;
}

int irq_register_handler(int irq, irq_handler handler) {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return (-1);
	}

	if (min_irq > irq || irq > max_irq) {
		irq_error("IRQ out of range!");
		return (-1);
	}

	if (handler_table[irq] != NULL ) {
		irq_info("Handler for IRQ already registered!");
		return (-1);
	}

	if (handler == NULL ) {
		irq_info("IRQ Handler is NULL!");
		return (-1);
	}

	if (hal_ioctl(irq_ctrl, IRQ_CATCH, (uintptr_t) &irq,
			sizeof(irq)) != HAL_SUCCESS) {
		irq_info("FAILED to register IRQ at interrupt controller!");
		return (-1);
	}

	handler_table[irq] = handler;

	irq_info("Registered %s for IRQ %d", get_function_name((void*)handler),
			irq);

	return (0);
}

void irq_free(int irq) {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return;
	}

	if (min_irq > irq || irq > max_irq) {
		irq_error("IRQ out of range!");
		return;
	}

	if (handler_table[irq] == NULL ) {
		irq_info("No Handler for IRQ registered!");
		return;
	}

	if (hal_ioctl(irq_ctrl, IRQ_RELEASE, (uintptr_t) &irq,
			sizeof(irq)) != HAL_SUCCESS) {
		irq_info("FAILED to release IRQ at interrupt controller!");
		return;
	}

	handler_table[irq] = NULL;
}

int irq_handle(int irq) {
	irq_info("Handling IRQ: %d", irq);

	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return (-2);
	}

	if (min_irq > irq || irq > max_irq) {
		irq_error("IRQ out of range!");
		return (-2);
	}

	if (handler_table[irq] == NULL ) {
		irq_error("No Handler for IRQ registered!");
		return (-2);
	}

	handler_table[irq](irq);

	return (0);
}

int irq_do() {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return (-2);
	}

	if (hal_ioctl(irq_ctrl, IRQ_DO_PENDING, (uintptr_t) NULL, 0) != HAL_SUCCESS) {
		irq_info("FAILED to process pending IRQs at interrupt controller!");
		return (-1);
	}
	return (0);
}

int irq_get_pending() {
	int irq = 0;

	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return (-1);
	}

	if (hal_ioctl(irq_ctrl, IRQ_GET_PENDING, (uintptr_t) &irq,
			sizeof(irq)) != HAL_SUCCESS) {
		irq_info("FAILED to clear interrupt!");
		return (-1);
	}

	return (irq);
}

void irq_clear(int irq) {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return;
	}

	if (hal_ioctl(irq_ctrl, IRQ_CLEAR_INT, (uintptr_t) &irq,
			sizeof(irq)) != HAL_SUCCESS) {
		irq_info("FAILED to clear interrupt!");
		return;
	}
}

void irq_swint(int irq) {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return;
	}

	if (hal_ioctl(irq_ctrl, IRQ_SW_INT, (uintptr_t) &irq,
			sizeof(irq)) != HAL_SUCCESS) {
		irq_info("FAILED to generate interrupt!");
		return;
	}
}

void irq_dump() {
	if (irq_ctrl == NULL ) {
		irq_error("Interrupts not supported!");
		return;
	}

	hal_ioctl(irq_ctrl, IRQ_DUMP_INFO, NULL, 0);
}

