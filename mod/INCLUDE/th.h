/*
 * th.h
 *
 *  Created on: May 31, 2020
 *      Author: OS1
 */

#ifndef INCLUDE_TH_H_
#define INCLUDE_TH_H_

#include "hello.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <semaphor.h>

int syncPrintf(const char*format, ...) {
	int res;
	va_list args;
	lock
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock
	return res;
}

#endif /* INCLUDE_TH_H_ */
