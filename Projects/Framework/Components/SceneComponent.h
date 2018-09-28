#pragma once
#include "Components\BaseComponent.h"
#include "Patterns/ParentObject.h"

class SceneComponent final : public BaseComponent
{
public:
	SceneComponent();
	virtual ~SceneComponent();

	SceneComponent(const SceneComponent& other) = delete;
	SceneComponent(SceneComponent&& other) = delete;
	SceneComponent& operator=(const SceneComponent& other) = delete;
	SceneComponent& operator=(SceneComponent&& other) = delete;

	//BaseObject implementation
	virtual void OnUpdate(float elapsedSec) override;

	//Translation accessors
	void Translate(const glm::vec3& pos);
	void Translate(const float x, const float y, const float z);
	void Rotate(const glm::quat& rotQuat);
	void Rotate(const glm::vec3& rotVec, bool bEuler);
	void Rotate(const float x, const float y, const float z, bool bEuler);
	void Scale(const glm::vec3& scale);
	void Scale(const float x, const float y, const float z);

	const glm::mat4x4& GetWorldMatrix() const;
	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetWorldPosition() const;
	const glm::quat& GetLocalRotation() const;
	const glm::quat& GetWorldRotation() const;
	const glm::vec3& GetLocalScale() const;
	const glm::vec3& GetWorldScale() const;

	bool WasTranslated() const;
private:
	void RebuildWorldMatrix();

	glm::vec3 m_Position;
	glm::vec3 m_WorldPosition;
	glm::quat m_Rotation;
	glm::quat m_WorldRotation;
	glm::vec3 m_Scale;
	glm::vec3 m_WorldScale;
	glm::mat4x4 m_World;

	bool m_bWasTranslated;
	bool m_bIsDirty;
};