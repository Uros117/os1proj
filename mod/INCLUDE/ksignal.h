/*
 * ksignal.h
 *
 *  Created on: Jul 2, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_KSIGNAL_H_
#define INCLUDE_KSIGNAL_H_
#include "macros.h"
#include "SigList.h"
class KernelSignal {
public:
	int blocked;
	SigList handlers;
	KernelSignal() : blocked(0) {};

	void addHandler(SignalHandler handler);
	void removeHandler(SignalHandler handler);
	void removeAllHandlers();
	void update();
	void block(); // postavlja blocked na true
	void unblock();
	void swap(SignalHandler hand1, SignalHandler hand2);

};

#endif /* INCLUDE_KSIGNAL_H_ */
