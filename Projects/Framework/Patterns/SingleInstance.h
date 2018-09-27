#pragma once
#include "BaseObject.h"

template<class T>
class SingleInstance : public BaseObject
{
public:
	SingleInstance()
	{
		assert(!m_Instantiated);
		m_Instantiated = true;
	}

	virtual ~SingleInstance()
	{
		m_Instantiated = false;
	}

protected:
	static bool m_Instantiated;
};

template<class T>
bool SingleInstance<T>::m_Instantiated = false;