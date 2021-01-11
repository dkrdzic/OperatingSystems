/*
 * Event.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "KEvent.h"
#include "PCB.h"


Event::Event(IVTNo ivtNo) {
	lock;
myImpl=new KernelEvent(ivtNo);
unlock;


}

Event::~Event() {
	lock;
	delete myImpl;
	unlock;
}


void Event::wait(){

	lock;

	myImpl->wait();
	unlock;

}

void Event::signal(){

	lock;
	myImpl->signal();
	unlock;

}
