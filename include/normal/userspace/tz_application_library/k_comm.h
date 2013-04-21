/*
 * k_comm.h
 *
 *  Created on: Apr 21, 2013
 *      Author: andy
 */

#ifndef K_COMM_H_
#define K_COMM_H_

#include <communication_types.h>

TZ_TEE_SPACE * getCommSpace();
void cleanup();
void mapCommMem();
void openTEEDevice();
void unmapCommMem();
void closeTEEDevice();
void lockComm();
void unlockComm();
int processComm();
void doexit();

#endif /* K_COMM_H_ */
