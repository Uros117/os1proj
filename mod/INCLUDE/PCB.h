#ifndef PCB_H_
#define PCB_H_
#include "macros.h"
#include "thread.h"

class PCB {
private:
	friend Thread;
	volatile static ID idcnt;

public:
	// Context switch
	static volatile PCB* running;
	static volatile PCB* idlePCB;

	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;


	volatile StackSize size;
	volatile unsigned* stack;
	volatile unsigned finished;
	volatile Thread* threadPointer;
	volatile ID id;
	volatile Time quantum;

	// Semaphore
	volatile int lastTimeUnbockedMsg;

	//
	volatile int lockFlag;

	PCB();// Konstruktor za MAIN thread
	PCB(StackSize size, Time quantum, void(*body)());// Konstruktor za threadove bez objekta
	PCB(StackSize size, Time quantum, Thread* thread);// konstruktor za threadove
	~PCB();

	static void wrapper();

};

#endif /* PCB_H_ */
