#ifndef MACROS_
#define MACROS_
typedef unsigned int ID;

#define FP_SEG(x) (unsigned int)((unsigned long)(x) >> 16) /* grab 16 most significant bits */
#define FP_OFF(x) (unsigned int)((unsigned long)(x)) /* grab 16 least significant bits */

#define lock asm cli
#define unlock asm sti

#endif
