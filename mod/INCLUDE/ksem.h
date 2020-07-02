/*
 * ksem.h
 *
 *  Created on: Apr 16, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_KSEM_H_
#define INCLUDE_KSEM_H_
#include "PCBList.h"
#include "PCBTList.h"
#include "SemList.h"

// Ako je vrednost semafora nula proces koji je izvriso operaciju wait
// mora da ceka da neki drugi proces odradi operaciju signal

// Moguca implementacija semafora:
// 1) val>0: jos val proces moze da izvrsi operaciju wait a da se ne blokira
// 2) val=0: nema blokiranih na semaforu, ali ce se proces koji naredni izvrsi wait blokirati
// 3) val<0: ima -val blokiranih proces, wait izaziva blokiranje

class KernelSem{
public:
	KernelSem(unsigned short init = 1);
	virtual ~KernelSem();
	// wait(S)
	// Ako je vrednost semafora S veca oid nule,
	// ta vrednost se umanjuje za jedan: u suprotnom,
	// proces mora da ceka sve dok S ne postane vece
	// od nule, a tada se vrednost takodje umanjuje
	// za jedan
	virtual int wait(Time maxTimeToWait);
	// signal(S)
	// Vrednost semafora se uvecava za jedan
	virtual int signal(int n = 0);
	int value() const {return val;}
private:
	friend SemList;
	volatile int val;
	volatile PCBTList timeBlocked;
	volatile PCBTList segBlocked;
public:
	volatile static SemList sem;

};


#endif /* INCLUDE_KSEM_H_ */
