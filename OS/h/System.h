/*
 * System.h
 *
 *  Created on: Jun 17, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_



typedef void interrupt(*routine)(...);




class System {
private :
	System();
	virtual ~System();


static volatile int contextSwitchRequest;
	static void interrupt timer(...);
static void initSystem();
static void restoreSystem();
friend class Thread;
friend class PCB;
friend class List;
friend class Idle;


friend int main(int argc, char* argv[]);
//friend int main();
friend void dispatch();
static volatile Idle* help;
static volatile  Thread* threadMain;

static routine old;

public:
static void tick();

};

#endif /* SYSTEM_H_ */
