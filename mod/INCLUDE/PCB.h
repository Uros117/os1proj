/*
 * PCB.h
 *
 *  Created on: Mar 5, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

class PCB {
private:
	static unsigned int idcnt;
public:
	unsigned sp;
	unsigned ss;
	unsigned size;
	unsigned* stack;
	unsigned finished;// Posle moze da bude state(enum)
	const unsigned id;
	int quantum;
	PCB();
	PCB(int size, void (*body)());
	virtual ~PCB();

};

#endif /* PCB_H_ */
