#include "ThreadB.h"
#include "semaphor.h"
#include "ThreadC.h"
volatile Semaphore* ThreadB::s = new Semaphore(0);
volatile Semaphore* ThreadB::s1 = new Semaphore(1);

void ThreadB::run() {
	unsigned int i = 0;
	ThreadC c1;
	ThreadC c2;
	c1.start();
	//c2.start();

	lock
	cout<<"SEM1 " << ThreadB::s1->val() <<endl;
	cout<<"SEM0 " << ThreadB::s->val() <<endl;
	unlock
	while(i <= 100){
		lock
		cout<<"B " << this->getId() << " Non-critical section " <<" i = "<< i <<endl;
		unlock

		/*unsigned long j = 0;
		while (j < 99999999l){
			j++;
		}*/
		i++;


		lock
		cout << "Bef ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		cout<< "B wait SEM1"<<endl;
		unlock

		ThreadB::s1->wait(0);

		lock
		cout<< "B returned from wait SEM1"<<endl;
		cout << "Aft ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		unlock


		lock
		cout<<"B " << this->getId() << " Critical section " <<" i = "<< i <<endl;
		unlock

		lock
		cout << "Bef ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		cout<< "B signal SEM0"<<endl;
		unlock

		ThreadB::s->signal(1);

		lock
		cout << "Aft ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		unlock


	}
	ThreadC::work = 0;
	//c1.waitToComplete();
	//c2.waitToComplete();
};




