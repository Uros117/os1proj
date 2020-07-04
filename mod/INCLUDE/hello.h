/*
 * hello.h
 *
 *  Created on: Mar 26, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_HELLO_H_
#define INCLUDE_HELLO_H_
#include "PCB.h"
#include "thread.h"

void dispatch();
void suspend();
void exitThread();
void put(PCB* pcb);
void interrupt timer();


class MainThread : public Thread {
public:
	MainThread():Thread(){

	};
protected:
	virtual void run();
};

#endif /* INCLUDE_HELLO_H_ */
