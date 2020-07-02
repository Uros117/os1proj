/*
 * SemList.cpp
 *
 *  Created on: Apr 20, 2020
 *      Author: OS1
 */
#include "SemList.h"
#include "ksem.h"
#include <iostream.h>
#include "macros.h"
#include "PCBTList.h"

SemList::SemList() : head(NULL), tail(NULL) {
	//lock
	//cout << "test konstruktor" << endl;
	//unlock
};

void SemList::dodaj(KernelSem* ob)
{
	lock
	//cout << "dodaj" << endl;
	ElemSem* e = new ElemSem(ob);
	if (!head) head = e;
	if (!tail) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}

	unlock
}

void SemList::brisiSve()
{
	lock
	//cout << "semlist" << endl;
	ElemSem* n;
	for (ElemSem* t = (ElemSem*) head; t; t = n)
	{
		n = t->next;
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int SemList::getCount() const
{
	lock
	int rez = 0;
	for (ElemSem* t = (ElemSem*) head; t; t = t->next, ++rez);
	unlock
	return rez;
}


//Obrisi Thread sa ThreadID-em : id

void SemList::brisi(KernelSem* adr)
{
	lock
	//int cnt = getCount();
	ElemSem* n = NULL;

	for (ElemSem* t = (ElemSem*) head; t; t = t->next)
	{
		if (t->info == adr)
		{
			if (t->next == NULL)
				tail = n;
			if (!n)
				n->next = t->next;
			else
				head = t->next;

			// ToDo: Da li dealocirati odmah : delete t->info;
			delete t;
			unlock
			return;
		}
		n = t;
	}
	unlock
}

void SemList::update(){
	lock
	//cout << (ElemSem*) head << endl;
	for (ElemSem* t = (ElemSem*) head; t; t = t->next)
	{
		t->info->timeBlocked.update();
	}
	unlock
}

SemList::~SemList() {
	lock
	brisiSve();
	unlock
}

/*PCB* PCBList::getByID(ID id)
{
	for (ElemPCB* t = head; t; t = t->next)
	{
		if (t->info->id == id)
		{
			return t->info;
		}
	}
	return NULL;
}*/



