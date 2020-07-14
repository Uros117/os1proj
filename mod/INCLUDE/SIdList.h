/*
 * SIdSem.h
 *
 *  Created on: Jul 3, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_SIDLIST_H_
#define INCLUDE_SIDLIST_H_


#include "macros.h"
#include <iostream.h>

class SIdList{
public:
	struct ElemSId {
		SignalId info;
		ElemSId* next;
		ElemSId(SignalId info) : info(info), next(NULL) {};
	};
public:

	volatile ElemSId* head;
	volatile ElemSId* tail;
public:

	SIdList() : head(NULL), tail(NULL) {};
	~SIdList();
	SignalId getTop();
	void dodaj(SignalId ob);
	void brisiSve();
	int getCount() const;
	void brisi(SignalId id);
};

#endif /* INCLUDE_SIDLIST_H_ */
