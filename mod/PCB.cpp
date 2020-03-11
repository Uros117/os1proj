/*
 * PCB.cpp
 *
 *  Created on: Mar 5, 2020
 *      Author: OS1
 */
#include <iostream.h>
#include <dos.h>
#include "PCB.h"
#include "macros.h"

unsigned PCB::idcnt = 0;

PCB::PCB() : stack(NULL), size(0), sp(0), ss(0), quantum(20), finished(0), id(idcnt) {
	idcnt ++;
}

PCB::PCB(int size, void (*body)()) : size(size), quantum(20), finished(0), id(idcnt) {
	idcnt ++;
	stack = new unsigned [size];
	sp = FP_OFF(stack);
	ss = FP_SEG(stack);

	unsigned int newPC = FP_OFF(body);
	unsigned int newCS = FP_SEG(body);
	stack[size - 2] = newCS;
	stack[size - 3] = newPC;
	sp = FP_OFF(stack + size - 12);
}

PCB::~PCB() {
	if (stack != NULL)
		delete[] stack;
	/*size = 0;
	sp = 0;
	ss = 0;
	quantum = 20;
	finished = 0;*/
}

