/*
 * ksem.cpp
 *
 *  Created on: Apr 16, 2020
 *      Author: OS1
 */
#include "ksem.h"
#include "macros.h"
#include "PCBList.h"
#include "PCBTList.h"
#include "hello.h"
#include "SemList.h"
#include "PCB.h"
#include "Scheduler.h"

volatile SemList KernelSem::sem;

//#define DEBUG_SEM_V 1

KernelSem::KernelSem(unsigned short init){
	lock
	val = init;
	//cout << "kernel sem konstruktor" << endl;
	KernelSem::sem.dodaj(this);
	unlock
}

KernelSem::~KernelSem(){
	lock
	KernelSem::sem.brisi(this);
	unlock
}

int KernelSem::wait(Time maxTimeToWait){
	lock
	--val;
	if(val >= 0){
		PCB::running->lastTimeUnbockedMsg = 1;
		unlock
		return PCB::running->lastTimeUnbockedMsg;
	}
	// ubaci ga u normalan signal queue
	segBlocked.dodaj((PCB *)PCB::running, maxTimeToWait);

	if(maxTimeToWait != 0){
		// ubaci ga i u time queue
		timeBlocked.dodaj((PCB *)PCB::running, maxTimeToWait);
	}

	suspend();
	//cout << "msg " << PCB::running->id << " " << PCB::running->lastTimeUnbockedMsg << endl;
	// Podatak da li je odblokiran thread kroz vreme ili signal se cuva
	// u samom PCB-u u latTimeUnblockedMsg
	unlock
	return PCB::running->lastTimeUnbockedMsg;

}


int KernelSem::signal(int n){
	lock
	if(n > 0){
		// treba odblokirati onoliko niti kolika
		// je vrednost n
		// ako je vrednost blokiranih niti manja
		// od n treba ih odblokirati sve i uvecati
		// vrednost semafora
		// povratna vrednost je broj odblokiranih
		// niti





		/*int i;
		for (i = 0; i < n; ++i){
			if (segBlocked.top() == NULL) break;
			segBlocked.top()->info->lastTimeUnbockedMsg = 1;
			Scheduler::put((PCB*)segBlocked.top()->info);
			if(segBlocked.top()->max != 0) {
				timeBlocked.brisi(segBlocked.top()->info->id);
			}
			segBlocked.brisi(segBlocked.top()->info->id);
			++val;
		}
		if (n > i)
			val += (n - i);
		unlock
		return i;*/

		val += n;
		int d = (segBlocked.getCount() > n)? n: segBlocked.getCount();

		int i = 0;
		for(PCBTList::ElemPCBT* t = (ElemPCBT*)segBlocked.head; t && i < d; t = (ElemPCBT*)segBlocked.head, i++){
			t->info->lastTimeUnbockedMsg = 1;

			if(t->max != 0) {
				timeBlocked.brisi(t->info->id);
			}
			Scheduler::put((PCB *) t->info);

			segBlocked.head = t->next;
			delete t;
		}
		if(segBlocked.head == NULL) {
			segBlocked.tail = NULL;
		}
		unlock
		return d;
	} else if(n == 0){
		// ponasa se na standardan nacin
		signal(1);
		unlock
		return 0;
	} else {
		// operacija signal treba da bude bez
		// dejstva, a povratna vrednost treba
		// da bude ona koja je prihvacena
		unlock
		return n;

	}


	unlock
	return 0;
}


void KernelSem::update(){
	lock

	PCBTList::ElemPCBT* next = NULL;

	for (PCBTList::ElemPCBT* t = (ElemPCBT*)timeBlocked.head; t != NULL; t = next)
	{
		t->t -= 1;
		//cout << "update " << t->info->id << " " << t->t << endl;
		next = t->next;
		if(t->t <= 0){
			++val;
			t->info->lastTimeUnbockedMsg = 0;

			Scheduler::put((PCB*)t->info);

			segBlocked.brisi(t->info->id);
			timeBlocked.brisi(t->info->id);
		}

	}
	unlock
}
