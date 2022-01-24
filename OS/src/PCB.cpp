/*
 * PCB.cpp
 *
 *  Created on: Jun 14, 2019
 *      Author: OS1
 */

#include "List.h"
#include "System.h"

#include "Idle.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdio.h>
volatile List *PCB::allThreads = new List();
volatile PCB *PCB::running = 0;
volatile List *PCB::waitingThreads = new List();

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *thread)
{
	lock;

	if (stackSize >= 65536)
		stackSize = 65535;
	int size = stackSize / sizeof(unsigned);
	stack = new unsigned[size];
#ifndef BCC_BLOCK_IGNORE
	stack[size - 1] = 0x200;
	stack[size - 2] = FP_SEG(&wrapper);
	stack[size - 3] = FP_OFF(&wrapper);

	ss = FP_SEG(stack + size - 12);
	sp = FP_OFF(stack + size - 12);
	bp = sp;
#endif

	this->timeSlice = timeSlice;
	this->myThread = thread;
	threadState = NEW;
	((List *)(allThreads))->add(this);
	waitingFor = -1;
	count = this->timeSlice;
	blockingTimeCount = 0;
	blockingTimeSlice = 0;
	idSem = 0;
	unlock;
}

PCB::PCB()
{
	lock;
	this->timeSlice = defaultTimeSlice;
	stack = 0;
	sp = 0;
	ss = 0;
	bp = 0;

	myThread = (Thread *)System::threadMain;

	threadState = RUNNABLE;
	PCB::running = (volatile PCB *)this;
	waitingFor = -1;
	count = defaultTimeSlice;
	((List *)(allThreads))->add(this);

	blockingTimeCount = 0;
	blockingTimeSlice = 0;
	idSem = 0;
	unlock;
}

void PCB::start()
{
	lock;

	Scheduler::put(this);

	threadState = READY;
	unlock;
}

void PCB::wrapper()
{

	running->myThread->run();

	lock;
	running->threadState = FINISHED;

	((List *)(waitingThreads))->removeWaitingThreads(running->myThread->id);

	dispatch();

	unlock;
}

PCB::~PCB()
{
	lock;

	delete stack;
	stack = 0;
	((List *)(allThreads))->removeThreadFromList(myThread->id);

	unlock;
}
