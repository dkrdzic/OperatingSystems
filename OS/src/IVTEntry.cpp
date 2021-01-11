/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include "PCB.h"


IVTEntry* IVTEntry::entry[256]={0};

IVTEntry::IVTEntry(IVTNo ivtNo, routine newRoutine) {


	lock;
#ifndef BCC_BLOCK_IGNORE

	oldRoutine=getvect(ivtNo);
setvect(ivtNo,newRoutine);

#endif

this->ivtNo=ivtNo;

entry[ivtNo]=this;
myEvent=0;

unlock;




}

IVTEntry::~IVTEntry() {
lock;
entry[ivtNo]=0;
#ifndef BCC_BLOCK_IGNORE

setvect(ivtNo, oldRoutine);

#endif
unlock;


}


void IVTEntry:: event(){

	lock;
if (myEvent)
	myEvent->signal();
	unlock;
}


void IVTEntry::callOldRoutine(){
	lock;
	if (oldRoutine)
	oldRoutine();
unlock;

}
