#ifndef MACROS_
#define MACROS_

//#define DEBUG 1
//#define DEBUG_V 1


typedef unsigned int ID;

#define FP_SEG(x) (unsigned int)((unsigned long)(x) >> 16) /* grab 16 most significant bits */
#define FP_OFF(x) (unsigned int)((unsigned long)(x)) /* grab 16 least significant bits */



// vezve 5 slajd 22
// zabranjuje prekide -
// na ovaj nacin opmogueno je
// ugnezdavanje parova poziva lock-unlock
#define lock asm pushf; asm cli;

#define unlock asm popf
// pushf : Push lower 16 bits of EFLAGS.



#define lock_asm asm cli
#define unlock_asm asm sti




//void mojlock();

//void mojunlock();

//#define lock mojlock();
//#define unlock mojunlock();

typedef unsigned long StackSize;
typedef unsigned int Time;// time, x 55ms const Time defaultTimeSlice = 2; // default = 2*55ms typedef int ID;
typedef unsigned int ID;
const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 20; // FIXME: ja dodao
/*
volatile unsigned int lvl = 0;


// FIXME: prototip
void lockf(){
	if(lvl == 0)
		lock
	++lvl;
}

void unlockf(){
	--lvl;
	//nece ovde upasti ako je vec zakljucano
	if(lvl == 0)
		unlock
}*/

#endif
