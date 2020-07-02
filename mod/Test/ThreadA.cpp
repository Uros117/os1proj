#include "ThreadA.h"

void ThreadA::run() {
	Semaphore* s = new Semaphore(0);
	lock
	cout << "test" << endl;
	unlock

	int i = -1;
	i = s->wait(10);

	lock
	cout << i << endl;
	unlock
	delete s;

	/*unsigned int i = 0;
	while(i <= 10){
		unsigned long j = 0;
		lock
		cout<<"Thread " << this->getId() << " i = "<< i <<endl;
		unlock

		while (j < 999999999l){
			j++;
		}
		i++;
	}*/
};



