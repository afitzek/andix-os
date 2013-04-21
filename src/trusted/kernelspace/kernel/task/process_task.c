/*
 * process_task.c
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common.h>
#include <task/task.h>
#include <communication_types.h>
#include <fs/fs.h>
#include <scheduler.h>

void process_entry() {
	//task_t* nonsec = get_nonsecure_task();
	TZ_TEE_SPACE tee_space;
	while (1) {
		TZ_MAIN_COM* ctrl = mon_get_control_space();

		if (ctrl != NULL ) {
			ctrl->p.tee
			//mon_info("Control Structur OP: %d", ctrl->op);
			/*if (ctrl->op == TZ_CTRL_OP_IDLE) {
				get_current_task()->state = BLOCKED;
			}*/
			/*if (ctrl->op == TZ_CTRL_OP_IDLE) {
				cfs_t file;
				uint8_t sample_buf[100];
				uint8_t sample_buf2[100];
				memset(sample_buf, 0, 100);
				mon_info("!!!!!Creating file!!!!!");
				if (fs_open((uint8_t*)"MASTER_DIR", strlen("MASTER_DIR"), (uint8_t*)"MASTER_FILE",
						strlen("MASTER_FILE"), O_CREAT, 0, &file) == 0) {

					mon_info("!!!!!WRITING file!!!!!");

					strncpy((char*)sample_buf, "HELLO FILE!!", 100);

					//sha2(sample_buf, 100, digest, 0);

					fs_write(&file, sample_buf, 100);
					mon_info("!!!!!SEEK file!!!!!");

					fs_lseek(&file, 0, REE_SEEK_SET);
					mon_info("!!!!!READING file!!!!!");

					memset(sample_buf2, 0, 100);

					fs_read(&file, sample_buf2, 100);

					//sha2(sample_buf, 100, digest2, 0);

					if (memcmp(sample_buf, sample_buf2, 32) == 0) {
						mon_info("Result matches!");
					}

					mon_info("sample_buf: %s", sample_buf);
					kprintHex(sample_buf, 32);
					kprintf("\n");
					mon_info("sample_buf2: %s", sample_buf2);
					kprintHex(sample_buf2, 32);
					kprintf("\n");
				}

				ctrl->op = TZ_CTRL_OP_DEXIT;
			}*/
		}
		//print_tasks();

		yield();
	}
}
