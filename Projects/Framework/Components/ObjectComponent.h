#pragma once
#include "Patterns/BaseObject.h"

class Renderer;
class GameObject;
class ObjectComponent : public BaseObject
{
public:
	ObjectComponent();
	virtual ~ObjectComponent();

	ObjectComponent(const ObjectComponent& other) = delete;
	ObjectComponent(ObjectComponent&& other) = delete;
	ObjectComponent& operator=(const ObjectComponent& other) = delete;
	ObjectComponent& operator=(ObjectComponent&& other) = delete;

	void Attach(GameObject* gmObj);
	void Detach();
protected:
	GameObject* m_pAttachedTo;
};