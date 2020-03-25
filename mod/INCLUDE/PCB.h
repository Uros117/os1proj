/*
 * PCB.h
 *
 *  Created on: Mar 5, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "macros.h"
#include "thread.h"

class PCB {
private:
	friend Thread;
	static ID idcnt;
public:
	unsigned sp;
	unsigned ss;
	StackSize size;
	unsigned* stack;
	unsigned finished;// Posle moze da bude state(enum)
	ID id;
	Time quantum;
	PCB();
	PCB(int size, void (*body)());
	PCB(StackSize size, Time quantum, void (Thread::*body)(), void (Thread::*exitFun)());
	virtual ~PCB();

};

#endif /* PCB_H_ */
