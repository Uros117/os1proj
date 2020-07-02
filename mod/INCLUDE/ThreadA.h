#ifndef INCLUDE_THREADA_H_
#define INCLUDE_THREADA_H_

#include "macros.h"
#include "thread.h"
#include "semaphor.h"

class ThreadA : public Thread{
public:

	ThreadA():Thread(){

	};
protected:
	virtual void run();
};
#endif /*asdf INCLUDE_THREADA_H_ */
