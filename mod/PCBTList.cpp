/*
 * PCBTList.cpp
 *
 *  Created on: Apr 20, 2020
 *      Author: OS1
 */
#include <iostream.h>
#include "macros.h"
#include "PCB.h"
#include "PCBTList.h"
#include "SCHEDULE.H"

void PCBTList::dodaj(PCB* ob, Time t)
{
	lock
	ElemPCBT* e = new ElemPCBT(ob, t);
	if (!head) head = e;
	if (!tail) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}
	unlock
}

void PCBTList::brisiSve()
{
	lock
	ElemPCBT* n;
	for (ElemPCBT* t = head; t; t = n)
	{
		n = t->next;
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int PCBTList::getCount() const
{
	lock
	int rez = 0;
	for (ElemPCBT* t = head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void PCBTList::brisi(ID id)
{
	lock
	ElemPCBT* n = NULL;

	for (ElemPCBT* t = head; t; t = t->next)
	{
		if (t->info->id == id)
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

	if (head == NULL)
		tail == NULL;
	unlock
}

PCB* PCBTList::getByID(ID id)
{
	lock
	for (ElemPCBT* t = head; t; t = t->next)
	{
		if (t->info->id == id)
		{
			unlock
			return t->info;
		}
	}
	unlock
	return NULL;
}

PCBTList::~PCBTList() {
	lock
	brisiSve();
	unlock
}







