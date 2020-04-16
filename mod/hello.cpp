#include <iostream.h>
//#include <DOS.H>
//#include <stdio.h>
#include "macros.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "ThreadA.h"

unsigned oldTimerOFF, oldTimerSEG;

void interrupt timer();

int nextThread;

unsigned int tsp;
unsigned int tss;
unsigned int tbp;

volatile PCB* PCB::running;
volatile PCB* PCB::mainPCB;

void initTimer(){
	lock
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax
		// Zapamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax
		// Postavi novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer
		// Postavi staru rutinu na int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
	}
	unlock
}
// Vraca staru prekidnu rutinu
void restore(){
	lock
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es

	}
	unlock
}

volatile int cntr = 20;
volatile int context_switch_on_demand = 0;
volatile int context_switch_without_return = 0;
// Sinhrona promena konteksta
void dispatch(){
	lock
	context_switch_on_demand = 1;
	timer();
	unlock
}

void exitThread(){
	PCB::running->finished = 1;
	dispatch();
}

void suspend(){
	lock
	context_switch_without_return = 1;
	dispatch();
	unlock
}

void put(PCB* pcb){
	Scheduler::put(pcb);
}

void interrupt timer(){
	if (!context_switch_on_demand) cntr--;
	if (cntr <= 0 || context_switch_on_demand) {
#ifdef DEBUG
		cout << PCB::running->id << " -> ";
#endif
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if(PCB::running->finished != 1) {
			if(context_switch_without_return == 0){
				Scheduler::put((PCB*)PCB::running);
			}
		};
		PCB::running = Scheduler::get();

		// Ako nema vise thread-ova u Scheduler-u
		// posavi aktivan thread na main thread
		// gde je HALT
		if (PCB::running == 0){
#ifdef DEBUG_V
			cout << "Thread queue is empty! ";
#endif
			PCB::running = PCB::mainPCB;
		}

		// Ispisi promenu
#ifdef DEBUG
		cout << PCB::running->id << endl;
#endif

#ifdef DEBUG_V
		if(PCB::running->threadPointer != NULL)
			cout << "Thread " << PCB::running->threadPointer->getId() << endl;
#endif
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		cntr = PCB::running->quantum;
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	// Prosledjivanje prekida DOS-u
	if(!context_switch_on_demand) asm int 60h;

	context_switch_without_return = 0;
	context_switch_on_demand = 0;
}

int haltLoop = 1;

int main(){
	lock

#ifdef DEBUG
	cout << "Hello world!" << endl;
#endif

	unsigned long i = 0;
	initTimer();
	PCB::mainPCB = new PCB();
	PCB::running = PCB::mainPCB;
	ThreadA* ta = new ThreadA();
	ThreadA* tb = new ThreadA();
	ta->start();
	tb->start();

	unlock

	//tb->waitToComplete();
	suspend();

	while(haltLoop){
		unsigned long j = 0;
		lock
		cout<<"u main() i = "<< i <<endl;
		unlock
		while (j < 999999999l)
			j++;
		i++;
	}

	lock
#ifdef DEBUG
	cout << "RESTORE" << endl;
#endif
	restore();
	unlock
}



/*
	lock
	//unsigned long i = 0;
	cout << "Hello world!" << endl;

	mainPCB = new PCB();
	running = mainPCB;

	initTimer();
	ThreadA* ta = new ThreadA();
	unlock

	ta->start();


	// HALT
	while(haltLoop){
		unsigned long j = 0;
		lock
		cout<<"u main() i = "<< i <<endl;
		unlock

		while (j < 99999999l){
			j++;
		}
	}

	lock
	restore();
	threadLista->brisiSve();
	delete mainPCB;

	cout<<"end"<< endl;
	unlock
 */
