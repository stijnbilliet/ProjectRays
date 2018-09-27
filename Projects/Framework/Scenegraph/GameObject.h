#pragma once
#include "Patterns/ParentObject.h"

class Renderer;
class ObjectComponent;
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
	void AddComponent(ObjectComponent* pComp);
	void RemoveComponent(ObjectComponent* pComp);
	void SetRoot(SceneComponent* pNewRoot);
	SceneComponent* GetRootComponent() const;

	std::list<ObjectComponent*> GetAllComponents() const;

	/*Ensures all components are traversed in order to find the wanted component*/
	template<typename T>
	T* GetComponent() const;

	/*Optimized variant of GetComponent, only traverses the objectcomponent list*/
	template<typename T>
	T* GetObjectComponent() const;

	/*Optimized variant of GetComponent, only traverses the scenecomponent list*/
	template<typename T>
	T* GetSceneComponent() const;
private:
	virtual void OnChildAdd(GameObject * pChild) override;

	std::list<ObjectComponent*> m_pComponents;
	SceneComponent* m_pRootComponent;
	BasePanel* m_pScene;
};

template<typename T>
inline T* GameObject::GetComponent() const
{
	//Early objectcomponent traversal
	auto pObjComp = GetObjectComponent<T>();
	if (pObjComp) return pObjComp;

	//No objectcomponent found, so search scenecomponent and it's children
	auto pChildComp = GetSceneComponent<T>();
	if (pChildComp) return pChildComp;

	//Nothing found
	return nullptr;
}

template<typename T>
inline T* GameObject::GetObjectComponent() const
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

template<typename T>
inline T* GameObject::GetSceneComponent() const
{
	//Early check if rootcomponent is our wanted component
	const type_info& typeInfo = typeid(T);
	if (typeid(*m_pRootComponent) == typeInfo) return dynamic_cast<T*>(GetRootComponent());

	//Ask rootcomponent to check his child components
	return m_pRootComponent->GetSceneComponent<T>();
}