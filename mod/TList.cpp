#include <iostream.h>
#include "macros.h"
#include "thread.h"
#include "TList.h"

void TList::TList ()  : head(NULL), tail(NULL)
{
	/*lock
	cout << "tlist konstruktor" << endl;
	unlock*/
};

void TList::dodaj(Thread* ob)
{
	lock
	//cout << "dodaj" << endl;
	Elem* e = new Elem(ob);
	if (!head) head = e;
	if (!tail) tail = e;
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
	//cout << "brisem sve" << endl;
	Elem* n;
	for (Elem* t = (TList::Elem*)head; t; t = n)
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
	for (Elem* t = (TList::Elem*)head; t; t = t->next, ++rez);
	unlock
	return rez;
}

/*
 * Obrisi Thread sa ThreadID-em : id
 */
void TList::brisi(ID id)
{
	lock
	//cout << "brisi thread " << id << endl;
	Elem* n = NULL;

	for (Elem* t = (TList::Elem*)head; t; t = t->next)
	{
		if (t->info->getId() == id)
		{
			if (t->next == NULL)
				tail = n;
			if (n != NULL)
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
	for (Elem* t = (TList::Elem*)head; t; t = t->next)
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
