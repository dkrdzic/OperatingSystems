/*
 * Sem.cpp

 *
 *  Created on: Jun 22, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "KSem.h"
#include "Semaphor.h"
Semaphore::Semaphore(int init) {
lock;

myImpl=new KernelSem(init);
unlock;

}

Semaphore::~Semaphore() {
	lock;
delete myImpl;
	unlock;
}

int Semaphore::wait(Time maxTimeToWait){
lock;
myImpl->wait(maxTimeToWait);
unlock;

}

int Semaphore::signal(int n){
lock;
myImpl->signal(n);
unlock;


}


int Semaphore::val() const{


return myImpl->val;


}
