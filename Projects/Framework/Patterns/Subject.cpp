#include "FrameworkPCH.h"
#include "Subject.h"

Subject::Subject()
{
}

Subject::~Subject()
{
	for (auto pObs : m_pObservers)
	{
		safe_delete(pObs);
	}
}

void Subject::addObserver(Observer* observer)
{
	m_pObservers.push_back(observer);
}

void Subject::removeObserver(Observer* observer)
{
	m_pObservers.remove(observer);
}

void Subject::notify(Event e)
{
	for (auto pObs: m_pObservers)
	{
		pObs->onNotify(e);
	}
}
