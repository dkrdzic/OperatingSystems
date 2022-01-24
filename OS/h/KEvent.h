/*
 * KEvent.h
 *
 *  Created on: Jun 25, 2019
 *      Author: OS1
 */

#ifndef KEVENT_H_
#define KEVENT_H_

#include "Event.h"

class PCB;

class KernelEvent
{
private:
	KernelEvent(IVTNo ivtNo);
	virtual ~KernelEvent();
	void wait();
	void signal();
	PCB *myPCB;
	friend class Event;
	friend class PCB;
	friend class IVTEntry;
	IVTNo ivtNo;
	int blocked;
};

#endif /* KEVENT_H_ */
