#pragma once
#include "Patterns/ParentObject.h"

class Renderer;
class BaseComponent;
class SceneComponent;
class BasePanel;
class Transform;
class GameObject : public ParentObject<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	//BaseObject functionality
	virtual void OnInit() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnDraw(Renderer* pContext) const override;

	//Scene accesors
	BasePanel* GetScene();
	void SetScene(BasePanel* pScene);

	//Component accesors
	void AddComponent(BaseComponent* pComp);
	void RemoveComponent(BaseComponent* pComp);

	SceneComponent* GetTransform() const;
	std::list<BaseComponent*> GetAllComponents() const;

	/*Ensures all components are traversed in order to find the wanted component*/
	template<typename T>
	T* GetComponent() const;
private:
	virtual void OnChildAdd(GameObject * pChild) override;

	std::list<BaseComponent*> m_pComponents;
	SceneComponent* m_pRootComponent;
	BasePanel* m_pScene;
};

template<typename T>
inline T* GameObject::GetComponent() const
{
	const type_info& typeInfo = typeid(T);
	for (auto pComp : m_pComponents)
	{
		if (typeid(*pComp) == typeInfo)
		{
			return dynamic_cast<T*>(pComp);
		}
	}
	return nullptr;
}