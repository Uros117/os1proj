#include <iostream.h>
#include "thread.h"
#include "macros.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "hello.h"
#include "PCBList.h"
#include "ksignal.h"

volatile ID Thread::idcnt = 1;
volatile TList Thread::threadLista;
volatile int Thread::glob_blocked[16] = {0};

void Thread::waitToComplete(){
	lock
	if(myPCB->finished == 0){
		waitList->dodaj((PCB*)PCB::running);
		suspend();
	}
	unlock
}


ID Thread::getId() {
	return id;
}
ID Thread::getRunningId() {
	if(PCB::running->threadPointer != NULL)
		return PCB::running->threadPointer->id;
	return 0;// Nula je rezervisan id za gresku
}

// Konstruktor alocira stack i pcb
//
Thread::Thread (StackSize stackSize, Time timeSlice): id(idcnt), allreadyStarted(0), myPCB(NULL){
	lock
	idcnt++;
	waitList = new PCBList();
	myPCB = new PCB(stackSize, timeSlice, (Thread *)this);
	threadLista.dodaj(this);

	parentThread = PCB::running->threadPointer;

	if (parentThread != NULL) {
		for ( int i = 0; i < 16; i++){
			if(parentThread->signals[i].blocked)
				blockSignal(i);
			for (SigList::ElemSig* t = (SigList::ElemSig*)parentThread->signals[i].handlers.head; t; t = t->next) {
				registerHandler(i,t->info);
			}
		}
	} else {
		registerHandler (0, signal0handler);
	}


	unlock
}

void Thread::start(){
	lock
	if(allreadyStarted == 0){
		put((PCB*)myPCB);
#ifdef DEBUG
		cout << "start mypcb id : " << myPCB->id << endl;
#endif
		allreadyStarted = 1;
	}
	unlock
}



Thread::~Thread(){
	waitToComplete();
	lock
#ifdef DEBUG_V
	cout << "Thread delete" << endl;
#endif
	threadLista.brisi(getId());
	delete waitList;

	//FIXME :Problem sa ne brisanjem myPCB-a
	myPCB->finished = 1;
	delete myPCB;
	unlock
}

void Thread::signal (SignalId signal) {
	lock
	if (signal >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signalQueue.dodaj(signal);
	unlock
}

void Thread::registerHandler (SignalId signal, SignalHandler handler) {
	lock
	if (signal >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signals[signal].addHandler(handler);
	unlock
}

void Thread::unregisterAllHandlers (SignalId id) {
	lock
	if (id >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signals[id].removeAllHandlers();
	unlock
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lock
	if (id >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signals[id].swap(hand1, hand2);
	unlock
}

void Thread::blockSignal (SignalId signal){
	lock
	if (signal >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signals[signal].block();
	unlock
}

void Thread::unblockSignal (SignalId signal){
	lock
	if (signal >= SIGNALID_ERROR) {
		unlock
		return;
	}
	signals[signal].unblock();
	unlock
}

void Thread::blockSignalGlobally (SignalId signal) {
	lock
	Thread::glob_blocked[signal] = 1;
	unlock
}

void Thread::unblockSignalGlobally (SignalId signal) {
	lock
	Thread::glob_blocked[signal] = 0;
	unlock
}

void signal0handler () {
	lock
	//cout << "signal0" << endl;
	PCB::running->threadPointer->signal(2);
	if (PCB::running->threadPointer->parentThread)
		PCB::running->threadPointer->parentThread->signal(1);

	PCB::running->threadPointer->waitList->putAll();
	PCB::running->finished = 1;
	unlock
	//exitThread();
}
