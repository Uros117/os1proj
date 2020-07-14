/*
 * IVTEntry.h
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_IVTENTRY_H_
#define INCLUDE_IVTENTRY_H_

#include "macros.h"
#include "KernelEv.h"

#define PREPAREENTRY(no,old)						\
	void interrupt interrupt##no (...) {			\
		KernelEv* ke = (KernelEv*)IVTEntry::get(no)->myEvent;	\
		if (ke) ke->signal();						\
		if (old) {									\
			IVTEntry::get(no)->oldRoutine();		\
		}											\
		dispatch();									\
	};												\
	IVTEntry ivte##no ((IVTNo)no, interrupt##no);

//		cout << "event " << no << endl;				\


class IVTEntry {
public:
	static volatile IVTEntry* table[256];
	static IVTEntry* get (IVTNo no) { return (IVTEntry*)table[no]; };
	IVTEntry(IVTNo no, interrupt_t f);
	~IVTEntry();
	volatile KernelEv* myEvent;
	volatile IVTNo no;
	volatile interrupt_t newRoutine;
	volatile interrupt_t oldRoutine;
};

/*extern void dispatch();

void interrupt interrupt5 (...) {
	KernelEv* ke = IVTEntry::get((IVTNo)5)->myEvent;
	if (ke) ke->signal();
	if (0) {
		IVTEntry::get(5)->oldRoutine();
	}
	dispatch();
};
IVTEntry ivte5 ((IVTNo)5, interrupt5);*/

#endif /* INCLUDE_IVTENTRY_H_ */
