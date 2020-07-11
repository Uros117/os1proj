/*
 * mojTest.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: OS1
 */

#include "th.h"

void signal1handler () {
	lock
	cout << "signal 1" << endl;
	unlock
}

void signal2handler () {
	lock
	cout << "signal 2" << endl;
	unlock
}


class ThreadD:public Thread {
public:
	ThreadD():Thread(){
	};
protected:
	virtual void run();
};

void ThreadD::run() {
	while (1) {

	}
};

Semaphore sd(0);
/*int userMain (int argc, char* argv[]) {
	PCB::running->threadPointer->registerHandler(1, signal1handler);
	PCB::running->threadPointer->registerHandler(2, signal2handler);

	ThreadD* td = new ThreadD();
	td->start();

	sd.wait(100);
	td->signal(0);
	delete td;
	cout << "happy end!" << endl;
	return 0;
}*/



