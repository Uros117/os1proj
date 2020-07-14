/*
 * SigList.h
 *
 *  Created on: Jul 2, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_SIGLIST_H_
#define INCLUDE_SIGLIST_H_

#include "macros.h"
#include <iostream.h>

class SigList{
public:
	struct ElemSig {
		SignalHandler info;
		ElemSig* next;
		ElemSig(SignalHandler info) : info(info), next(NULL) {};
	};
public:
	volatile ElemSig* head;
	volatile ElemSig* tail;
public:

	SigList() : head(NULL), tail(NULL) {};
	~SigList();

	void dodaj(SignalHandler ob);
	void brisiSve();
	int getCount() const;
	void brisi(SignalHandler id);
};


#endif /* INCLUDE_SIGLIST_H_ */
