/*
 * PCBTList.h
 *
 *  Created on: Apr 20, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_PCBTLIST_H_
#define INCLUDE_PCBTLIST_H_

#include "macros.h"
#include <iostream.h>

class PCB ;

class PCBTList{
public:
	struct ElemPCBT {
		PCB* info;
		ElemPCBT* next;
		Time t;
		Time max;
		ElemPCBT(PCB* info, Time t) : info(info), next(NULL), t(t), max(t) {};
	};
private:
	friend class KernelSem;
	ElemPCBT* head;
	ElemPCBT* tail;
public:

	PCBTList() : head(NULL), tail(NULL) {};
	~PCBTList();

	void dodaj(PCB* ob, Time t);
	void brisiSve();
	int getCount() const;
	void brisi(ID id);
	PCB* getByID(ID id);
	void update();
};

#endif /* INCLUDE_PCBTLIST_H_ */
