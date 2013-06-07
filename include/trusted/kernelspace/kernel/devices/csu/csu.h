/*
 * csu.h
 *
 *  Created on: Jun 7, 2013
 *      Author: andy
 */

#ifndef CSU_H_
#define CSU_H_

#include <common/typedefs.h>
#include <platform/vector_debug.h>
#include <hal.h>

/**
 * @addtogroup csu
 *	@{
 */

/**
 * IOCTL call to set access permission for device
 */
#define CSU_IOCTL_SET_AP_DEVICE (0x1)

/**
 * IOCTL parameter structure for csu
 */
typedef struct {
	/**
	 * The device id
	 */
	int32_t device_id;

	/**
	 * The access permission
	 */
	int32_t access_permission;
} csu_ioctl_param_t;

#define __CSU_AP_MIN (0x0)
#define __CSU_AP_MAX (0x7)

/**
 * Non-Secure UserMode RD+RW
 * Non-Secure SpvrMode RD+RW
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_RW_K_RW_S_U_RW_K_RW (0x0)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode RD+RW
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_RW_S_U_RW_K_RW (0x1)

/**
 * Non-Secure UserMode RD
 * Non-Secure SpvrMode RD
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_RD_K_RD_S_U_RW_K_RW (0x2)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode RD
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_RD_S_U_RW_K_RW (0x3)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode RD+RW
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_RW_K_RW (0x4)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode NONE
 * Secure SpvrMode RD+RW
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_NO_K_RW (0x5)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode RD
 * Secure SpvrMode RD
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_RD_K_RD (0x6)

/**
 * Non-Secure UserMode NONE
 * Non-Secure SpvrMode NONE
 * Secure UserMode NONE
 * Secure SpvrMode NONE
 */
#define CSU_AP_NS_U_NO_K_NO_S_U_NO_K_NO (0x7)

/**
 * initialize csu device
 */
void csu_init();

/**
 * set access permission for device id
 *
 * @param devID The device id
 * @param ap The access permission (see CSU_AP_*)
 *
 * @return 0 on success other on error
 */
int32_t csu_set_device_permission(int32_t devID, int32_t ap);

/* @} group */

#endif /* CSU_H_ */
