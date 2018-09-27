#pragma once
#include "Observer.h"
#include <memory>
#include <list>
#include "Event.h"

class Subject
{
public:
	Subject();
	virtual ~Subject();

	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);
	void notify(Event e);
private:
	std::list<Observer*> m_pObservers;
};