#include <iostream.h>
#include <DOS.H>
#include <stdio.h>
#include "hello.h"
#include "macros.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "ksem.h"
#include "SemList.h"
#include "SIdList.h"
#include "ksignal.h"

extern userMain (int argc, char* argv[]);
extern void tick ();
unsigned oldTimerOFF, oldTimerSEG;

void interrupt timer();

int nextThread;

unsigned int tsp;
unsigned int tss;
unsigned int tbp;



volatile PCB* PCB::running;
volatile PCB* PCB::idlePCB;
volatile int Thread::glob_blocked = 0;

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
volatile int context_switch_disabled = 0;
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


SignalId temp = SIGNALID_ERROR;

void interrupt timer(){

	if (!context_switch_on_demand) {
		tick();
		KernelSem::sem.update();
	}

	// Prosledjivanje prekida DOS-u
	if(!context_switch_on_demand) asm int 60h;

	if (!context_switch_disabled) {// || context_switch_on_demand
		// Signali
		if (!Thread::glob_blocked && PCB::running->threadPointer) {//&& !context_switch_on_demand
			context_switch_disabled = 1;
			//cout << "signal check" << endl;
			// pop context

			// process signals
			temp = PCB::running->threadPointer->signalQueue.getTop();
			while (temp != SIGNALID_ERROR) {
				PCB::running->threadPointer->signals[temp].update();

				temp = PCB::running->threadPointer->signalQueue.getTop();
			}
			context_switch_disabled = 0;
		}

		if((!PCB::running->quantum == 0) || context_switch_on_demand)
		{
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
					PCB::running = PCB::idlePCB;
				}

				// Ispisi promenu
		#ifdef DEBUG
				cout << PCB::running->id << endl;
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
		}

	}
	context_switch_without_return = 0;
	context_switch_on_demand = 0;
}

volatile int haltLoop = 1;

void idle() {
	//unsigned long i = 0;
	while(haltLoop){
		/*unsigned long j = 0;
		lock
		cout<<"idle "<< i <<endl;
		unlock
		while (j < 999999999l)
			j++;
		i++;*/
	}
	exitThread();
}
volatile int* argc;
volatile char*** argv = 0;

volatile int rezultat = -1;

void MainThread::run() {
	rezultat = userMain (*argc, (char**) *argv);
}

int main(int argc, char* argv[])
{
	lock
	::argc = &argc;
	::argv = &argv;
	initTimer();
	PCB::idlePCB = new PCB(1024, 10, &idle);
	PCB::running = new PCB();

	MainThread* mt = new MainThread();
	unlock

	//int rezultat = userMain (argc, argv);
	mt->start();
	mt->waitToComplete();

	lock
	restore();
	delete mt;
	delete PCB::idlePCB;
	delete PCB::running;
	unlock
	return rezultat;
}
