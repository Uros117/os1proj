/*
 * KernelEv.h
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_KERNELEV_H_
#define INCLUDE_KERNELEV_H_

#include "macros.h"
#include "ksem.h"

class KernelEv {
public:
	KernelEv(IVTNo no);
	~KernelEv();

	void wait();
	void signal();

	IVTNo no;
	PCB* pcb;
	KernelSem sem;

};

#endif /* INCLUDE_KERNELEV_H_ */
