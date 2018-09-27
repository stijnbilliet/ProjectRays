#pragma once
#include "Patterns/BaseObject.h"

template<class T>
class ParentObject : public BaseObject
{
public:
	ParentObject() {};
	virtual ~ParentObject() {
		for (auto child : m_pChildren)
		{
			safe_delete(child);
		}
	};

	T* GetParent() const
	{
		return m_pParent;
	};

	std::list<T*> GetChildren() const
	{
		return m_pChildren;
	}

	void Add(T* pNew)
	{
		m_pChildren.push_back(pNew);
		pNew->m_pParent = static_cast<T*>(this);

		OnChildAdd(pNew);
	};

	void Remove(T* pToBeRemoved)
	{
		m_pChildren.remove(pToBeRemoved);
		pToBeRemoved->m_pParent = nullptr;

		OnChildRemove(pToBeRemoved);
		safe_delete(pToBeRemoved);
	};

protected:
	virtual void OnChildAdd(T* pNew) { UNREFERENCED_PARAMETER(pNew); };
	virtual void OnChildRemove(T* pToBeRemoved) { UNREFERENCED_PARAMETER(pToBeRemoved); };

	std::list<T*> m_pChildren;
	T* m_pParent;
};