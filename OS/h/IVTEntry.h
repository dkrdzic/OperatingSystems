/*
 * IVTEntry.h
 *
 *  Created on: Jun 25, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "System.h"
#include "KEvent.h"
#include <stdio.h>

#define PREPAREENTRY(IVTNum, callOld)                             \
	void interrupt eventHappened##IVTNum(...);                    \
	IVTEntry currentEntry##IVTNum(IVTNum, eventHappened##IVTNum); \
	void interrupt eventHappened##IVTNum(...)                     \
	{                                                             \
		if (callOld)                                              \
			currentEntry##IVTNum.callOldRoutine();                \
		currentEntry##IVTNum.event();                             \
		dispatch();                                               \
	}

class IVTEntry
{

	routine oldRoutine;
	IVTNo ivtNo;

	KernelEvent *myEvent;

public:
	static IVTEntry *entry[256];

	void event();

	IVTEntry(IVTNo ivtNo, routine newRoutine);
	virtual ~IVTEntry();
	void callOldRoutine();
	friend class KernelEvent;
};

#endif /* IVTENTRY_H_ */
