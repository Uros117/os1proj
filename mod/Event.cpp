/*
 * Event.cpp
 *
 *  Created on: Jun 29, 2020
 *      Author: OS1
 */
#include "Event.h"


Event::Event (IVTNo ivtNo) {
	lock
	myImpl = new KernelEv(ivtNo);
	unlock
}

Event::~Event () {
	lock
	delete myImpl;
	unlock
}

void Event::wait () {
	myImpl->wait();
}

void Event::signal () {
	myImpl->signal();
}



