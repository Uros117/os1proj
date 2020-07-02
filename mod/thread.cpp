#include <iostream.h>
#include "thread.h"
#include "macros.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "hello.h"
#include "PCBList.h"

volatile ID Thread::idcnt = 1;

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

	/*int i;
	int j;
	lock
	j = Thread::lista.getCount();
	for(i = 0; i < j; i++){
		if(lista[i].getId() == getId())
			lista(i);
	}
	unlock*/
	//delete myPCB;
}
