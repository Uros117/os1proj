/*
 * ThreadB.h
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_THREADB_H_
#define INCLUDE_THREADB_H_
#include "thread.h"
#include "semaphore.h"
class ThreadB : public Thread {
public:
	static volatile Semaphore* s;
	static volatile Semaphore* s1;
	ThreadB():Thread(){

	};
protected:
	virtual void run();
};

#endif /* INCLUDE_THREADB_H_ */
