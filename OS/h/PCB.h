/*
 * PCB.h
 *
 *  Created on: Jun 14, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"

#define lock \
  asm pushf; \
  asm cli;
#define unlock asm popf;

enum State
{
  NEW = 0,
  READY,
  RUNNABLE,
  BLOCKED,
  FINISHED,
  IDLE
};
class Thread;

class PCB
{

private:
  friend class Thread;
  friend class List;
  friend class System;
  friend class Idle;
  friend class KernelSem;

  volatile State threadState;

  volatile unsigned sp, ss, bp;
  unsigned *stack;
  volatile ID idSem;
  volatile Time timeSlice;
  volatile Time count;
  Thread *myThread;
  volatile Time blockingTimeCount;
  volatile Time blockingTimeSlice;

  volatile static PCB *running;

  volatile static List *allThreads;
  volatile static List *waitingThreads;

  volatile ID waitingFor;

  PCB(StackSize stackSize, Time timeSlice, Thread *thread);
  PCB();

  static void wrapper();
  virtual ~PCB();
  void start();
  void waitToComplete();

  //friend int main();
  friend class KernelEvent;
};

#endif /* PCB_H_ */
