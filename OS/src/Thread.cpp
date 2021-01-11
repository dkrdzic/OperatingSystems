/*
 * Thread.cpp


 *
 *  Created on: Jun 14, 2019
 *      Author: OS1
 */



#include "List.h"
#include "System.h"
#include "stdio.h"

#include "PCB.h"
#include "SCHEDULE.H"
int Thread:: static_id=0;



Thread::Thread(StackSize stackSize, Time timeSlice){

lock;
	id=++static_id;

myPCB=new PCB(stackSize, timeSlice, this);

deleted=0;


unlock;
}

Thread:: Thread(ID id){
lock;

this->id=id;
myPCB=0;
deleted=0;
unlock;

}



void Thread:: start(){

	lock;

	if (myPCB->threadState==NEW)
	((PCB*)(myPCB))->start();


  unlock;

}

void Thread:: createMainThread(){
lock;

	System::threadMain=new Thread(0);
	System::threadMain->myPCB=new PCB();


unlock;
}


void Thread:: run(){}


Thread* Thread:: getThreadById(ID id){

return  ((List*)(PCB::allThreads))->getThreadById(id);

}



ID Thread:: getID(){    return id;}

ID Thread::getRunningId(){return ((Thread*)(PCB::running->myThread))->getID();}

void Thread::waitToComplete(){

	lock;


	if (myPCB->threadState==FINISHED )
	{unlock; return;}


	if (myPCB->threadState==NEW )
		{unlock; return;}


	if	(myPCB==PCB::running)
	{unlock; return;}


	if 	(this==System::help)
	{unlock; return;}


	if( this==System::threadMain)
	{unlock; return;}

if(deleted==1){
	unlock; return;
}



 PCB:: running->threadState=BLOCKED;


PCB::running->waitingFor=myPCB->myThread->id;

((List*)(PCB::waitingThreads))->add((PCB*)PCB::running);



  dispatch();
unlock;





}



Thread::~Thread() {

lock;

waitToComplete();

	delete myPCB;
myPCB=0;
deleted=1;

   unlock;
}



void dispatch(){
lock;

System::contextSwitchRequest=1;
System::timer();


unlock;


}







