/*
 * KernelEv.cpp
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#include "KernelEv.h"
#include "IVTEntry.h"
#include "macros.h"
#include "PCB.h"
KernelEv::KernelEv(IVTNo no) : no(no), pcb((PCB*)PCB::running), sem(0) {
	lock
	IVTEntry::get(no)->myEvent = this;
	unlock
}

KernelEv::~KernelEv() {
	lock
	IVTEntry::get(no)->myEvent = 0;
	unlock
}

void KernelEv::wait() {
	lock
	if (PCB::running == pcb) sem.wait(0);
	unlock
}

void KernelEv::signal()
{
	lock
	sem.signal();
	unlock
}
