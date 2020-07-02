/*
 * PCBList.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include "macros.h"
#include "PCB.h"
#include "PCBList.h"
#include "SCHEDULE.H"

void PCBList::dodaj(PCB* ob)
{
	lock
	ElemPCB* e = new ElemPCB(ob);
	if (!head) head = e;
	if (!tail) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}
	unlock
}

void PCBList::brisiSve()
{
	lock
	ElemPCB* n;
	for (ElemPCB* t = head; t; t = n)
	{
		n = t->next;
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int PCBList::getCount() const
{
	lock
	int rez = 0;
	for (ElemPCB* t = head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void PCBList::brisi(ID id)
{
	lock
	int cnt = getCount();
	ElemPCB* n = NULL;

	for (ElemPCB* t = head; t; t = t->next)
	{
		if (t->info->id == id)
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

PCB* PCBList::getByID(ID id)
{
	lock
	for (ElemPCB* t = head; t; t = t->next)
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


void PCBList::putAll(){
	lock
	for (ElemPCB* t = head; t; t = t->next)
	{
		Scheduler::put((PCB*)t->info);
	}
	brisiSve();
	unlock
}


PCBList::~PCBList(){
	lock
	brisiSve();
	unlock
}
