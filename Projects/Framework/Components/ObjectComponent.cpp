#include "FrameworkPCH.h"
#include "ObjectComponent.h"
#include "Scenegraph/GameObject.h"

ObjectComponent::ObjectComponent()
	:m_pAttachedTo{}
{
}

ObjectComponent::~ObjectComponent()
{
}

void ObjectComponent::Attach(GameObject* gmObj)
{
	m_pAttachedTo = gmObj;
}

void ObjectComponent::Detach()
{
	m_pAttachedTo = nullptr;
}