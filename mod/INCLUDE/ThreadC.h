/*
 * ThreadC.h
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_THREADC_H_
#define INCLUDE_THREADC_H_

#include "thread.h"
#include "semaphore.h"
class ThreadC : public Thread {
public:
	static volatile int work;
	ThreadC():Thread(){

	};
protected:
	virtual void run();
};

#endif /* INCLUDE_THREADC_H_ */
