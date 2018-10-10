#pragma once
#include "Patterns/BaseObject.h"

class Renderer;
class GameObject;
class BaseComponent : public BaseObject
{
public:
	BaseComponent();
	virtual ~BaseComponent();

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;

	void Attach(GameObject* gmObj);
	void Detach();
	GameObject* GetGameObject() const;
protected:
	GameObject* m_pAttachedTo;
};