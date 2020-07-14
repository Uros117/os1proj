/*
 * Semaphore.h
 *
 *  Created on: Apr 16, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_SEMAPHORE_H_
#define INCLUDE_SEMAPHORE_H_
#include "macros.h"

class KernelSem;

class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	int wait (Time maxTimeToWait);
	int signal(int n=0);
	int val () const;
private:
	volatile KernelSem* myImpl;
};

#endif /* INCLUDE_SEMAPHORE_H_ */
