/*
 * SemList.h
 *
 *  Created on: Apr 20, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_SEMLIST_H_
#define INCLUDE_SEMLIST_H_
#include "macros.h"
#include <iostream.h>

class KernelSem;

class SemList{
public:
	struct ElemSem {
		KernelSem* info;
		ElemSem* next;
		ElemSem(KernelSem* info) : info(info), next(NULL) {};
	};
private:
	volatile ElemSem* head;
	volatile ElemSem* tail;
public:

	SemList();
	~SemList();

	void dodaj(KernelSem* ob);
	void brisiSve();
	int getCount() const;
	void brisi(KernelSem* adr);
	void update();
	//KernelSem* getByID(ID id);
};


#endif /* INCLUDE_SEMLIST_H_ */
