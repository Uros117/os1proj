/*
 * semaphor.cpp
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */
#include "semaphor.h"
#include "macros.h"
#include "ksem.h"
Semaphore::Semaphore (int init) {
	lock
	myImpl = new KernelSem(init);
	unlock
}

Semaphore::~Semaphore() {
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait (Time maxTimeToWait) {
	lock
	//cout << "WAIT" << endl;
	myImpl->wait(maxTimeToWait);
	unlock
}
int Semaphore::signal(int n) {
	lock
	//cout << "SIGNAL" << endl;
	myImpl->signal(n);
	unlock
}
int Semaphore::val () const {
	return myImpl->value();  // Returns the current value of the semaphore
}


