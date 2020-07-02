#include <iostream.h>
#include "macros.h"
#include "thread.h"
#include "TList.h"


void TList::dodaj(Thread* ob)
{
	lock
	Elem* e = new Elem(ob);
	if (head == NULL) head = e;
	if (tail == NULL) tail = e;
	else
	{
		tail->next = e;
		tail = e;
	}
	unlock
}

void TList::brisiSve()
{
	lock
	Elem* n;
	for (Elem* t = head; t; t = n)
	{
		n = t->next;
		// TODO: Moze doci do problema ako se brisu svi threadovi odmah.
		//delete t->info;

		delete t;
	}
	head = NULL;
	tail = NULL;
	unlock
}

int TList::getCount() const
{
	lock
	int rez = 0;
	for (Elem* t = head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void TList::brisi(ID id)
{
	lock
	int cnt = getCount();
	Elem* n = NULL;

	for (Elem* t = head; t; t = t->next)
	{
		if (t->info->getId() == id)
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

Thread* TList::getByID(ID id)
{
	lock
	for (Elem* t = head; t; t = t->next)
	{
		if (t->info->getId() == id)
		{
			unlock
			return t->info;
		}
	}
	unlock
	return NULL;
}
