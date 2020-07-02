/*
 * ThreadC.cpp
 *
 *  Created on: May 11, 2020
 *      Author: OS1
 */

#include "ThreadC.h"
#include "semaphor.h"
#include "ThreadB.h"

volatile int ThreadC::work = 1;

void ThreadC::run() {
	unsigned int i = 0;
	while(i <= 10 && work){
		unsigned long j = 0;

		lock
		cout << "Bef ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		cout<< "C wait SEM0"<<endl;
		unlock

		int res = ThreadB::s->wait(0);

		lock
		cout<< "C returned from wait SEM0"<<endl;

		cout << "Aft ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		unlock

		lock
		cout<<"C " << this->getId() << " Critical section " <<" i = "<< i <<endl;
		unlock

		lock
		cout << "Bef ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		cout<< "C signal SEM1"<<endl;
		unlock

		ThreadB::s1->signal(1);

		lock
		cout << "Aft ";
		cout<<"SEM0 " << ThreadB::s->val() << " ";
		cout<<"SEM1 " << ThreadB::s1->val() << endl;
		unlock

		lock
		cout<<"C " << this->getId() << " Non-critical section " <<" i = "<< i <<endl;
		unlock

		/*while (j < 99999999l){
			j++;
		}*/
		i++;
	}
};


