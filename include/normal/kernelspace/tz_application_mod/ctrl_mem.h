/*
 * ctrl_mem.h
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#ifndef CTRL_MEM_H_
#define CTRL_MEM_H_

#include <tz_application_mod/andixtee.h>

int poll_ctrl_task_to_ns(TZ_CTLR_SPACE* userspace);
int push_ctrl_task_from_ns(TZ_CTLR_SPACE* userspace);
int push_ctrl_task_from_s(void);
int poll_ctrl_task_to_s(void);
int check_process_is_root(void);
int register_ctrl_mem_in_tz(TZ_CTLR_SPACE *com);
int unregister_ctrl_mem_from_tz(void);
void cleanup_ctrl(void);
int initialize_ctrl(void);

#endif /* CTRL_MEM_H_ */
