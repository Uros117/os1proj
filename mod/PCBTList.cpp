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
			if (!n)
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
volatile int x = 0;
void PCBTList::update(){
	// Uredjenost ove liste nebi mnogo pomoglo jer moramo da prodjemo
	// kroz nju da bi smanjili vreme koje se ceka na izlaz
	// Optimizacija bi bila u tome sto bi se manje menjali pointeri
	// ali je onda dodavanje mnogo zahtevnije!!!
	lock
	//cout << "update " << ++x << endl;
	for (ElemPCBT* t = head, *p = NULL; t; )
	{
		if(--(t->t) <= 0){
			t->info->lastTimeUnbockedMsg = 0;
			Scheduler::put((PCB*)t->info);
			//cout << "izbacen elem" << endl;
			// Izbaci element
			ElemPCBT* next = t->next;


			if (t->next == NULL)
				tail = p;
			if (p != NULL)
				p->next = t->next;
			else
				head = t->next;

			// P se nece menjati jer samo spustamo elemente za jedan pa
			// past element uvek ostaje isti ako brisemo element
			// i samo se menja ako ne brisemo element tj. idemo na sledeci;
			delete t;
			t = next;
		} else {
			// Propagiraj
			p=t;
			t = t->next;
		}

	}
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







