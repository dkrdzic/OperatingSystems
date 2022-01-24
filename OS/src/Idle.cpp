/*
 * Idle.cpp
 *
 *  Created on: Jun 19, 2019
 *      Author: OS1
 */

#include "Idle.h"
#include "PCB.h"
#include <stdio.h>

Idle::Idle() : Thread(4096, 1)
{
}

Idle::~Idle()
{
}

void Idle::run()
{
	//	lock
	//printf("U idle sam!");
	//	unlock;
	while (1)
		;
}

void Idle::start()
{

	lock;

	myPCB->threadState = IDLE;
	unlock;
}
