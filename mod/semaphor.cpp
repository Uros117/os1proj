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
	//cout << "W" << endl;
	int res = myImpl->wait(maxTimeToWait);
	unlock
	return res;
}
int Semaphore::signal(int n) {
	lock
	//cout << "S" << endl;
	int res = myImpl->signal(n);
	unlock
	return res;
}
int Semaphore::val () const {
	return myImpl->value();  // Returns the current value of the semaphore
}


