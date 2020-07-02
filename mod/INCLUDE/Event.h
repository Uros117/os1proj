/*
 * event.h
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_EVENT_H_
#define INCLUDE_EVENT_H_

#include "macros.h"
#include "IVTEntry.h"

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait  ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

#endif /* INCLUDE_EVENT_H_ */
