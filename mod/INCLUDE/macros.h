#ifndef MACROS_
#define MACROS_
typedef unsigned int ID;

#define FP_SEG(x) (unsigned int)((unsigned long)(x) >> 16) /* grab 16 most significant bits */
#define FP_OFF(x) (unsigned int)((unsigned long)(x)) /* grab 16 least significant bits */

#define lock asm cli
#define unlock asm sti

typedef unsigned long StackSize;
typedef unsigned int Time;// time, x 55ms const Time defaultTimeSlice = 2; // default = 2*55ms typedef int ID;
typedef unsigned int ID;
const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 20; // FIXME: ja dodao

class PCB;
PCB* running;

#endif
