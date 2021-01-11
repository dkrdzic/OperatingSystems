/*
 * Idle.h
 *
 *  Created on: Jun 19, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.h"


class Idle:public Thread {
private:

friend class PCB;
friend class Thread;
friend class System;
	Idle();
	virtual ~Idle();
	virtual void run();
	void start();
};

#endif /* IDLE_H_ */
