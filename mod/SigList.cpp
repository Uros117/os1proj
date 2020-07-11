/*
 * SigList.cpp
 *
 *  Created on: Jul 2, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include "macros.h"
#include "SigList.h"

void SigList::dodaj(SignalHandler ob)
{
	lock
	//cout << "dodaj " << ob << endl;
	ElemSig* e = new ElemSig(ob);
	if (!head) head = e;
	if (!tail) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}
	unlock
}

void SigList::brisiSve()
{
	lock
	ElemSig* n;
	for (ElemSig* t = head; t; t = n)
	{
		n = t->next;
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int SigList::getCount() const
{
	lock
	int rez = 0;
	for (ElemSig* t = head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void SigList::brisi(SignalHandler id)
{
	lock
	int cnt = getCount();
	ElemSig* n = NULL;

	for (ElemSig* t = head; t; t = t->next)
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


SigList::~SigList(){
	lock
	brisiSve();
	unlock
}



