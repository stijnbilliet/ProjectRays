#pragma once
#include "Components\ObjectComponent.h"
#include "Patterns/ParentObject.h"

class SceneComponent : public ObjectComponent, public ParentObject<SceneComponent>
{
public:
	SceneComponent();
	virtual ~SceneComponent();

	SceneComponent(const SceneComponent& other) = delete;
	SceneComponent(SceneComponent&& other) = delete;
	SceneComponent& operator=(const SceneComponent& other) = delete;
	SceneComponent& operator=(SceneComponent&& other) = delete;

	//BaseObject implementation
	virtual void OnDraw(Renderer* pContext) const override;
	virtual void OnUpdate(float elapsedSec) override;

	//Translation accessors
	void Translate(const glm::vec2& pos);
	void Rotate(const float rot);
	void Scale(const glm::vec2& scale);

	const glm::mat4x4& GetWorldMatrix() const;
	const glm::vec2& GetLocalPosition() const;
	const glm::vec2& GetWorldPosition() const;
	const float GetLocalRotation() const;
	const float GetWorldRotation() const;
	const glm::vec2& GetLocalScale() const;
	const glm::vec2& GetWorldScale() const;

	void SetWorldPosition(const glm::vec2& pos);
	void SetWorldRotation(const float rot);
	void SetWorldScale(const glm::vec2& scale);

	bool WasTranslated() const;

	/*Does a breadth first search of it's children*/
	template<typename T>
	T* GetSceneComponent() const;
private:
	void RebuildWorldMatrix();

	glm::vec2 m_Position;
	glm::vec2 m_WorldPosition;
	float m_Rotation;
	float m_WorldRotation;
	glm::vec2 m_Scale;
	glm::vec2 m_WorldScale;
	glm::mat4x4 m_World;

	bool m_bWasTranslated;
	bool m_bIsDirty;
};

template<typename T>
inline T* SceneComponent::GetSceneComponent() const
{
	T* pComponent = nullptr;

	//Search top level children
	const type_info& typeInfo = typeid(T);
	for (auto pChildComp : GetChildren())
	{
		if (typeid(*pChildComp) == typeInfo)
		{
			pComponent = dynamic_cast<T*>(pChildComp);
		}
	}

	if (pComponent)
	{
		return pComponent;
	}
	else
	{
		//Tell our children to check their children since we haven't found it yet
		for (auto pChildComp : GetChildren())
		{
			pComponent = pChildComp->GetSceneComponent<T>();
		}
	}

	//Well I guess we haven't found it at all
	return pComponent;
}