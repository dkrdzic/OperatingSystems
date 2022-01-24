/*
 * KSem.cpp
 *
 *  Created on: Jun 22, 2019
 *      Author: OS1
 */

#include "KSem.h"
#include "List.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <stdio.h>

volatile List *KernelSem::blockedThreads = new List();
ID KernelSem::static_id = 0;

KernelSem::KernelSem(int init)
{
	lock;
	val = init;
	id = ++static_id;
	unlock;
}

KernelSem::~KernelSem()
{

	lock;
	((List *)(blockedThreads))->removeBlockedThreads(id);

	unlock;
}

int KernelSem::wait(Time maxTimeToWait)
{
	lock;

	val--;

	if (val >= 0)
	{
		unlock;
		return 1;
	}

	else
	{

		((List *)(blockedThreads))->add((PCB *)PCB::running);

		PCB::running->blockingTimeCount = maxTimeToWait;
		PCB::running->blockingTimeSlice = maxTimeToWait;
		PCB::running->threadState = BLOCKED;
		PCB::running->idSem = id;
		dispatch();
	}

	if (PCB::running->blockingTimeCount == 0 && PCB::running->blockingTimeSlice != 0)
	{
		val++;
		PCB::running->blockingTimeSlice = 0;
		unlock;
		return 0;
	}
	else
	{
		PCB::running->blockingTimeCount = 0;
		PCB::running->blockingTimeSlice = 0;
		unlock;
		return 1;
	}
}

int KernelSem::signal(int n)
{
	lock;

	if (n == 0)
	{

		if (val < 0)
		{
			((List *)(blockedThreads))->removeFirst(id);

			val++;
			unlock;
			return 0;
		}

		else
		{
			val++;
			unlock;
			return 0;
		}
	}

	if (n > 0)
	{

		if (val >= 0)
		{
			val += n;
			unlock;
			return 0;
		}
		else
		{

			int help = val + n;
			int ret;
			if (help <= 0)
			{
				ret = n;
				help = n;

				while (help > 0)
				{
					((List *)(blockedThreads))->removeFirst(id);
					help--;
				}
			}

			else
			{
				ret = 0 - val;
				help = 0 - val;
				while (help > 0)
				{
					((List *)(blockedThreads))->removeFirst(id);
					help--;
				}
			}

			val += n;
			unlock;
			return ret;
		}
	}

	else
	{
		unlock;
		return n;
	}
}
