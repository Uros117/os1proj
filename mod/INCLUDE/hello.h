/*
 * hello.h
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_HELLO_H_
#define INCLUDE_HELLO_H_
#include "PCB.h"

void dispatch();
void suspend();
void exitThread();
void put(PCB* pcb);
void interrupt timer();


#endif /* INCLUDE_HELLO_H_ */
