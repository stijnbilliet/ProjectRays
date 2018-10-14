#pragma once
#include "Patterns/BaseObject.h"

class GameObject;
class BasePanel : public BaseObject
{
public:
	BasePanel(const std::string& name);
	virtual ~BasePanel();
	
	const std::string& GetName() const;
	void Add(GameObject* object);
	void Remove(GameObject* object);

	const std::list<GameObject*>& GetObjects() const;
protected:
	std::list<GameObject*> m_pObjects;
	std::string m_Name;
};