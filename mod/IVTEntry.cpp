/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */
#include <dos.h>
#include "macros.h"
#include "IVTEntry.h"

IVTEntry* IVTEntry::table[] = {0};
IVTEntry::IVTEntry (IVTNo no, interrupt_t f): no(no), newRoutine(f), myEvent(0) {
	lock
	table[no] = this;

	oldRoutine = getvect(no);
	setvect(no, f);

	unlock
}

IVTEntry::~IVTEntry()
{
	setvect(no, oldRoutine);
	lock
	table[no] = 0;
	unlock
}
