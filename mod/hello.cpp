#include <iostream.h>
#include <DOS.H>
#include <stdio.h>

#include "PCB.h"
#include "macros.h"
#include "SCHEDULE.H"
unsigned oldTimerOFF, oldTimerSEG;

void interrupt timer();

PCB *p[3];
PCB* running;

int nextThread;

unsigned int tsp;
// zabranjeno ime za asembler je seg
unsigned int tss;

//makro
/*#define dispatch(x) { \
	nextThread = x; \
	_dispatch(); \
 }*/

/*void _dispatch() {
	lock
	asm {
		push ax
		push bx
		push cx
		push dx

		push si
		push di
		push bp
		push ds
		push es

		mov tsp, sp
		mov tss, ss
	}

	running->sp = tsp;
	running->ss = tss;

	Scheduler::put(running);
	running = Scheduler::get();
	//running = p[nextThread];


	tsp = running->sp;
	tss = running->ss;
	asm {
		mov sp, tsp
		mov ss, tss

		pop es
		pop ds
		pop bp
		pop di
		pop si

		pop dx
		pop cx
		pop bx
		pop ax
	}
	unlock
}*/


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

		mov ax, 0;
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

// sinhrona promena konteksta
void dispatch(){
	lock
	context_switch_on_demand = 1;
	timer();
	unlock
}

// poslednja funkcija koju svaka nit mora da pozove
// - zasto?
void exitThread(){
	running->finished = 1;
	dispatch();
}


void interrupt timer(){
	//timer nije locked ?
	//cout << " " << cntr;
	if (!context_switch_on_demand) cntr--;
	if (cntr <= 0 || context_switch_on_demand) {
		cout << running->id << " -> ";
		asm {
			mov tsp, sp
			mov tss, ss
		}
		running->sp = tsp;
		running->ss = tss;
		if(running->finished != 1){
			Scheduler::put(running);
		}
		running = Scheduler::get();
		//running = p[nextThread];
		cout << running->id << endl;

		tsp = running->sp;
		tss = running->ss;
		cntr = running->quantum;
		asm {
			mov sp, tsp
			mov ss, tss
		}
}

	// poziv stare prekidne rutine
	// koja se nalazila na 08h, a sad je na 60h;
	// poziva se samo kada
	// nije zahtevana promena konteksta
	// tako da se stara rutina poziva
	// samo kada je stvarno doslo do prekida
	if(!context_switch_on_demand) asm int 60h;

	context_switch_on_demand = 0;
}


void a(){
	for (int i =0; i < 30; ++i) {
		unsigned long j = 0;
		lock
		cout<<"u a() i = "<< i <<endl;
		unlock

		while (j < 999999999l){
			j++;
		}
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		unsigned long j = 0;
		lock
		cout<<"u b() i = "<< i <<endl;
		unlock

		while (j < 999999999l){
			j++;
		}
	}
	exitThread();
}


void delete_all() {
	//delete [] p[1]->stack;
	//delete [] p[2]->stack;
	delete p[0];
	delete p[1];
	delete p[2];
}


void doSomething(){
	int i=0;
	lock
	p[1] = new PCB(1024, &a);
	cout<<"Napravio a"<<endl;
	p[1]->quantum = 20;

	p[2] = new PCB(1024, &b);
	cout<<"Napravio b"<<endl;
	p[2]->quantum = 20;

	p[0] = new PCB();
	p[0]->quantum = 20;
	running = p[0];

	Scheduler::put(p[1]);
	Scheduler::put(p[2]);
	unlock

	for (i=0; i < 30; ++i) {
		unsigned long j = 0;
		lock
		cout<<"main "<<i<<endl;
		unlock
		while (j < 9999999999l){
			j++;
		}
	}
	cout<<"Srecan kraj!"<<endl;
}

int main(){

	initTimer();

	doSomething();

	restore();

	return 0;

}


/*int main() {
	//cout << sizeof(unsigned short) << endl;
	//cout << sizeof(unsigned int) << endl;
	//cout << sizeof(void (*)()) << endl;

    p[1] = new PCB(1024, &a);
	//createThread(p[1], &a);
	cout << "napravio a" << endl;

	p[2] = new PCB(1024, &b);
	//createThread(p[2], &b);
	cout << "napravio b" << endl;

	p[0] = new PCB();
	running = p[0];

	cout << "kraj" << endl;
	delete_all();
	return 0;
}*/
