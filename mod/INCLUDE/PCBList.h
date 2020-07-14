/*
 * PCBList.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_PCBLIST_H_
#define INCLUDE_PCBLIST_H_
#include "macros.h"
#include <iostream.h>

class PCB ;

class PCBList{
public:
	struct ElemPCB {
		PCB* info;
		ElemPCB* next;
		ElemPCB(PCB* info) : info(info), next(NULL) {};
	};
private:
	volatile ElemPCB* head;
	volatile ElemPCB* tail;
public:

	PCBList() : head(NULL), tail(NULL) {};
	~PCBList();

	void dodaj(PCB* ob);
	void brisiSve();
	int getCount() const;
	void brisi(ID id);
	PCB* getByID(ID id);
	void putAll();
};


#endif /* INCLUDE_PCBLIST_H_ */
