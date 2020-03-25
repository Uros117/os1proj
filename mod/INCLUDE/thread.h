// File: thread.h
#ifndef _thread_h_
#define _thread_h_
#include "macros.h"

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}
private:
	void exitThread();
	PCB* myPCB;
};
void dispatch ();
#endif
