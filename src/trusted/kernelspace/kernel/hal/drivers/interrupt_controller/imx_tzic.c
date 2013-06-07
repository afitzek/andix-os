/*
 * imx_tzic.c
 *
 *  Created on: May 27, 2013
 *      Author: andy
 */

#include <drivers/interrupt_controller/imx_tzic.h>
#include <devices/interrupt_controller/interrupt_controller.h>
#include <drivers/drivers.h>

void imx_tzic_disable(imx_tzic_t* tzic) {
	__raw_writel(0, &tzic->intctrl);
}

void imx_tzic_enable(imx_tzic_t* tzic) {
	__raw_writel(1, &tzic->intctrl);
}

int imx_tzic_probe(platform_device_t *dev) {
	int32_t i = 0;
	hal_map_io_mem(dev);

	imx_tzic_t* tzic = (imx_tzic_t*) dev->vbase;
	dev->device_data = (uintptr_t) tzic;

	// all interrupts are non secure
	for (i = 0; i < 4; i++) {
		__raw_writel(0xFFFFFFFF, &tzic->intsec[i]);
	}

	__raw_writel(0x4F, &tzic->priomask);

	// set priorities for all interrupts
	for (i = 0; i < 32; i++) {
		__raw_writel(0x80808080, &tzic->priority[i]);
	}

	// all interrupts are non secure
	for (i = 0; i < 4; i++) {
		__raw_writel(0xFFFFFFFF, &tzic->srcclear[i]);
	}

	for (i = 0; i < 4; i++) {
		hal_info("Wakeup: 0x%x", __raw_readl(&tzic->wakeup_config[i]));
	}

	imx_tzic_enable(tzic);

	return (HAL_SUCCESS);
}

/**
 * return 0 if irq is not secure
 * 1 if irq is secure
 * -1 on error
 */
