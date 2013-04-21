/*
 * andixservice.c
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tz_application_mod/andixtee.h>
#include <linux/mutex.h>

TZ_CTLR_SPACE ctrlspace;
int pending;
mutex ctlr_mutex;

/**
 * Polls the ctlr_space from com mem to userspace
 *
 * CTRL_PENDING_FROM_S -> CTRL_PENDING_IN_NS
 */
int poll_ctrl_task_to_ns(TZ_CTLR_SPACE* userspace) {

	int result = 0;

	mutex_lock(&ctrl_mutex);

	if (pending == CTRL_PENDING_FROM_S) {
		result = 1;
		copy_to_user(userspace, &ctrlspace, sizeof(TZ_CTLR_SPACE));
		pending = CTRL_PENDING_IN_NS;
	}

	mutex_unlock(&ctrl_mutex);

	return result;
}

/**
 * Pushes the ctlr_space from userspace to comm mem
 *
 * CTRL_PENDING_IN_NS -> CTRL_RESPONSE_FROM_NS
 */
int push_ctrl_task_from_ns(TZ_CTLR_SPACE* userspace) {

	int result = 0;

	mutex_lock(&ctrl_mutex);

	if (pending == CTRL_PENDING_IN_NS) {
		result = 1;
		copy_from_user(&ctrlspace, userspace, sizeof(TZ_CTLR_SPACE));
		pending = CTRL_RESPONSE_FROM_NS;
	}

	mutex_unlock(&ctrl_mutex);

	return result;
}

/**
 * Pushes the ctlr_space from com mem to ctrlspace
 *
 * CTRL_IDLE -> CTRL_PENDING_FROM_S
 */
int push_ctrl_task_from_s(TZ_CTLR_SPACE* comspace) {
	int result = 0;

	mutex_lock(&ctrl_mutex);

	if (pending == CTRL_IDLE) {
		result = 1;
		memcpy(&ctrlspace, comspace, sizeof(TZ_CTLR_SPACE));
		pending = CTRL_PENDING_FROM_S;
	}

	mutex_unlock(&ctrl_mutex);

	return result;
}

/**
 * Polls the ctlr_space from ctrl_space to com mem
 *
 * CTRL_RESPONSE_FROM_NS -> CTRL_IDLE
 */
int poll_ctrl_task_to_s(TZ_CTLR_SPACE* comspace) {

	int result = 0;

	mutex_lock(&ctrl_mutex);

	if (pending == CTRL_RESPONSE_FROM_NS) {
		result = 1;
		memcpy(comspace, &ctrlspace, sizeof(TZ_CTLR_SPACE));
		pending = CTRL_IDLE;
	}

	mutex_unlock(&ctrl_mutex);

	return result;
}

int register_tee_mem_in_tz(TZ_MAIN_COM *com) {
	uint32_t paddr = virt_to_phys((void*) com);
	uint32_t res = 0;
	printk(KERN_INFO "Registering com memory virtual 0x%x physical 0x%x",
			(unsigned int) com, paddr);
	res = __smc_1(SMC_REGISTER_TMEM, paddr);
	return ((int) res);
}

int unregister_tee_mem_from_tz() {
	uint32_t res = 0;
	printk(KERN_INFO "Unregistering com memory");
	res = __smc_1(SMC_UNREGISTER_TMEM, 0);
	return ((int) res);
}
