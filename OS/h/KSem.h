/*
 * KSem.h
 *
 *  Created on: Jun 22, 2019
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

#include "PCB.h"

class KernelSem
{
private:
	KernelSem(int init);
	virtual ~KernelSem();

	friend class Semaphore;
	friend class System;
	friend class List;

	int val;
	volatile ID id;
	static ID static_id;
	int wait(Time maxTimeToWait);
	int signal(int n);

	static volatile List *blockedThreads;
};

#endif /* KSEM_H_ */
