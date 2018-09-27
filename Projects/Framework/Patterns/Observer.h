#pragma once
#include "Event.h"

class Observer
{
public:
	Observer();
	virtual ~Observer();

	virtual void onNotify(Event e) = 0;
};