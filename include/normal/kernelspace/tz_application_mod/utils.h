/*
 * utils.h
 *
 *  Created on: Jan 8, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <tz_application_mod/types.h>

void* v_to_p(void* ptr);

int ipow(int base, int exp);
int mmap_comm_mem(struct file *filp, struct vm_area_struct *vma, TZ_TEE_SPACE* com);

#endif /* UTILS_H_ */
