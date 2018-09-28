#include "FrameworkPCH.h"
#include "BaseComponent.h"
#include "Scenegraph/GameObject.h"

BaseComponent::BaseComponent()
	:m_pAttachedTo{}
{
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::Attach(GameObject* gmObj)
{
	m_pAttachedTo = gmObj;
}

void BaseComponent::Detach()
{
	m_pAttachedTo = nullptr;
}