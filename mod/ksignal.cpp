/*
 * ksignal.cpp
 *
 *  Created on: Jul 4, 2020
 *      Author: OS1
 */
#include "ksignal.h"
#include "SigList.h"

void KernelSignal::addHandler(SignalHandler handler) {
	lock
	handlers.dodaj(handler);
	unlock
}
void KernelSignal::removeHandler(SignalHandler handler) {
	lock
	handlers.brisi(handler);
	unlock
}
void KernelSignal::removeAllHandlers() {
	lock
	handlers.brisiSve();
	unlock
}
void KernelSignal::update() {
	lock
	if (blocked) {
		unlock
		return;
	}
	for (SigList::ElemSig* t = handlers.head; t; t = t->next) {
		unlock_asm // odblokiraj prekide
		t->info();
		lock_asm // blokiraj prekide
	}

	unlock
}
void KernelSignal::block() {
	lock
	blocked = 1;
	unlock
}
void KernelSignal::unblock() {
	lock
	blocked = 0;
	unlock
}
void KernelSignal::swap(SignalHandler hand1, SignalHandler hand2) {
	lock
	if (hand1 == hand2) {
		unlock
		return;
	}
	SigList::ElemSig* t1;
	int found1 =  0;
	for (t1 = handlers.head; t1; t1 = t1->next) {
		if (t1->info == hand1) {
			found1 = 1;
			break;
		}
	}
	if (found1 == 0) {
		unlock
		return;
	}

	SigList::ElemSig* t2;
	int found2 =  0;
	for (t2 = handlers.head; t2; t2 = t2->next) {
		if (t2->info == hand1) {
			found2 = 1;
			break;
		}
	}
	if (found2 == 0) {
		unlock
		return;
	}

	t1->info = hand2;
	t2->info = hand1;

	unlock
}


