/*
 * loader.h
 *
 *  Created on: Nov 10, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <kprintf.h>
#include <elf.h>
#include <task/task.h>

void start_user(uint8_t* usr, uint32_t size);
task_t* create_user_task(uint8_t* usr, uint32_t size);
task_t* load_linux();
task_t* load_android();
int32_t payload_load_data(void* vdata, void* pdest, uint32_t size);

#endif /* LOADER_H_ */
