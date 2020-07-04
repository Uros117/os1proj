#ifndef THREADLIST_H_
#define THREADLIST_H_
#include <iostream.h>

class Thread;

class TList{
public:
	struct Elem {
		Thread* info;
		Elem* next;
		Elem(Thread* info) : info(info), next(NULL) {};
	};
private:
	volatile Elem* head;
	volatile Elem* tail;
public:

	TList();
	~TList(){
		brisiSve();
	};

	void dodaj(Thread* ob);
	void brisiSve();
	int getCount() const;
	void brisi(ID id);
	Thread* getByID(ID id);

};



#endif /* INCLUDE_THREADLIST_H_ */
