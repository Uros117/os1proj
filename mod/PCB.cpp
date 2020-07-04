#include "hello.h"
#include <iostream.h>
#include "PCB.h"
#include "macros.h"
#include "thread.h"


volatile ID PCB::idcnt = 1;

PCB::PCB() : stack(NULL), size(0), sp(0), ss(0), quantum(20), finished(0), id(idcnt), threadPointer(NULL), lastTimeUnbockedMsg(-1), lockFlag(0) {
	lock
	idcnt ++;
	unlock
}

PCB::PCB(StackSize size, Time quantum, void(*body)()) : size(size), quantum(quantum), finished(0), id(idcnt), threadPointer(NULL), lastTimeUnbockedMsg(-1), lockFlag(0) {
	lock
	idcnt ++;
	stack = new unsigned [size];

	// Postavljanje PSW-a
	stack[size - 1] = 0x200;


	//sp = FP_OFF(stack);
	//ss = FP_SEG(stack);

	unsigned int newPC = FP_OFF(body);
	unsigned int newCS = FP_SEG(body);
	stack[size - 2] = newCS;
	stack[size - 3] = newPC;

	sp = FP_OFF(stack + size - 12);
	ss = FP_SEG(stack + size - 12);

	bp = FP_OFF(stack + size - 12);

	finished = 0;
	unlock
}

PCB::PCB(StackSize size, Time quantum, Thread* thread) : size(size), quantum(quantum), finished(0), id(idcnt), threadPointer(NULL), lastTimeUnbockedMsg(-1), lockFlag(0) {
	lock
	idcnt ++;
	stack = new unsigned [size];

	// Postavljanje PSW-a
	stack[size - 1] = 0x200;

	unsigned int newPC = FP_OFF(&PCB::wrapper);
	unsigned int newCS = FP_SEG(&PCB::wrapper);
	stack[size - 2] = newCS;
	stack[size - 3] = newPC;

	sp = FP_OFF(stack + size - 12);
	ss = FP_SEG(stack + size - 12);

	bp = FP_OFF(stack + size - 12);
	//stack[size - 12] = bp;// Ja sam dodao jer se popuje bp

	finished = 0;
	this->threadPointer = thread;
	unlock
}
/*
lock
idcnt ++;
stack = new unsigned [size];
sp = FP_OFF(stack);
ss = FP_SEG(stack);

unsigned int newPC = FP_OFF(&entryPoint);
unsigned int newCS = FP_SEG(&entryPoint);
//stack[size - 1] = 512;
stack[size - 2] = newCS;
stack[size - 3] = newPC;
//stack[size - 13] = 512;
sp = FP_OFF(stack + size - 12);// Bilo 13 zbog novog lock unlock
unlock*/

PCB::~PCB() {
	lock
	if (stack != NULL)
		delete[] (void*)stack;
	unlock
	/*size = 0;
	sp = 0;
	ss = 0;
	quantum = 20;
	finished = 0;*/
}

void PCB::wrapper(){
	running->threadPointer->run();

	lock
	PCB::running->threadPointer->signal(2);
	if (PCB::running->threadPointer->parentThread) {
		PCB::running->threadPointer->parentThread->signal(1);
	}


#ifdef DEBUG
	cout << "THREAD FINNISHED" << endl;
#endif
	running->threadPointer->waitList->putAll();
	unlock
	exitThread();
};

