/*
 * System.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: OS1
 */

#include "System.h"
#include "List.h"
#include "Idle.h"
#include "KSem.h"
#include "SCHEDULE.H"
#include <dos.h>
#include "PCB.h"
#include "stdio.h"

volatile unsigned tsp, tss, tbp;

routine System::old = 0;
volatile int System::contextSwitchRequest = 0;

volatile Idle *System::help = new Idle();

volatile Thread *System::threadMain = 0;

System::System()
{
	// TODO Auto-generated constructor stub
}

System::~System()
{
	// TODO Auto-generated destructor stub
}

void System::tick() {}

void interrupt System::timer(...)
{

	if (!contextSwitchRequest && PCB::running->timeSlice != 0)
		PCB::running->count--;

	//lock;
	//	printf("Sada radi kontekst niti:%d i vreme mu je %d\n", PCB::running->myThread->id,PCB:: running->count);
	//unlock;

	if ((PCB::running->count == 0 && PCB::running->timeSlice != 0) || contextSwitchRequest)
	{

		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp,bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running->threadState == RUNNABLE)
		{

			PCB::running->threadState = READY;
			Scheduler::put((PCB *)PCB::running);
		}

		PCB::running = (volatile PCB *)Scheduler::get();

		if (PCB::running == 0)
		{
			PCB::running = help->myPCB;
			PCB::running->count = 1;
		}

		else
		{
			PCB::running->threadState = RUNNABLE;
			PCB::running->count = PCB::running->timeSlice;
		}

		tss = PCB::running->ss;
		tsp = PCB::running->sp;
		tbp = PCB::running->bp;

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp,tbp
		}
	}

	if (contextSwitchRequest == 0)
	{
		tick();
		asm int 60h;
	}

	else
		contextSwitchRequest = 0;

	((List *)(KernelSem::blockedThreads))->checkTime();
}

void System::initSystem()
{

	lock;

#ifndef BCC_BLOCK_IGNORE

	old = getvect(0x08);
	setvect(0x08, &timer);
	setvect(0x60, old);

#endif

	Thread::createMainThread();

	((Idle *)help)->start();

	unlock;
}

void System::restoreSystem()
{

	lock;

#ifndef BCC_BLOCK_IGNORE

	setvect(0x08, old);

#endif

	unlock;
}
