// File: thread.h
#ifndef _thread_h_
#define _thread_h_
#include <iostream.h>
#include "macros.h"
#include "TList.h"
#include "PCBList.h"

static volatile TList threadLista;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id){
		if(id == 0) return NULL;// ID 0 je rezervisan za greske
		return threadLista.getByID(id);
	}

protected:
	friend class PCB;
	friend void entryPoint();

	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {};
private:
	volatile ID id;
	volatile static ID idcnt;
	volatile int allreadyStarted;
	volatile PCB* myPCB;
	volatile PCBList* waitList;
};

#endif
