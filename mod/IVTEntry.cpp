/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */
#include <dos.h>
#include "macros.h"
#include "IVTEntry.h"

volatile IVTEntry* IVTEntry::table[] = {0};
IVTEntry::IVTEntry (IVTNo no, interrupt_t f): no(no), newRoutine(f), myEvent(0) {
	lock
	table[no] = this;

	oldRoutine = getvect(no);
	setvect(no, f);

	unlock
}

IVTEntry::~IVTEntry()
{
	lock
	setvect(no, oldRoutine);
	table[no] = 0;
	unlock
}
