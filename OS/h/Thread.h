/*
 * Thread.h

 *
 *  Created on: Jun 14, 2019
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;

class PCB;

class Thread
{

public:
	void start();
	void waitToComplete();

	virtual ~Thread();
	ID getID();
	static ID getRunningId();
	static Thread *getThreadById(ID id);

protected:
	friend class PCB;

	Thread(StackSize stackSize = defaultStackSize,
		   Time timeSlice = defaultTimeSlice);

	Thread(ID id);

	virtual void run();

private:
	PCB *myPCB;
	static ID static_id;
	volatile ID id;
	friend class List;
	friend class System;
	friend class Idle;

	volatile int deleted;

	//friend int main();

	static void createMainThread();
};

void dispatch();

#endif /* THREAD_H_ */
