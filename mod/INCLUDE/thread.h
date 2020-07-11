// File: thread.h
#ifndef _thread_h_
#define _thread_h_
#include <iostream.h>
#include "macros.h"
#include "TList.h"
#include "PCBList.h"
#include "PCB.h"
#include "ksignal.h"
#include "SIdList.h"

extern void dispatch();

void signal0handler ();

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
	// SIGNALI
	void signal (SignalId signal);

	void registerHandler (SignalId signal, SignalHandler handler);
	void unregisterAllHandlers (SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal (SignalId signal);
	static void blockSignalGlobally (SignalId signal);
	void unblockSignal (SignalId signal);
	static void unblockSignalGlobally (SignalId signal);

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
	static volatile TList threadLista;

	friend void interrupt timer();
	friend void signal0handler();

	// SIGNALI
	KernelSignal signals[16];
	static volatile int glob_blocked[16];
	SIdList signalQueue;
	volatile Thread* parentThread;
};

#endif
