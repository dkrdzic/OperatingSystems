/*
 * KEvent.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: OS1
 */

#include "KEvent.h"
#include "PCB.h"
#include "IVTEntry.h"
#include "SCHEDULE.H"
#include <stdio.h>
KernelEvent::KernelEvent(IVTNo ivtNo)
{

	lock;
	myPCB = (PCB *)PCB::running;
	blocked = 0;

	IVTEntry::entry[ivtNo]->myEvent = this;

	this->ivtNo = ivtNo;
	unlock;
}

KernelEvent::~KernelEvent()
{

	lock;

	IVTEntry::entry[ivtNo]->myEvent == 0;
	if (blocked)
		signal();
	unlock;
}

void KernelEvent::wait()
{
	lock;
	if (myPCB != PCB::running)
	{
		unlock;
		return;
	}

	blocked = 1;
	myPCB->threadState = BLOCKED;
	dispatch();
	unlock;
}

void KernelEvent::signal()
{
	lock;
	if (blocked == 0)
	{
		unlock;
		return;
	}

	blocked = 0;
	myPCB->threadState = READY;
	Scheduler::put(myPCB);

	unlock;
}
