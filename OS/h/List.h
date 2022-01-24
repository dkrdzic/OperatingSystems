/*
 * List.h
 *
 *  Created on: Jun 16, 2019
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "Thread.h"

class Thread;
class PCB;

class List
{
private:
	friend class PCB;
	friend class Thread;
	friend class KernelSem;
	friend class System;

	struct Elem
	{
		PCB *pcb;
		Elem *next;
		Elem(PCB *p, Elem *n = 0)
		{
			pcb = p;
			next = n;
		}
	};

	volatile int length;

	volatile Elem *first;

	List();
	void add(PCB *pcb);

	void removeThreadFromList(ID id);
	void removeBlockedThreads(ID semID);
	void removeWaitingThreads(ID id);

	void removeFirst(ID idSem);

	void checkTime();

	Thread *getThreadById(ID waitingFor);

	virtual ~List();

public:
	void readList();
};

#endif /* LIST_H_ */
