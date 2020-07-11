/*
 * SIdList.cpp
 *
 *  Created on: Jul 3, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include "macros.h"
#include "SIdList.h"

SignalId SIdList::getTop () {
	lock
	if (!head) {
		// Lista je prazna
		unlock
		return SIGNALID_ERROR;
	}
	SignalId t = head->info;
	ElemSId* temp = head;
	head = head->next;
	if (!head)
		tail = NULL;
	delete temp;
	unlock
	return t;
}


void SIdList::dodaj(SignalId ob)
{
	lock
	ElemSId* e = new ElemSId(ob);
	if (!head) head = e;
	if (!tail) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}
	unlock
}

void SIdList::brisiSve()
{
	lock
	ElemSId* n;
	for (ElemSId* t = head; t; t = n)
	{
		n = t->next;
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int SIdList::getCount() const
{
	lock
	int rez = 0;
	for (ElemSId* t = head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void SIdList::brisi(SignalId id)
{
	lock
	int cnt = getCount();
	ElemSId* n = NULL;

	for (ElemSId* t = head; t; t = t->next)
	{
		if (t->info == id)
		{
			if (t->next == NULL)
				tail = n;
			if (n != NULL)
				n->next = t->next;
			else
				head = t->next;

			delete t;
			unlock
			return;
		}
		n = t;
	}
	unlock
}

SIdList::~SIdList(){
	lock
	brisiSve();
	unlock
}


