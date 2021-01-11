/*
 * List.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: OS1
 */

#include "List.h"
#include "System.h"
#include <stdio.h>

#include "PCB.h"
#include "SCHEDULE.H"
#include "KSem.h"

List::List() {
lock;
first=0;
length=0;
unlock;
}

void List:: add(PCB* pcb){
	lock;

	Elem* cur;

	Elem* help=new Elem(pcb);
	if (first==0) first=help;
	else {
		Elem* cur=(Elem*) first;
		while(cur->next) cur=cur->next;
		cur->next=help;


	}


	length++;
unlock;
}





Thread* List:: getThreadById(ID id){
lock;

Elem* cur=(Elem*)first;

while (cur!=0 && cur->pcb->myThread->id!=id ) cur=cur->next;
if (cur==0) {unlock; return 0;}
else {unlock; return (Thread*) cur->pcb->myThread;}

}


void List:: removeThreadFromList(ID id){

	lock;

Elem* cur=(Elem*)first, *before=0;

while (cur)
	if (cur->pcb->myThread->id!=id){

		before=cur;
		cur=cur->next;

	}

	else{

		Elem* help=cur;

		cur=cur->next;
		if (before==0) first=cur;
		else before->next=cur;
delete help;
length--;
unlock;

return;
	}



}



void List:: removeWaitingThreads(ID waitingFor){

	lock;
Elem* cur=(Elem*) first, *before=0;


	while (cur)
		if (cur->pcb->waitingFor!=waitingFor){

			before=cur;
			cur=cur->next;

		}

		else{

			Elem* help=cur;
help->pcb->threadState=READY;
help->pcb->waitingFor=-1;

Scheduler::put(help->pcb);



			cur=cur->next;
			if (before==0) first=cur;
			else before->next=cur;
	delete help;

length--;
		}


	unlock;


}


void List:: checkTime(){

	lock;


	//((List*)(KernelSem::blockedThreads))->readList();

Elem* cur=(Elem*) first, *before=0;


	while (cur)
		if (cur->pcb->blockingTimeSlice==0) {before=cur;cur=cur->next;}



		else if ((--cur->pcb->blockingTimeCount)!=0){

			before=cur;

			cur=cur->next;

		}

		else{


			Elem* help=cur;
help->pcb->threadState=READY;
help->pcb->idSem=0;

Scheduler::put(help->pcb);



			cur=cur->next;

			if (before==0) first=cur;
			else before->next=cur;
	delete help;




length--;
		}


	unlock;







}


void List:: removeBlockedThreads(ID idSem){

	lock;
Elem* cur=(Elem*) first, *before=0;


	while (cur)
		if (cur->pcb->idSem!=idSem){

			before=cur;
			cur=cur->next;

		}

		else{

			Elem* help=cur;
help->pcb->threadState=READY;
help->pcb->idSem=0;

Scheduler::put(help->pcb);



			cur=cur->next;
			if (before==0) first=cur;
			else before->next=cur;
	delete help;

length--;
		}


	unlock;


}




void List:: removeFirst(ID idSem){
	lock;



Elem* cur=(Elem*)first, *before=0;

while (cur)
	if (cur->pcb->idSem!=idSem){

		before=cur;
		cur=cur->next;

	}

	else{

		Elem* help=cur;
help->pcb->idSem=0;
help->pcb->threadState=READY;
Scheduler::put(help->pcb);

		cur=cur->next;
		if (before==0) first=cur;
		else before->next=cur;
 delete help;
length--;
unlock;

return ;
	}




}



void List:: readList(){
	lock;
	Elem* cur=(Elem*) first;
	if (cur==0) printf("Prazna lista\n");
	while (cur!=0) {printf("Cekam semafor: %d, TS: %d, TC:%d, ID:%d \n", cur->pcb->idSem, cur->pcb->blockingTimeSlice,

	cur->pcb->blockingTimeCount, cur->pcb->myThread->id); cur=cur->next;}

	unlock;
}

List::~List() {
lock;

while (first){
	Elem* old=(Elem*)first;
	first=first->next;

	delete old;

}

unlock;



}