int imx_tzic_probe_secure(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		if (CHECK_BIT(__raw_readl(&tzic->intsec[reg]), off) == 0) {
			// not set => secure
			return (1);
		} else {
			return (0);
		}
	}
	hal_error("irq probe failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

/**
 * return 0 if irq is disabled
 * 1 if irq is enabled
 * -1 on error
 */
int imx_tzic_probe_enable(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		if (CHECK_BIT(__raw_readl(&tzic->enset[reg]), off) == 0) {
			// not set => disabled
			return (0);
		} else {
			return (1);
		}
	}
	hal_error("irq probe failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

int imx_tzic_set_secure(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	int prioreg = irq / 4;
	int priooff = irq % 4;
	uint32_t prio = 0;
	uint8_t* prios = &prio;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		__raw_writel(__raw_readl(&tzic->intsec[reg]) & ~(1 << off),
				&tzic->intsec[reg]);
		prio = __raw_readl(&tzic->priority[prioreg]);
		prios[priooff] = 0x10;
		__raw_writel(prio, &tzic->priority[prioreg]);
		return (0);
	}
	hal_error("irq set enable failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

int imx_tzic_unset_secure(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		__raw_writel(__raw_readl(&tzic->intsec[reg]) | (1 << off),
				&tzic->intsec[reg]);
		return (0);
	}
	hal_error("irq unset enable failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

int imx_tzic_set_enable(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		__raw_writel(__raw_readl(&tzic->enset[reg]) | (1 << off),
				&tzic->enset[reg]);
		return (0);
	}
	hal_error("irq set enable failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

int imx_tzic_unset_enable(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		__raw_writel(__raw_readl(&tzic->enset[reg]) & ~(1 << off),
				&tzic->enset[reg]);
		return (0);
	}
	hal_error("irq unset enable failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

int imx_tzic_clear_irq(imx_tzic_t* tzic, int irq) {
	int reg = irq / 32;
	int off = irq % 32;
	if (reg >= 0 && reg < 4 && off >= 0 && off < 32) {
		__raw_writel((1 << off), &tzic->srcclear[reg]);
		return (0);
	}
	hal_error("irq set enable failed: IRQ: %d Reg: %d Off: %d", irq, reg, off);
	return (-1);
}

/**
 * -1 none pending ...
 */
int imx_tzic_get_pending(imx_tzic_t* tzic) {
	int idx = 0;
	int odx = 0;
	int irq = 0;

	for (idx = 0; idx < 4; idx++) {
		if ((__raw_readl(&tzic->hipending[idx])
				& __raw_readl(&tzic->intsec[idx])) != 0) {
			for (odx = 0; odx < 32; odx++) {
				if (CHECK_BIT(__raw_readl(&tzic->hipending[idx]), odx) != 0) {
					irq = 32 * idx + odx;
					return (irq);
				}
			}
		}
	}

	for (idx = 0; idx < 4; idx++) {
		if ((__raw_readl(&tzic->pending[idx]) & __raw_readl(&tzic->intsec[idx]))
				!= 0) {
			for (odx = 0; odx < 32; odx++) {
				if (CHECK_BIT(__raw_readl(&tzic->pending[idx]), odx) != 0) {
					irq = 32 * idx + odx;
					return (irq);
				}
			}
		}
	}

	return (-1);
}

int imx_tzic_do_pending(imx_tzic_t* tzic) {
	int idx = 0;
	int odx = 0;
	int irq = 0;

	for (idx = 0; idx < 4; idx++) {
		if ((__raw_readl(&tzic->hipending[idx])
				& ~__raw_readl(&tzic->intsec[idx])) != 0) {
			for (odx = 0; odx < 32; odx++) {
				if (CHECK_BIT(__raw_readl(&tzic->hipending[idx]),
						odx) != 0) {
					irq = 32 * idx + odx;
					irq_handle(irq);
				}
			}
		}
	}

	for (idx = 0; idx < 4; idx++) {
		if ((__raw_readl(&tzic->pending[idx]) & ~__raw_readl(&tzic->intsec[idx]))
				!= 0) {
			for (odx = 0; odx < 32; odx++) {
				if (CHECK_BIT(__raw_readl(&tzic->pending[idx]), odx) != 0) {
					irq = 32 * idx + odx;
					irq_handle(irq);
				}
			}
		}
	}

	return (0);
}

void imx_tzic_swint(int irq, imx_tzic_t* tzic) {
	if (0 > irq || irq > 128) {
		hal_error("IRQ out of range! %d", irq);
		return;
	}

	__raw_writel(0x1, &(tzic->dsmint));
	__raw_writel(irq, &(tzic->swint));
}

uint32_t imx_tzic_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	int idx = 0;
	imx_tzic_t* tzic = NULL;

	if (dev == NULL ) {
		hal_error("device dev is NULL");
		return (HAL_E_INVALID_DEV);
	}

	tzic = (imx_tzic_t*) dev->device_data;

	if (tzic == NULL ) {
		hal_error("device tzic is NULL");
		return (HAL_E_INVALID_DEV);
	}

	switch (request) {
	case IRQ_CATCH:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		imx_tzic_disable(tzic);
		if (imx_tzic_set_secure(tzic, (int) (*param)) != 0) {
			hal_error("INVALID IRQ");
			imx_tzic_enable(tzic);
			return (HAL_E_DEV_ERROR);
		}
		if (imx_tzic_set_enable(tzic, (int) (*param)) != 0) {
			imx_tzic_unset_secure(tzic, (int) (*param));
			hal_error("INVALID IRQ");
			imx_tzic_enable(tzic);
			return (HAL_E_DEV_ERROR);
		}
		imx_tzic_enable(tzic);
		break;
	case IRQ_RELEASE:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		imx_tzic_disable(tzic);
		if (imx_tzic_unset_secure(tzic, (int) (*param)) != 0) {
			hal_error("INVALID IRQ");
			imx_tzic_enable(tzic);
			return (HAL_E_DEV_ERROR);
		}
		if (imx_tzic_unset_enable(tzic, (int) (*param)) != 0) {
			imx_tzic_set_secure(tzic, (int) (*param));
			hal_error("INVALID IRQ");
			imx_tzic_enable(tzic);
			return (HAL_E_DEV_ERROR);
		}
		break;
	case IRQ_GET_MAX_IRQ:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		(*param) = 128;
		break;
	case IRQ_GET_MIN_IRQ:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		(*param) = 0;
		break;
	case IRQ_GET_PENDING:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		(*param) = imx_tzic_get_pending(tzic);
		break;
	case IRQ_PROBE_IRQ:
		return (HAL_E_FUNC_NOT_AVAIL);
		break;
	case IRQ_CLEAR_INT:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		imx_tzic_clear_irq(tzic, (int) (*param));
		break;
	case IRQ_DO_PENDING:
		imx_tzic_do_pending(tzic);
		break;
	case IRQ_SW_INT:
		if (param == NULL ) {
			hal_error("Parameter is NULL");
			return (HAL_E_IOCTL_PARA_INVALID);
		}
		imx_tzic_swint((int) (*param), tzic);
		break;
	case IRQ_DUMP_INFO:
		hal_info("TZIC DUMP");
		hal_info("INTCTRL: 0x%x", __raw_readl(&tzic->intctrl));
		hal_info("INTTYPE: 0x%x", __raw_readl(&tzic->inttype));
		hal_info("PRIOMASK: 0x%x", __raw_readl(&tzic->priomask));
		for (idx = 0; idx < 4; idx++) {
			hal_info("ENSET[%d]: 0x%x", idx, __raw_readl(&tzic->enset[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("ENCLR[%d]: 0x%x", idx, __raw_readl(&tzic->enclear[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("INTSEC[%d]: 0x%x", idx, __raw_readl(&tzic->intsec[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("PENDING[%d]: 0x%x", idx,
					__raw_readl(&tzic->pending[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("HPENDING[%d]: 0x%x", idx,
					__raw_readl(&tzic->hipending[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("SRCSET[%d]: 0x%x", idx, __raw_readl(&tzic->srcset[idx]));
		}
		for (idx = 0; idx < 4; idx++) {
			hal_info("SRCCLR[%d]: 0x%x", idx,
					__raw_readl(&tzic->srcclear[idx]));
		}
		for (idx = 0; idx < 32; idx++) {
			hal_info("PRIORITY[%d]: 0x%x", idx,
					__raw_readl(&tzic->priority[idx]));
		}
		hal_info("TZIC DUMP");
		break;
	default:
		return (HAL_E_IOCTL_REQ_NOT_AVAIL);
	}

	return HAL_SUCCESS;
}

static const platform_driver_t imx_tzic_driver = { .probe = imx_tzic_probe,
		.release = NULL, .write = NULL, .read = NULL, .ioctl = imx_tzic_ioctl,
		.driver = { .name = IMX53_TZIC_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_tzic_driver)
