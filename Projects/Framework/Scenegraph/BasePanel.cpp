#include "FrameworkPCH.h"
#include "BasePanel.h"
#include "Scenegraph/GameObject.h"

BasePanel::BasePanel(const std::string& name)
	:m_Name(name)
{
}

BasePanel::~BasePanel()
{
	for (auto obj : m_pObjects)
	{
		safe_delete(obj);
	}
}

const std::string & BasePanel::GetName() const
{
	return m_Name;
}

void BasePanel::Add(GameObject * object)
{
	m_pObjects.push_back(object);
	object->SetScene(this);
}

void BasePanel::Remove(GameObject * object)
{
	m_pObjects.remove(object);
}

const std::list<GameObject*>& BasePanel::GetObjects() const
{
	return m_pObjects;
}
